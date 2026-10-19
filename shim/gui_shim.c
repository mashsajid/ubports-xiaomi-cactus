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
