# Gedit Session Restore Plugin

**Read this in other languages:** [日本語](docs/README.ja.md) · [中文](docs/README.zh.md) · [Русский](docs/README.ru.md) · [Français](docs/README.fr.md) · [Español](docs/README.es.md) · [Português](docs/README.pt.md) · [Italiano](docs/README.it.md) · [한국어](docs/README.ko.md)

A gedit plugin that remembers your open files and restores them on next startup — like Chrome's "Continue where you left off".

Written in C for gedit 49+ which [no longer supports Python plugins](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## How It Works

- On window close: saves all open file URIs to `~/.local/share/gedit/session.txt`
- On window open: restores files as tabs (skips files that no longer exist)

## Known Behavior

- On startup, gedit always creates an empty untitled tab before plugins run. After
  the session files are restored, this empty tab is closed automatically, so you may
  briefly see it appear and then disappear.

- The session file is only updated when at least one saved file is open. If you
  manually close all files (leaving only an empty document), the session is not
  cleared, so those files will be restored again on next startup.

- Designed for single-window use. All windows share the same `session.txt`, so
  using multiple windows simultaneously may cause sessions to overwrite each other
  or restore duplicate tabs.

- On some systems (e.g. GNOME Shell on Wayland), enabling this plugin may stop the
  dash from showing the running indicator dot for gedit. This is triggered by the
  plugin restoring tabs and closing the initial empty tab at startup, which can
  break GNOME Shell's window-to-application association. Workaround: copy
  `/usr/share/applications/org.gnome.gedit.desktop` to `~/.local/share/applications/`
  and add `StartupWMClass=gedit`.

## Build

Requires `gedit-devel` and `meson`:

```bash
# Fedora
sudo dnf install gedit-devel meson

# Build
meson setup builddir
meson compile -C builddir
```

## Install

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Then enable "Session Restore" in gedit → Preferences → Plugins.

## Troubleshooting

### `libgedit-49.so: cannot open shared object file: No such file or directory`

If the plugin stops loading after a gedit upgrade and you see something like:

```
Failed to load module 'sessionrestore': libgedit-49.so: cannot open shared object file: No such file or directory
Error loading plugin 'sessionrestore'
```

this is **expected after a gedit major-version upgrade**. The compiled `.so` is
linked against the exact `libgedit-<N>.so` that was installed when you built it
(e.g. `libgedit-49.so` for gedit 49). When gedit is upgraded (e.g. to 50), the
old library is removed, so the plugin can no longer find it.

**Fix: just rebuild against the new gedit and reinstall.**

```bash
rm -rf builddir
meson setup builddir
meson compile -C builddir
cp builddir/libsessionrestore.so ~/.local/share/gedit/plugins/session-restore/
```

Then restart gedit. You need to do this once after every gedit major-version
upgrade.

## License

MIT
