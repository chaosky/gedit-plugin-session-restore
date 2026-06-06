# Gedit 会话恢复插件

**其他语言:** [English](../README.md) · [日本語](README.ja.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Español](README.es.md) · [Português](README.pt.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

一个 gedit 插件，记住你打开的文件，并在下次启动时恢复——就像 Chrome 的"继续浏览上次打开的网页"功能。

用 C 为 gedit 49+ 编写（该版本[不再支持 Python 插件](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html)）。

## 工作原理

- 关闭窗口时：将所有打开文件的 URI 保存到 `~/.local/share/gedit/session.txt`
- 打开窗口时：将文件作为标签页恢复（跳过已不存在的文件）

## 已知行为

- 启动时，gedit 总会在插件运行前创建一个空的未命名标签页。会话文件恢复后，这个空标签会被自动关闭，所以你可能会看到它短暂出现又消失。

- 仅当至少有一个已保存的文件处于打开状态时，会话文件才会更新。如果你手动关闭所有文件（只剩一个空文档），会话不会被清空，因此这些文件会在下次启动时再次恢复。

- 为单窗口使用设计。所有窗口共享同一个 `session.txt`，因此同时使用多个窗口可能导致会话互相覆盖或恢复重复的标签页。

- 在某些环境下（例如 Wayland 上的 GNOME Shell），启用此插件后 dash 可能不再为 gedit 显示运行指示点。这是由于插件在启动时恢复标签页并关闭初始空标签页，破坏了 GNOME Shell 的窗口与应用关联导致的。解决方法：将 `/usr/share/applications/org.gnome.gedit.desktop` 复制到 `~/.local/share/applications/` 并添加 `StartupWMClass=gedit`。

## 构建

需要 `gedit-devel` 和 `meson`：

```bash
# Fedora
sudo dnf install gedit-devel meson

# 构建
meson setup builddir
meson compile -C builddir
```

## 安装

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

然后在 gedit → 首选项 → 插件 中启用 "Session Restore"。

## 许可证

MIT
