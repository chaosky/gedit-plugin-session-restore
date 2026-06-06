# Gedit 세션 복원 플러그인

**다른 언어로 읽기:** [English](../README.md) · [日本語](README.ja.md) · [中文](README.zh.md) · [Русский](README.ru.md) · [Français](README.fr.md) · [Español](README.es.md) · [Português](README.pt.md) · [Italiano](README.it.md)

열려 있는 파일을 기억하고 다음 실행 시 복원하는 gedit 플러그인입니다 — Chrome의 "중단한 위치에서 계속하기" 기능과 비슷합니다.

[Python 플러그인을 더 이상 지원하지 않는](https://gedit-text-editor.org/blog/2025-04-30-no-more-pythons-in-gedit.html) gedit 49+ 용으로 C로 작성되었습니다.

## 작동 방식

- 창을 닫을 때: 열려 있는 모든 파일의 URI를 `~/.local/share/gedit/session.txt`에 저장합니다
- 창을 열 때: 파일을 탭으로 복원합니다 (더 이상 존재하지 않는 파일은 건너뜁니다)

## 알려진 동작

- 시작 시 gedit는 플러그인이 실행되기 전에 항상 빈 제목 없는 탭을 만듭니다. 세션 파일이 복원된 후 이 빈 탭은 자동으로 닫히므로, 잠깐 나타났다가 사라지는 것을 볼 수 있습니다.

- 세션 파일은 저장된 파일이 하나 이상 열려 있을 때만 업데이트됩니다. 모든 파일을 수동으로 닫으면(빈 문서만 남는 경우) 세션이 지워지지 않으므로 다음 실행 시 해당 파일이 다시 복원됩니다.

- 단일 창 사용을 위해 설계되었습니다. 모든 창이 동일한 `session.txt`를 공유하므로 여러 창을 동시에 사용하면 세션이 서로 덮어쓰이거나 중복된 탭이 복원될 수 있습니다.

## 빌드

`gedit-devel`과 `meson`이 필요합니다:

```bash
# Fedora
sudo dnf install gedit-devel meson

# 빌드
meson setup builddir
meson compile -C builddir
```

## 설치

```bash
mkdir -p ~/.local/share/gedit/plugins/session-restore
cp builddir/libsessionrestore.so sessionrestore.plugin ~/.local/share/gedit/plugins/session-restore/
```

그런 다음 gedit → 기본 설정 → 플러그인에서 "Session Restore"를 활성화합니다.

## 라이선스

MIT
