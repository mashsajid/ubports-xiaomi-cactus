/*
 * libbsh: provides the Pie-only android::base::StartsWith(const std::string&,
 * const char*) overload for MIUI vendor libnvram.so / libsysenv.so (used by
 * ccci_mdinit, ccci_fsd, nvram_daemon, nvram HAL ...). Those libraries' NEEDED
 * libbase.so entry is patched to libbsh.so, which itself needs libbase.so.
 * Build: arm-linux-androideabi-gcc -std=gnu99 -ffreestanding -O2 -fPIC -shared -nostdlib -Wl,-soname,libbsh.so
 *        -o libbsh.so base_shim.c libbase.so   (libbase.so from the phone's /system/lib)
 */
typedef unsigned int uint32_t;

int _ZN7android4base10StartsWithERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEPKc(
    const void *str, const char *prefix)
{
    const unsigned char *b = (const unsigned char *)str;
    const char *data;
    uint32_t size, n = 0;
    if (b[0] & 1) {
        size = ((const uint32_t *)str)[1];
        data = ((const char *const *)str)[2];
    } else {
        size = b[0] >> 1;
        data = (const char *)b + 1;
    }
    while (prefix[n]) n++;
    if (n > size) return 0;
    for (uint32_t i = 0; i < n; i++)
        if (data[i] != prefix[i]) return 0;
    return 1;
}
