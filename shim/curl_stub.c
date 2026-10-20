/*
 * libcurl stub for MIUI vendor mnld (GPS daemon). Android 10 / Halium 10 no longer
 * ship libcurl; mnld only uses it to download EPO/QEPO assistance data. Every
 * transfer fails cleanly (CURLE_COULDNT_CONNECT), GPS works without the download.
 * Build: arm-linux-androideabi-gcc -std=gnu99 -ffreestanding -O2 -fPIC -shared -nostdlib
 *        -Wl,-soname,libcurl.so -o libcurl.so curl_stub.c
 */
int curl_global_init(long flags) { (void)flags; return 0; }
void *curl_easy_init(void) { return 0; }
int curl_easy_setopt(void *h, int opt, ...) { (void)h; (void)opt; return 0; }
int curl_easy_perform(void *h) { (void)h; return 7; /* CURLE_COULDNT_CONNECT */ }
void curl_easy_cleanup(void *h) { (void)h; }
