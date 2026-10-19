/*
 * libunwstk_shim: bridges MIUI (Android 9) vendor libladder.so to the
 * Android 10 libunwindstack shipped by Halium 10.
 *
 * Android 10 made unwindstack::Maps::~Maps() inline, so the exported
 * base-object destructor _ZN11unwindstack4MapsD2Ev that the Pie blob links
 * against no longer exists, and every vendor library that pulls in
 * libladder.so (via libmtkutils.so -> GPU/EGL stack) fails to load.
 *
 * libladder only uses Maps on its debug back-trace path; a no-op destructor
 * leaks the parsed map list there, which is acceptable.
 */
void _ZN11unwindstack4MapsD2Ev(void *self) { (void)self; }
