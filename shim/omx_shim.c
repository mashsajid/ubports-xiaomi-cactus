/*
 * libomxsh: lets the MIUI (Android 9) android.hardware.media.omx@1.0-service run
 * against Halium 10's libstagefright_omx / libstagefright_xmlparser.
 *  - MediaCodecsXmlParser::defaultSearchDirs (Pie data symbol) is provided here.
 *  - OmxStore(owner, searchDirs[], mainXml, perfXml, profPath) (Pie) is adapted to
 *    OmxStore(sp<IOmx> const&, owner, vector<string> const& dirs,
 *             vector<string> const& xmlFiles, profPath) (Android 10).
 *    The class layout is identical in both releases (AOSP OmxStore.h).
 *    std::string / std::vector are built in libc++'s 32-bit little-endian layout;
 *    the callee only reads them during construction.
 * The service's NEEDED libstagefright_xmlparser.so is patched to libomxsh.so,
 * which needs libstagefright_xmlparser.so and libstagefright_omx.so.
 * Build: arm-linux-androideabi-gcc -std=gnu99 -ffreestanding -O2 -fPIC -shared -nostdlib
 *        -Wl,-soname,libomxsh.so -o libomxsh.so omx_shim.c
 *        libstagefright_omx.so libstagefright_xmlparser.so   (from /system/lib)
 */
typedef unsigned int u32;

const char *const pie_default_search_dirs[] __asm__("_ZN7android20MediaCodecsXmlParser17defaultSearchDirsE") =
    { "/odm/etc", "/vendor/etc", "/etc", 0 };

struct lcxx_string { u32 w[3]; };              /* sizeof(std::string) == 12 */
struct lcxx_vector { void *begin, *end, *cap; };

#define MAX_STR 8

static void make_string(struct lcxx_string *s, const char *c)
{
    u32 n = 0;
    while (c[n]) n++;
    if (n <= 10) {                             /* short: size<<1 in byte 0, data at +1 */
        unsigned char *b = (unsigned char *)s;
        b[0] = (unsigned char)(n << 1);
        for (u32 i = 0; i < n; i++) b[1 + i] = (unsigned char)c[i];
        b[1 + n] = 0;
        for (u32 i = n + 2; i < 12; i++) b[i] = 0;
    } else {                                   /* long: {cap|1, size, data} */
        s->w[0] = ((n + 16) & ~15u) | 1;
        s->w[1] = n;
        s->w[2] = (u32)c;
    }
}

void *q_ctor(void *self, const void *sp_omx, const char *owner,
             const struct lcxx_vector *dirs, const struct lcxx_vector *xmls,
             const char *prof)
    __asm__("_ZN7android8hardware5media3omx4V1_014implementation8OmxStoreC1ERKNS_2spINS3_4IOmxEEEPKcRKNSt3__16vectorINSD_12basic_stringIcNSD_11char_traitsIcEENSD_9allocatorIcEEEENSI_ISK_EEEESO_SC_");

void *pie_ctor(void *self, const char *owner, const char *const *search_dirs,
               const char *main_xml, const char *perf_xml, const char *prof)
    __asm__("_ZN7android8hardware5media3omx4V1_014implementation8OmxStoreC1EPKcPKS7_S7_S7_S7_");

void *pie_ctor(void *self, const char *owner, const char *const *search_dirs,
               const char *main_xml, const char *perf_xml, const char *prof)
{
    struct lcxx_string dir_s[MAX_STR], xml_s[2];
    struct lcxx_vector dirs, xmls;
    void *null_sp = 0;
    u32 n = 0;

    if (!search_dirs) search_dirs = pie_default_search_dirs;
    while (n < MAX_STR && search_dirs[n]) { make_string(&dir_s[n], search_dirs[n]); n++; }
    dirs.begin = dir_s; dirs.end = dir_s + n; dirs.cap = dir_s + n;

    make_string(&xml_s[0], main_xml ? main_xml : "media_codecs.xml");
    make_string(&xml_s[1], perf_xml ? perf_xml : "media_codecs_performance.xml");
    xmls.begin = xml_s; xmls.end = xml_s + 2; xmls.cap = xml_s + 2;

    return q_ctor(self, &null_sp, owner ? owner : "default", &dirs, &xmls, prof);
}
