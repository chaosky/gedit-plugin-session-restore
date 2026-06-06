# Plugin de restauration de session pour Gedit

**Lire dans d'autres langues :** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Español](README.es.md) · [Português](README.pt.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

Un plugin gedit qui mémorise vos fichiers ouverts et les restaure au prochain démarrage — comme la fonction « Reprendre là où vous vous êtes arrêté » de Chrome.

Écrit en C pour gedit 49+ qui [ne prend plus en charge les plugins Python](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## Fonctionnement

- À la fermeture de la fenêtre : enregistre les URI de tous les fichiers ouverts dans `~/.local/share/gedit/session.txt`
- À l'ouverture de la fenêtre : restaure les fichiers sous forme d'onglets (ignore les fichiers qui n'existent plus)

## Comportement connu

- Au démarrage, gedit crée toujours un onglet vide sans titre avant l'exécution des plugins. Une fois les fichiers de session restaurés, cet onglet vide est fermé automatiquement, vous pouvez donc le voir apparaître brièvement puis disparaître.

- Le fichier de session n'est mis à jour que lorsqu'au moins un fichier enregistré est ouvert. Si vous fermez manuellement tous les fichiers (ne laissant qu'un document vide), la session n'est pas effacée, ces fichiers seront donc restaurés à nouveau au prochain démarrage.

- Conçu pour une utilisation avec une seule fenêtre. Toutes les fenêtres partagent le même `session.txt`, donc l'utilisation simultanée de plusieurs fenêtres peut entraîner l'écrasement mutuel des sessions ou la restauration d'onglets en double.

- Sur certains environnements (par exemple GNOME Shell sous Wayland), l'activation de ce plugin peut empêcher le dash d'afficher le point indicateur d'exécution pour gedit. Cela est déclenché par la restauration des onglets et la fermeture de l'onglet vide initial au démarrage, ce qui peut rompre l'association fenêtre-application de GNOME Shell. Solution de contournement : copiez `/usr/share/applications/org.gnome.gedit.desktop` dans `~/.local/share/applications/` et ajoutez `StartupWMClass=gedit`.

## Compilation

Nécessite `gedit-devel` et `meson` :

```bash
# Fedora
sudo dnf install gedit-devel meson

# Compilation
meson setup builddir
meson compile -C builddir
```

## Installation

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Activez ensuite « Session Restore » dans gedit → Préférences → Greffons.

## Licence

MIT
