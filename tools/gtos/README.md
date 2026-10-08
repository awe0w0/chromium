# GTOS platform foundation

This local port adds the independent `target_os = "gtos"`, `is_gtos` and
`BUILDFLAG(IS_GTOS)` identities. GTOS does not set `is_linux`, `IS_LINUX`,
`OS_LINUX`, or `LIBC_GLIBC`. It currently exposes a small guest ABI rather than
a qualified POSIX runtime, so `is_posix` and `IS_POSIX` remain false. Portable
POSIX source reuse requires a reviewed guest implementation of its services.

The dedicated GTOS qualification toolchains compile with official Clang using
`i686-unknown-none-elf` / `x86_64-unknown-none-elf`. The GTOS compile config adds
`__GTOS__=1`. They do not use the Linux sysroot, host libc, or a Linux linker
driver. Shared-library filename metadata does not establish a dynamic loader;
the qualification toolchain has no `solink` tool.

The first executable is a real **platform qualification program**, not the
Chromium browser. It includes Chromium's actual platform header, statically
checks the independent OS identity, embeds a GTOS ELF note, and runs using the
already published i386 GTOS ABI1. It checks ABI version, writes the platform
marker, and exits. Its loader/runtime success cannot prove V8 or browser support.

For x64 the target produces an ELF64 relocatable object only. The x64 kernel's
supervisor boot/VM work does not yet expose a user ABI; this port invents none.

## Build the qualification

From this Chromium worktree with the real GN executable and official Clang:

```sh
gn gen out/gtos-platform-x86 --root="$PWD" \
  --dotfile="$PWD/tools/gtos/platform_identity.gn" \
  --args='target_os="gtos" target_cpu="x86" gtos_clang_bin="/absolute/pinned/clang/bin"' \
  --fail-on-unused-args
ninja -C out/gtos-platform-x86 platform_identity_probe
```

Repeat with `target_cpu="x64"` and another output directory for the object
qualification. GN writes `gtos-platform.json` with the actual target identity,
ABI scope and missing backends. Preserve host architecture separately from the
target architecture; a 64-bit build host does not force a 64-bit GTOS target.

`--dotfile` requires the explicit `--root` in this qualification command.
The build config remains Chromium's original `build/config/BUILDCONFIG.gn`;
the alternate dotfile limits the graph to this freestanding target.

## Runtime boundary

The complete Chromium root build deliberately reports the missing native
runtime. V8's native GTOS GN context likewise reports its missing backend.
V8's `V8_TARGET_OS_GTOS` is separate from the OS of host snapshot tools:
a Linux host may have `V8_OS_LINUX` while producing GTOS target data. It must
not set `V8_TARGET_OS_LINUX` for the GTOS target. Conflicting target definitions
are rejected. Native GTOS compilation with host libc headers is rejected.

Ozone is not enabled for GTOS before a real surface/input backend exists.
Selecting Ozone, a Linux/default sysroot, or a manual unqualified sysroot is
an explicit error in the qualification. The Linux headless raster FileSurface
is a useful reference for CPU drawing, not a native GTOS display backend.

Remaining work includes a qualified libc/C++ SDK, checked guest VM services,
threads/TLS/qualified FP state, process/IPC isolation, file/entropy/time services,
Ozone display/input, and actual networking/DNS/transport TLS. Reuse generic code
only after these services have real GTOS semantics and guest tests. No empty
success-returning VM/thread/network/display implementations are provided here.

The Chromium platform configuration is published in the
[GTOS fork](https://github.com/awe0w0/chromium/tree/gtos-platform-integration),
based on Chromium `04bca2584cddb39e134fc7c16bb2886e02863d0e`.
V8 is a separate pinned dependency; its matching platform patch and application
instructions are preserved in [patches/README.md](patches/README.md).
These changes have not been submitted upstream. This branch publishes the
platform foundation; full native V8 and Chromium remain incomplete.
