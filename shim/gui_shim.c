/*
 * libux: bridges MIUI (Android 9) vendor libgui.so to Halium 10 (Android 10)
 * system libraries. Loaded via vendor libgui's NEEDED entry (libui.so ->
 * libux.so); libux itself depends on the real libui.so.
 *
 * Android 10 added optional outBytesPerPixel/outBytesPerStride arguments to
 * GraphicBuffer::lock/lockAsync; the Pie signatures forward with NULLs.
 * The remaining entries are debug/VR-only and are stubbed.
 */
#include <stddef.h>
#include <stdint.h>

typedef int status_t;

/* Android 10 symbols from libui.so */
status_t _ZN7android13GraphicBuffer4lockEjPPvPiS3_(void *self, uint32_t usage, void **vaddr,
                                                    int32_t *bpp, int32_t *bps);
status_t _ZN7android13GraphicBuffer4lockEjRKNS_4RectEPPvPiS6_(void *self, uint32_t usage,
                                                              const void *rect, void **vaddr,
                                                              int32_t *bpp, int32_t *bps);
status_t _ZN7android13GraphicBuffer9lockAsyncEjRKNS_4RectEPPviPiS6_(void *self, uint32_t usage,
                                                                    const void *rect, void **vaddr,
                                                                    int fence, int32_t *bpp,
                                                                    int32_t *bps);

/* GraphicBuffer::lock(uint32_t, void**) */
status_t _ZN7android13GraphicBuffer4lockEjPPv(void *self, uint32_t usage, void **vaddr)
{
    return _ZN7android13GraphicBuffer4lockEjPPvPiS3_(self, usage, vaddr, NULL, NULL);
}

/* GraphicBuffer::lock(uint32_t, const Rect&, void**) */
status_t _ZN7android13GraphicBuffer4lockEjRKNS_4RectEPPv(void *self, uint32_t usage,
                                                         const void *rect, void **vaddr)
{
    return _ZN7android13GraphicBuffer4lockEjRKNS_4RectEPPvPiS6_(self, usage, rect, vaddr,
                                                                NULL, NULL);
}

/* GraphicBuffer::lockAsync(uint32_t, const Rect&, void**, int) */
status_t _ZN7android13GraphicBuffer9lockAsyncEjRKNS_4RectEPPvi(void *self, uint32_t usage,
                                                               const void *rect, void **vaddr,
                                                               int fence)
{
    return _ZN7android13GraphicBuffer9lockAsyncEjRKNS_4RectEPPviPiS6_(self, usage, rect, vaddr,
                                                                      fence, NULL, NULL);
}

/* dvr::BufferProducer::PostAsync(...) - VR path, not used on this device */
int _ZN7android3dvr14BufferProducer9PostAsyncEPK23DvrNativeBufferMetadataRKNS_3pdx10FileHandleILNS5_14FileHandleModeE0EEE(
    void *self, const void *meta, const void *fence)
{
    (void)self; (void)meta; (void)fence;
    return -38; /* -ENOSYS */
}

/* hardware::details::wouldLogInfo() */
int _ZN7android8hardware7details12wouldLogInfoEv(void) { return 0; }

/* hardware::details::logAlwaysInfo(const std::string&) */
void _ZN7android8hardware7details13logAlwaysInfoERKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEE(
    const void *msg)
{
    (void)msg;
}

/* Region::dump(String8&, const char*, uint32_t) const */
void _ZNK7android6Region4dumpERNS_7String8EPKcj(const void *self, void *out, const char *what,
                                               uint32_t flags)
{
    (void)self; (void)out; (void)what; (void)flags;
}

/*
 * __system_properties_init: Pie vendor libs (libpdx_default_transport.so)
 * bind it at version LIBC_PLATFORM, which Android 10 libc no longer
 * provides (it moved to LIBC_Q). libc has already initialised the property
 * area by the time any library is loaded, so this is a no-op.
 */
int __system_properties_init(void) { return 0; }

/*
 * android::base::StartsWith(const std::string&, const char*)
 * Android 10 libbase only provides the std::string_view overloads.
 * libc++ (32-bit) std::string: long mode when bit 0 of the first byte is
 * set -> {cap, size, data}; short mode -> size = byte0 >> 1, data at +1.
 */
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

/*
 * DisplayEventReceiver(VsyncSource, ConfigChanged): Android 10 added the second
 * argument; Halium's libandroidfw (pulled in by libubuntu_application_api for the
 * location service) needs it, but the Pie vendor libgui only has the one-argument
 * constructor. The object layout is unchanged, so drop the extra argument.
 */
__asm__(".text\n.arm\n"
        ".globl _ZN7android20DisplayEventReceiverC1ENS_16ISurfaceComposer11VsyncSourceENS1_13ConfigChangedE\n"
        ".type _ZN7android20DisplayEventReceiverC1ENS_16ISurfaceComposer11VsyncSourceENS1_13ConfigChangedE,%function\n"
        "_ZN7android20DisplayEventReceiverC1ENS_16ISurfaceComposer11VsyncSourceENS1_13ConfigChangedE:\n"
        "\tb _ZN7android20DisplayEventReceiverC1ENS_16ISurfaceComposer11VsyncSourceE(PLT)\n");
