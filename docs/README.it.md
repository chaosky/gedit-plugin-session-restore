# Plugin di ripristino sessione per Gedit

**Leggi in altre lingue:** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Español](README.es.md) · [Português](README.pt.md) · [한국어](README.ko.md)

Un plugin per gedit che ricorda i file aperti e li ripristina al successivo avvio — come la funzione «Continua da dove avevi interrotto» di Chrome.

Scritto in C per gedit 49+, che [non supporta più i plugin Python](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## Come funziona

- Alla chiusura della finestra: salva gli URI di tutti i file aperti in `~/.local/share/gedit/session.txt`
- All'apertura della finestra: ripristina i file come schede (salta i file che non esistono più)

## Comportamento noto

- All'avvio, gedit crea sempre una scheda vuota senza titolo prima dell'esecuzione dei plugin. Dopo il ripristino dei file della sessione, questa scheda vuota viene chiusa automaticamente, quindi potresti vederla apparire brevemente e poi scomparire.

- Il file di sessione viene aggiornato solo quando è aperto almeno un file salvato. Se chiudi manualmente tutti i file (lasciando solo un documento vuoto), la sessione non viene cancellata, quindi quei file verranno ripristinati di nuovo al successivo avvio.

- Progettato per l'uso con una singola finestra. Tutte le finestre condividono lo stesso `session.txt`, quindi l'uso di più finestre contemporaneamente può causare la sovrascrittura reciproca delle sessioni o il ripristino di schede duplicate.

## Compilazione

Richiede `gedit-devel` e `meson`:

```bash
# Fedora
sudo dnf install gedit-devel meson

# Compilazione
meson setup builddir
meson compile -C builddir
```

## Installazione

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Quindi abilita «Session Restore» in gedit → Preferenze → Plugin.

## Licenza

MIT
