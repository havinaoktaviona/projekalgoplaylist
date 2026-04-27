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
