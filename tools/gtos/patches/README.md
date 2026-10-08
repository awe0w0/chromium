# V8 GTOS platform dependency patch

Chromium base: `04bca2584cddb39e134fc7c16bb2886e02863d0e`.
V8 base: `be042d4462bee463c9b785701b8c6ee4576ee0a3`.
Patch SHA-256: `f8679584282798a4fb5d40701895548457e94675f026ca111a36c728ef819f48`.

V8 is fetched separately by Chromium DEPS. Its gitlink and DEPS pin are unchanged;
the patch adds GTOS host/target identities and rejects unimplemented native runtime.
After fetching the pinned dependencies, confirm the V8 revision and apply from
the Chromium source root, using a clean V8 checkout:

```sh
git -C v8 rev-parse HEAD
git -C v8 apply --check ../tools/gtos/patches/v8-0001-gtos-platform-foundation.patch
git -C v8 apply ../tools/gtos/patches/v8-0001-gtos-platform-foundation.patch
```

Use the exact V8 base above. The patch does not provide a complete V8 runtime.
Additional native runtime work is outside this platform-configuration publication.
