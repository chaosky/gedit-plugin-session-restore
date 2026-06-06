#include <gedit/gedit-window-activatable.h>
#include <gedit/gedit-window.h>
#include <gedit/gedit-document.h>
#include <gedit/gedit-commands.h>
#include <libpeas/peas.h>

#define SESSION_RESTORE_TYPE_WINDOW_ACTIVATABLE (session_restore_window_activatable_get_type ())
G_DECLARE_FINAL_TYPE (SessionRestoreWindowActivatable, session_restore_window_activatable,
                      SESSION_RESTORE, WINDOW_ACTIVATABLE, GObject)

struct _SessionRestoreWindowActivatable
{
	GObject parent;
	GeditWindow *window;
	gulong tab_added_id;
	gulong tab_removed_id;
	guint save_idle_id;
	guint cleanup_idle_id;
};

static void session_restore_window_activatable_iface_init (GeditWindowActivatableInterface *iface);
G_DEFINE_TYPE_WITH_CODE (SessionRestoreWindowActivatable, session_restore_window_activatable, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GEDIT_TYPE_WINDOW_ACTIVATABLE, session_restore_window_activatable_iface_init))

enum { PROP_0, PROP_WINDOW };

static gchar *
get_session_file_path (void)
{
	return g_build_filename (g_get_user_data_dir (), "gedit", "session.txt", NULL);
}

static void
session_restore_window_activatable_set_property (GObject *object, guint prop_id,
                                                 const GValue *value, GParamSpec *pspec)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (object);
	if (prop_id == PROP_WINDOW)
		self->window = g_value_get_object (value);
	else
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
}

static void
session_restore_window_activatable_get_property (GObject *object, guint prop_id,
                                                 GValue *value, GParamSpec *pspec)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (object);
	if (prop_id == PROP_WINDOW)
		g_value_set_object (value, self->window);
	else
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
}

static void
save_session (SessionRestoreWindowActivatable *self)
{
	GList *documents = gedit_window_get_documents (self->window);
	GString *session = g_string_new (NULL);

	for (GList *l = documents; l != NULL; l = l->next)
	{
		GeditDocument *doc = GEDIT_DOCUMENT (l->data);
		GtkSourceFile *source_file = gedit_document_get_file (doc);
		GFile *location = gtk_source_file_get_location (source_file);

		if (location != NULL)
		{
			gchar *uri = g_file_get_uri (location);
			g_string_append (session, uri);
			g_string_append_c (session, '\n');
			g_free (uri);
		}
	}

	g_list_free (documents);

	/* Only write if there's something to save */
	if (session->len > 0)
	{
		gchar *path = get_session_file_path ();
		g_file_set_contents (path, session->str, session->len, NULL);
		g_free (path);
	}

	g_string_free (session, TRUE);
}

static gboolean
save_session_idle_cb (gpointer user_data)
{
	SessionRestoreWindowActivatable *self = user_data;
	self->save_idle_id = 0;
	save_session (self);
	return G_SOURCE_REMOVE;
}

static void
schedule_save (SessionRestoreWindowActivatable *self)
{
	if (self->save_idle_id == 0)
		self->save_idle_id = g_idle_add (save_session_idle_cb, self);
}

static void
on_tab_added (GeditWindow *window, GeditTab *tab, gpointer user_data)
{
	schedule_save (SESSION_RESTORE_WINDOW_ACTIVATABLE (user_data));
}

static void
on_tab_removed (GeditWindow *window, GeditTab *tab, gpointer user_data)
{
	schedule_save (SESSION_RESTORE_WINDOW_ACTIVATABLE (user_data));
}

static gboolean
restore_session (SessionRestoreWindowActivatable *self)
{
	gchar *path = get_session_file_path ();
	gchar *contents = NULL;
	gboolean restored = FALSE;

	if (!g_file_get_contents (path, &contents, NULL, NULL))
	{
		g_free (path);
		return FALSE;
	}

	gchar **uris = g_strsplit (contents, "\n", -1);
	for (gint i = 0; uris[i] != NULL; i++)
	{
		if (uris[i][0] == '\0')
			continue;

		GFile *location = g_file_new_for_uri (uris[i]);
		if (g_file_query_exists (location, NULL))
		{
			gedit_commands_load_location (self->window, location, NULL, 0, 0);
			restored = TRUE;
		}
		g_object_unref (location);
	}

	g_strfreev (uris);
	g_free (contents);
	g_free (path);
	return restored;
}

/* Close the initial empty untitled document gedit creates on startup */
static gboolean
cleanup_idle_cb (gpointer user_data)
{
	SessionRestoreWindowActivatable *self = user_data;
	GList *documents = gedit_window_get_documents (self->window);

	if (g_list_length (documents) > 1)
	{
		for (GList *l = documents; l != NULL; l = l->next)
		{
			GeditDocument *doc = GEDIT_DOCUMENT (l->data);
			GtkTextBuffer *buf = GTK_TEXT_BUFFER (doc);

			if (gtk_source_file_get_location (gedit_document_get_file (doc)) == NULL &&
			    gtk_text_buffer_get_char_count (buf) == 0 &&
			    !gtk_text_buffer_get_modified (buf))
			{
				gedit_window_close_tab (self->window, gedit_tab_get_from_document (doc));
				break;
			}
		}
	}

	g_list_free (documents);
	self->cleanup_idle_id = 0;
	return G_SOURCE_REMOVE;
}

static void
session_restore_window_activate (GeditWindowActivatable *activatable)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (activatable);

	if (restore_session (self))
		self->cleanup_idle_id = g_idle_add (cleanup_idle_cb, self);

	self->tab_added_id = g_signal_connect (self->window, "tab-added",
	                                       G_CALLBACK (on_tab_added), self);
	self->tab_removed_id = g_signal_connect (self->window, "tab-removed",
	                                         G_CALLBACK (on_tab_removed), self);
}

static void
session_restore_window_deactivate (GeditWindowActivatable *activatable)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (activatable);

	/* Cancel any pending idle save to avoid accessing destroyed window */
	if (self->save_idle_id > 0)
	{
		g_source_remove (self->save_idle_id);
		self->save_idle_id = 0;
	}
	if (self->cleanup_idle_id > 0)
	{
		g_source_remove (self->cleanup_idle_id);
		self->cleanup_idle_id = 0;
	}

	if (self->tab_added_id > 0)
	{
		g_signal_handler_disconnect (self->window, self->tab_added_id);
		self->tab_added_id = 0;
	}
	if (self->tab_removed_id > 0)
	{
		g_signal_handler_disconnect (self->window, self->tab_removed_id);
		self->tab_removed_id = 0;
	}
}

static void
session_restore_window_activatable_iface_init (GeditWindowActivatableInterface *iface)
{
	iface->activate = session_restore_window_activate;
	iface->deactivate = session_restore_window_deactivate;
}

static void
session_restore_window_activatable_class_init (SessionRestoreWindowActivatableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->set_property = session_restore_window_activatable_set_property;
	object_class->get_property = session_restore_window_activatable_get_property;
	g_object_class_override_property (object_class, PROP_WINDOW, "window");
}

static void
session_restore_window_activatable_init (SessionRestoreWindowActivatable *self)
{
}

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
	peas_object_module_register_extension_type (module, GEDIT_TYPE_WINDOW_ACTIVATABLE,
	                                           SESSION_RESTORE_TYPE_WINDOW_ACTIVATABLE);
}
