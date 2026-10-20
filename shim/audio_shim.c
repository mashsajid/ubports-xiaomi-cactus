/*
 * libaushim: bridges the MIUI (Android 9) audio HAL to Halium 10 system libs.
 *  - Pie's audio.common@4.0-util exported HidlUtils in common::V4_0; Android 10
 *    moved it to common::V4_0::implementation (same arguments). The Pie names
 *    tail-branch to the Q ones, so any calling convention (incl. sret) passes through.
 *  - Pie's libmedia_helper had TypeConverter<FormatTraits>::mTable; Q renamed the
 *    traits to VectorTraits<audio_format_t>. Same {literal, value} entries, so the
 *    Pie table is filled from the Q one at load time (we are a NEEDED dependency
 *    of audio.primary, so this runs before its own constructors).
 * Loaded via patched NEEDED entries (libmedia_helper.so / android.hardware.audio.
 * common@4.0-util.so -> libaushim.so); libaushim itself needs both real libraries.
 * Build: arm-linux-androideabi-gcc -std=gnu99 -ffreestanding -O2 -fPIC -shared -nostdlib
 *        -Wl,-soname,libaushim.so -o libaushim.so audio_shim.c
 *        libmedia_helper.so android.hardware.audio.common@4.0-util.so  (from /system/lib)
 */
#define FWD(pie, q) \
    __asm__(".text\n.arm\n.globl " #pie "\n.type " #pie ",%function\n" #pie ":\n\tb " #q "(PLT)\n");

#define HU_PIE(n) _ZN7android8hardware5audio6common4V4_09HidlUtils##n
#define HU_Q(n)   _ZN7android8hardware5audio6common4V4_014implementation9HidlUtils##n
#define HU(n) FWD(_ZN7android8hardware5audio6common4V4_09HidlUtils##n, _ZN7android8hardware5audio6common4V4_014implementation9HidlUtils##n)

HU(11uuidFromHalERK12audio_uuid_sPNS3_4UuidE)
HU(9uuidToHalERKNS3_4UuidEP12audio_uuid_s)
HU(14audioPortToHalERKNS3_9AudioPortEP10audio_port)
HU(16audioPortFromHalERK10audio_portPNS3_9AudioPortE)
HU(16audioConfigToHalERKNS3_11AudioConfigEP12audio_config)
HU(18audioConfigFromHalERK12audio_configPNS3_11AudioConfigE)
HU(20audioPortConfigToHalERKNS3_15AudioPortConfigEP17audio_port_config)
HU(21audioPortConfigsToHalERKNS0_8hidl_vecINS3_15AudioPortConfigEEE)

struct def { const char *literal; unsigned int value; };
#define TABLE_MAX 256
extern const struct def _ZN7android13TypeConverterINS_12VectorTraitsI14audio_format_tEEE6mTableE[];
struct def _ZN7android13TypeConverterINS_12FormatTraitsEE6mTableE[TABLE_MAX];

__attribute__((constructor)) static void aushim_init(void)
{
    const struct def *q = _ZN7android13TypeConverterINS_12VectorTraitsI14audio_format_tEEE6mTableE;
    int i;
    for (i = 0; i < TABLE_MAX - 1 && q[i].literal; i++)
        _ZN7android13TypeConverterINS_12FormatTraitsEE6mTableE[i] = q[i];
    _ZN7android13TypeConverterINS_12FormatTraitsEE6mTableE[i].literal = 0;
}
