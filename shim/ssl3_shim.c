/*
 * libsl3: SSLv3_client_method/SSLv3_server_method for MIUI vendor mtk_agpsd.
 * BoringSSL in Android 10 dropped them; forward to TLS_*_method (SSLv3 itself is
 * disabled anyway). mtk_agpsd's NEEDED libssl.so is patched to libsl3.so, which
 * needs the real libssl.so.
 * Build: arm-linux-androideabi-gcc -std=gnu99 -ffreestanding -O2 -fPIC -shared -nostdlib
 *        -Wl,-soname,libsl3.so -o libsl3.so ssl3_shim.c libssl.so  (from /system/lib)
 */
__asm__(".text\n.arm\n"
        ".globl SSLv3_client_method\n.type SSLv3_client_method,%function\nSSLv3_client_method:\n\tb TLS_client_method(PLT)\n"
        ".globl SSLv3_server_method\n.type SSLv3_server_method,%function\nSSLv3_server_method:\n\tb TLS_server_method(PLT)\n");
