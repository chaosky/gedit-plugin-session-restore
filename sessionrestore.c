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
restore_session (SessionRestoreWindowActivatable *self)
{
	gchar *path = get_session_file_path ();
	gchar *contents = NULL;
	gsize len = 0;

	if (!g_file_get_contents (path, &contents, &len, NULL))
	{
		g_free (path);
		return;
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
		}
		g_object_unref (location);
	}

	g_strfreev (uris);
	g_free (contents);
	g_free (path);
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

	gchar *path = get_session_file_path ();
	g_file_set_contents (path, session->str, session->len, NULL);
	g_free (path);
	g_string_free (session, TRUE);
}

static void
session_restore_window_activate (GeditWindowActivatable *activatable)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (activatable);
	restore_session (self);
}

static void
session_restore_window_deactivate (GeditWindowActivatable *activatable)
{
	SessionRestoreWindowActivatable *self = SESSION_RESTORE_WINDOW_ACTIVATABLE (activatable);
	save_session (self);
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
