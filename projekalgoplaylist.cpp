#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

// ============================================================
// FUNGSI BANTU UMUM
// ============================================================

// Fungsi jeda menggunakan loop kosong
void jedaDetik(int detik) {
    long iterasiPerDetik = 300000000L;
    long total = (long)detik * iterasiPerDetik;
    long i;
    for (i = 0; i < total; i++) {
        // loop kosong sebagai pengganti sleep
    }
}
// Ubah satu huruf menjadi huruf kecil
char hurufKecil(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}


// Cek apakah needle ada di dalam haystack (tidak peduli besar/kecil huruf)
// Kembalikan 1 jika ditemukan, 0 jika tidak
int mengandungKata(const char *haystack, const char *needle) {
    int panjangHaystack = strlen(haystack);
    int panjangNeedle   = strlen(needle);
    int i, j, cocok;
    for (i = 0; i <= panjangHaystack - panjangNeedle; i++) {
        cocok = 1;
        for (j = 0; j < panjangNeedle; j++) {
            if (hurufKecil(haystack[i + j]) != hurufKecil(needle[j])) {
                cocok = 0;
                break;
            }
        }
        if (cocok) return 1;
    }
    return 0;
}

// Bandingkan dua string tanpa peduli besar/kecil huruf
// Kembalikan negatif jika a < b, 0 jika sama, positif jika a > b
int bandingkanString(const char *a, const char *b) {
    while (*a && *b) {
        char ca = hurufKecil(*a);
        char cb = hurufKecil(*b);
        if (ca != cb) return ca - cb;
        a++;
        b++;
    }
    return hurufKecil(*a) - hurufKecil(*b);
}
