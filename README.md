# Ubuntu Touch 20.04 (focal) for Xiaomi Redmi 6A (cactus)

Halium 10 generic-system port of Ubuntu Touch 20.04 to the Xiaomi Redmi 6A
(codename `cactus`, MediaTek Helio A22 / MT6765). The port is 32-bit:
an `arm` kernel and an armhf focal rootfs on top of the stock MIUI 11 (Android 9)
vendor partition.

| | |
|---|---|
| Device | Xiaomi Redmi 6A (`cactus`) |
| SoC | MediaTek MT6765 |
| Architecture | arm (armhf userspace) |
| Halium | 10 |
| Ubuntu Touch | 20.04 (focal) |
| Kernel | [mashsajid/kernel_xiaomi_cactus](https://github.com/mashsajid/kernel_xiaomi_cactus), branch `ut-focal-arm32`, `certus_halium_defconfig` |

> **Note:** `overlay/` contains proprietary vendor binaries (MIUI / Android
> libraries and kernel modules) needed to make the Pie vendor work with
> Halium 10. Check their licensing before redistributing this repository.

## Status

### Working
- UI and display, touch
- Lock and wake (screen comes back after suspend)
- Deep sleep (PM autosleep via repowerd), power-key wake
- Wi-Fi
- Battery percentage
- LTE (tested with a SIM in slot 2), calls with audio, SMS, mobile data
- Sound (PulseAudio droid sinks)
- Terminal
- Interface scale (GridUnit 17 for the 720x1440 5.45" panel)

### Known issues
- Camera preview shows a green glitch
- Front camera does not work
- Red/blue channels may be swapped
- Untested: Bluetooth pairing, GPS fix, video playback and recording
- Lens-shading correction is disabled (no camera NVRAM calibration data)
- SELinux is permissive
- Bring-up debugging is still enabled: key-only root SSH login, journald debug
  syncing, and the CMDQ instruction checker in log-only mode

## Requirements
- Unlocked bootloader
- Stock MIUI 11 (Android 9) vendor partition
- LineageOS 17.1 (certus, 32-bit) as the base the Halium 10 system is built on

## Repository layout
- `deviceinfo`: device configuration for the halium-generic-adaptation-build-tools
- `build.sh`: entry point; clones the build tools into `build/` and runs them
- `overlay/`: files baked into the rootfs (systemd units, Halium vendor-lib
  overlay, connectivity modules, deviceinfo overrides)
- `shim/`: sources of the small compatibility libraries in the overlay
  (`libux`, `libaushim`, `libomxsh`, `libsl3`, `libunwstk_shim`, curl stub, ...);
  `shim/conn-modules-SOURCES.txt` describes where the MTK connectivity modules
  were rebuilt from

## Building

The build runs in an Ubuntu 20.04 container:

```dockerfile
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      bc bison build-essential ca-certificates cpio curl fakeroot flex git kmod \
      libelf-dev libssl-dev libtinfo5 lz4 python2 python3 sudo unzip wget xz-utils \
      img2simg jq zstd dos2unix android-tools-mkbootimg \
    && ln -sf /usr/bin/python2 /usr/bin/python
```

```bash
docker build -t ut-build:20.04 .
docker run --rm -it -u "$(id -u):$(id -g)" -v "$PWD:$PWD" -w "$PWD" ut-build:20.04 ./build.sh -b workdir
```

`build.sh` fetches the kernel named in `deviceinfo`
(`deviceinfo_kernel_source` / `deviceinfo_kernel_source_branch`), builds it for
`arch/arm` with `certus_halium_defconfig` using the GCC 4.9
`arm-linux-androideabi-` toolchain (`KCFLAGS=-Wno-maybe-uninitialized`), and
produces `boot.img` and the device tarball in `out/`.

The system image is then created from the Ubuntu Touch rootfs with
`build/system-image-from-ota.sh` (run as root in the container, since it packs
root-owned files).

## Flashing

Use fastboot. `adb push` from TWRP drops the USB connection on this phone.
The bootloader's max-download-size is `0x8000000`, so sparse-split large images
with `-S 100M`:

```bash
fastboot flash boot out/boot.img
```

```bash
fastboot -S 100M flash system out/system.img
```

```bash
fastboot reboot
```

## Credits
- [UBports](https://ubports.com) and [Halium](https://halium.org), and the
  halium-generic-adaptation-build-tools
- [r0ddty/kernel_xiaomi_cactus](https://github.com/r0ddty/kernel_xiaomi_cactus): the kernel tree this port's kernel is based on
- [Veynamer/ubports-xiaomi-cactus](https://gitlab.com/Veynamer/ubports-xiaomi-cactus): the original device repository
- [LineageOS](https://lineageos.org)
