# Gedit セッション復元プラグイン

**他の言語で読む:** [English](../README.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Español](README.es.md) · [Português](README.pt.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

開いているファイルを記憶し、次回起動時に復元する gedit プラグインです。Chrome の「前回開いていたページを再度開く」機能のようなものです。

Python プラグインに[対応しなくなった](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html) gedit 49+ 向けに C で書かれています。

## 動作の仕組み

- ウィンドウを閉じるとき: 開いているすべてのファイルの URI を `~/.local/share/gedit/session.txt` に保存します
- ウィンドウを開くとき: ファイルをタブとして復元します（存在しないファイルはスキップします）

## 既知の動作

- 起動時、gedit はプラグインが実行される前に必ず空の無題タブを作成します。セッションファイルが復元された後、この空のタブは自動的に閉じられるため、一瞬表示されてすぐに消えることがあります。

- セッションファイルは、保存済みのファイルが少なくとも 1 つ開いているときにのみ更新されます。すべてのファイルを手動で閉じた場合（空のドキュメントのみが残る場合）、セッションはクリアされないため、次回起動時にそれらのファイルが再び復元されます。

- 単一ウィンドウでの使用を想定しています。すべてのウィンドウが同じ `session.txt` を共有するため、複数のウィンドウを同時に使用すると、セッションが互いに上書きされたり、タブが重複して復元されたりする可能性があります。

## ビルド

`gedit-devel` と `meson` が必要です:

```bash
# Fedora
sudo dnf install gedit-devel meson

# ビルド
meson setup builddir
meson compile -C builddir
```

## インストール

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

その後、gedit → 設定 → プラグインで「Session Restore」を有効にします。

## ライセンス

MIT
