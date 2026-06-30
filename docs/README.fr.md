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

## Dépannage

### `libgedit-49.so: cannot open shared object file: No such file or directory`

Si le plugin ne se charge plus après une mise à niveau de gedit et que vous voyez un message comme :

```
Failed to load module 'sessionrestore': libgedit-49.so: cannot open shared object file: No such file or directory
Error loading plugin 'sessionrestore'
```

c'est **attendu après une mise à niveau majeure de gedit**. Le `.so` compilé est lié au `libgedit-<N>.so` exact installé au moment de la compilation (par exemple `libgedit-49.so` pour gedit 49). Lorsque gedit est mis à niveau (par exemple vers 50), l'ancienne bibliothèque est supprimée, donc le plugin ne la trouve plus.

**Solution : recompilez simplement pour le nouveau gedit et réinstallez.**

```bash
rm -rf builddir
meson setup builddir
meson compile -C builddir
cp builddir/libsessionrestore.so ~/.local/share/gedit/plugins/session-restore/
```

Redémarrez ensuite gedit. Vous devez le faire une fois après chaque mise à niveau majeure de gedit.

## Licence

MIT
