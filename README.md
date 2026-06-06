# Gedit Session Restore Plugin

A gedit plugin that remembers your open files and restores them on next startup — like Chrome's "Continue where you left off".

Written in C for gedit 49+ which [no longer supports Python plugins](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## How It Works

- On window close: saves all open file URIs to `~/.local/share/gedit/session.txt`
- On window open: restores files as tabs (skips files that no longer exist)

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

## License

MIT
