# Complemento de restauración de sesión para Gedit

**Leer en otros idiomas:** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Português](README.pt.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

Un complemento de gedit que recuerda los archivos abiertos y los restaura en el próximo inicio, como la función «Continuar donde lo dejaste» de Chrome.

Escrito en C para gedit 49+, que [ya no admite complementos de Python](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## Cómo funciona

- Al cerrar la ventana: guarda los URI de todos los archivos abiertos en `~/.local/share/gedit/session.txt`
- Al abrir la ventana: restaura los archivos como pestañas (omite los archivos que ya no existen)

## Comportamiento conocido

- Al iniciar, gedit siempre crea una pestaña vacía sin título antes de ejecutar los complementos. Después de restaurar los archivos de la sesión, esta pestaña vacía se cierra automáticamente, por lo que puede que la veas aparecer brevemente y desaparecer.

- El archivo de sesión solo se actualiza cuando hay al menos un archivo guardado abierto. Si cierras manualmente todos los archivos (dejando solo un documento vacío), la sesión no se borra, por lo que esos archivos se restaurarán de nuevo en el próximo inicio.

- Diseñado para uso con una sola ventana. Todas las ventanas comparten el mismo `session.txt`, por lo que usar varias ventanas simultáneamente puede provocar que las sesiones se sobrescriban entre sí o que se restauren pestañas duplicadas.

- En algunos entornos (por ejemplo, GNOME Shell en Wayland), activar este complemento puede hacer que el dash deje de mostrar el punto indicador de ejecución de gedit. Se debe a que el complemento restaura las pestañas y cierra la pestaña vacía inicial al inicio, lo que puede romper la asociación ventana-aplicación de GNOME Shell. Solución alternativa: copia `/usr/share/applications/org.gnome.gedit.desktop` en `~/.local/share/applications/` y añade `StartupWMClass=gedit`.

## Compilación

Requiere `gedit-devel` y `meson`:

```bash
# Fedora
sudo dnf install gedit-devel meson

# Compilación
meson setup builddir
meson compile -C builddir
```

## Instalación

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Luego activa «Session Restore» en gedit → Preferencias → Complementos.

## Licencia

MIT
