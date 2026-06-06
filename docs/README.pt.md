# Plugin de restauração de sessão para o Gedit

**Ler em outros idiomas:** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Español](README.es.md) · [Italiano](README.it.md) · [한국어](README.ko.md)

Um plugin do gedit que memoriza os arquivos abertos e os restaura na próxima inicialização — como o recurso «Continuar de onde parou» do Chrome.

Escrito em C para o gedit 49+, que [não oferece mais suporte a plugins em Python](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html).

## Como funciona

- Ao fechar a janela: salva os URIs de todos os arquivos abertos em `~/.local/share/gedit/session.txt`
- Ao abrir a janela: restaura os arquivos como abas (ignora os arquivos que não existem mais)

## Comportamento conhecido

- Na inicialização, o gedit sempre cria uma aba vazia sem título antes de os plugins serem executados. Após a restauração dos arquivos da sessão, essa aba vazia é fechada automaticamente, então você pode vê-la aparecer brevemente e desaparecer.

- O arquivo de sessão só é atualizado quando pelo menos um arquivo salvo está aberto. Se você fechar manualmente todos os arquivos (deixando apenas um documento vazio), a sessão não é limpa, portanto esses arquivos serão restaurados novamente na próxima inicialização.

- Projetado para uso com uma única janela. Todas as janelas compartilham o mesmo `session.txt`, então usar várias janelas simultaneamente pode fazer com que as sessões se sobrescrevam ou que abas duplicadas sejam restauradas.

- Em alguns ambientes (por exemplo, GNOME Shell no Wayland), ativar este plugin pode fazer com que o dash deixe de exibir o ponto indicador de execução do gedit. Isso é causado pelo plugin restaurar as abas e fechar a aba vazia inicial na inicialização, o que pode quebrar a associação janela-aplicativo do GNOME Shell. Solução alternativa: copie `/usr/share/applications/org.gnome.gedit.desktop` para `~/.local/share/applications/` e adicione `StartupWMClass=gedit`.

## Compilação

Requer `gedit-devel` e `meson`:

```bash
# Fedora
sudo dnf install gedit-devel meson

# Compilação
meson setup builddir
meson compile -C builddir
```

## Instalação

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

Em seguida, ative «Session Restore» em gedit → Preferências → Plugins.

## Licença

MIT
