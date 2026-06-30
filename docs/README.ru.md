# Плагин восстановления сессии для Gedit

**На других языках:** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Français](README.fr.md) · [Español](README.es.md) · [Português](README.pt.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

Плагин для gedit, который запоминает открытые файлы и восстанавливает их при следующем запуске — как функция Chrome «Продолжить с того же места».

Написан на C для gedit 49+, который [больше не поддерживает плагины на Python](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## Как это работает

- При закрытии окна: сохраняет URI всех открытых файлов в `~/.local/share/gedit/session.txt`
- При открытии окна: восстанавливает файлы как вкладки (пропускает несуществующие файлы)

## Известное поведение

- При запуске gedit всегда создаёт пустую безымянную вкладку до запуска плагинов. После восстановления файлов сессии эта пустая вкладка автоматически закрывается, поэтому вы можете заметить, как она ненадолго появляется и исчезает.

- Файл сессии обновляется только когда открыт хотя бы один сохранённый файл. Если вы вручную закроете все файлы (оставив только пустой документ), сессия не очищается, поэтому эти файлы будут восстановлены снова при следующем запуске.

- Рассчитан на использование с одним окном. Все окна используют один и тот же `session.txt`, поэтому одновременное использование нескольких окон может привести к взаимной перезаписи сессий или восстановлению дублирующихся вкладок.

- В некоторых окружениях (например, GNOME Shell на Wayland) включение этого плагина может привести к тому, что в dash перестанет отображаться индикатор запущенного приложения (точка) для gedit. Это вызвано тем, что плагин восстанавливает вкладки и закрывает начальную пустую вкладку при запуске, что нарушает связь окна с приложением в GNOME Shell. Решение: скопируйте `/usr/share/applications/org.gnome.gedit.desktop` в `~/.local/share/applications/` и добавьте `StartupWMClass=gedit`.

## Сборка

Требуются `gedit-devel` и `meson`:

```bash
# Fedora
sudo dnf install gedit-devel meson

# Сборка
meson setup builddir
meson compile -C builddir
```

## Установка

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Затем включите «Session Restore» в gedit → Параметры → Модули.

## Устранение неполадок

### `libgedit-49.so: cannot open shared object file: No such file or directory`

Если после обновления gedit плагин перестал загружаться и вы видите примерно следующее:

```
Failed to load module 'sessionrestore': libgedit-49.so: cannot open shared object file: No such file or directory
Error loading plugin 'sessionrestore'
```

это **ожидаемо после обновления мажорной версии gedit**. Скомпилированный `.so` слинкован с тем конкретным `libgedit-<N>.so`, который был установлен на момент сборки (например, `libgedit-49.so` для gedit 49). При обновлении gedit (например, до 50) старая библиотека удаляется, и плагин больше не может её найти.

**Решение: просто пересоберите под новый gedit и переустановите.**

```bash
rm -rf builddir
meson setup builddir
meson compile -C builddir
cp builddir/libsessionrestore.so ~/.local/share/gedit/plugins/session-restore/
```

Затем перезапустите gedit. Это нужно делать один раз после каждого обновления мажорной версии gedit.

## Лицензия

MIT
