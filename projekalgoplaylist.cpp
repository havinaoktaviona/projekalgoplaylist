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

// ================
// STRUKTUR DATA
// ================

// Node Doubly Linked List untuk menyimpan data lagu
typedef struct Lagu {
    char judul[50];
    char penyanyi[50];
    int durasi;          // dalam detik
    struct Lagu *next;
    struct Lagu *prev;
} Lagu;

// Struktur data user untuk login
typedef struct {
    char username[30];
    char password[30];
} User;
// Head dan tail playlist (Doubly Linked List)
Lagu *head = NULL;
Lagu *tail = NULL;

// ============================================================
// FUNGSI UTILITAS TAMPILAN
// ============================================================

void clearScreen() {
    system("cls");
}

void pauseProgram() {
    printf("\nTekan Enter untuk melanjutkan...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void printGaris() {
    printf("============================================================\n");
}

void printJudul(const char *judul) {
    printGaris();
    printf("   %s\n", judul);
    printGaris();
}

int durasiKeDetik(int menit, int detik) {
    return menit * 60 + detik;
}

void detikKeMenit(int totalDetik, int *menit, int *detik) {
    *menit = totalDetik / 60;
    *detik = totalDetik % 60;
}

// Cetak tabel dari array pointer lagu
void cetakDariArray(Lagu **arr, int n) {
    int no, totalDetik = 0;
    printf("  %-4s %-30s %-25s %s\n", "No", "Judul", "Penyanyi", "Durasi");
    printf("  ----  ------------------------------  -------------------------  --------\n");
    for (no = 0; no < n; no++) {
        int m, s;
        detikKeMenit(arr[no]->durasi, &m, &s);
        printf("  %-4d %-30s %-25s %02d:%02d\n",
               no + 1, arr[no]->judul, arr[no]->penyanyi, m, s);
        totalDetik += arr[no]->durasi;
    }
    int tm, ts;
    detikKeMenit(totalDetik, &tm, &ts);
    printf("  ----\n");
    printf("  Total lagu: %d | Total durasi: %02d:%02d\n", n, tm, ts);
}

// ===================
// SISTEM LOGIN
// ===================

#define FILE_USER "users.dat"

void simpanUser(User u) {
    FILE *f = fopen(FILE_USER, "ab");
    if (f) {
        fwrite(&u, sizeof(User), 1, f);
        fclose(f);
    }
}

int usernameAda(const char *username) {
    FILE *f = fopen(FILE_USER, "rb");
    if (!f) return 0;
    User u;
    while (fread(&u, sizeof(User), 1, f)) {
        if (strcmp(u.username, username) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int verifikasiLogin(const char *username, const char *password) {
    FILE *f = fopen(FILE_USER, "rb");
    if (!f) return 0;
    User u;
    while (fread(&u, sizeof(User), 1, f)) {
        if (strcmp(u.username, username) == 0 &&
            strcmp(u.password, password) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void registrasi() {
    clearScreen();
    printJudul("REGISTRASI AKUN BARU");

    User u;
    printf("Masukkan username baru : ");
    scanf("%29s", u.username);

    if (usernameAda(u.username)) {
        printf("\n[!] Username sudah digunakan. Coba username lain.\n");
        pauseProgram();
        return;
    }

    printf("Masukkan password      : ");
    scanf("%29s", u.password);

    simpanUser(u);
    printf("\n[+] Akun berhasil dibuat! Silakan login.\n");
    pauseProgram();
}

// FIX 1: Login berhasil langsung masuk program, tidak perlu tekan Enter
// Kembalikan 1 jika berhasil login
int login(char *usernameAktif) {
    #define MAKS_PERCOBAAN 3
    #define JEDA_DETIK     30

    int percobaan = 0;

    while (1) {
        clearScreen();
        printJudul("LOGIN");

        if (percobaan > 0) {
            printf("  [!] Username/Password salah. Sisa percobaan: %d\n\n",
                   MAKS_PERCOBAAN - percobaan);
        }

        char username[30], password[30];
        printf("Username : ");
        scanf("%29s", username);
        printf("Password : ");
        scanf("%29s", password);

        if (verifikasiLogin(username, password)) {
            strcpy(usernameAktif, username);
            // FIX 1: Tidak ada pauseProgram(), langsung lanjut setelah jeda singkat
            printf("\n[+] Login berhasil! Selamat datang, %s!\n", username);
            jedaDetik(1);
            return 1;
        }

        percobaan++;

        if (percobaan >= MAKS_PERCOBAAN) {
            printf("\n[!] Percobaan login habis!\n");
            printf("    Akun dikunci sementara. Harap tunggu...\n\n");

            int i;
            for (i = JEDA_DETIK; i > 0; i--) {
                printf("\r    Coba lagi dalam: %2d detik   ", i);
                fflush(stdout);
                jedaDetik(1);
            }
            printf("\r    Silakan coba login kembali.     \n");
            pauseProgram();
            percobaan = 0;
        }
    }
}

// Tampilan awal: login atau registrasi
// Kembalikan 1 jika login berhasil, 0 jika user memilih keluar
int menuAwal(char *usernameAktif) {
    int pilihan;
    while (1) {
        clearScreen();
        printJudul("PENGELOLA PLAYLIST MUSIK");
        printf("  1. Login\n");
        printf("  2. Registrasi Akun Baru\n");
        printf("  0. Keluar\n");
        printGaris();
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                if (login(usernameAktif)) return 1;
                break;
            case 2:
                registrasi();
                break;
            case 0:
                printf("Sampai jumpa!\n");
                return 0;
            default:
                printf("[!] Pilihan tidak valid.\n");
                pauseProgram();
        }
    }
}

// ============================================================
// MANAJEMEN LINKED LIST (PLAYLIST)
// ============================================================

Lagu *buatNode(const char *judul, const char *penyanyi, int durasi) {
    Lagu *baru = (Lagu *)malloc(sizeof(Lagu));
    if (!baru) {
        printf("[!] Gagal alokasi memori!\n");
        return NULL;
    }
    strncpy(baru->judul, judul, 49);
    strncpy(baru->penyanyi, penyanyi, 49);
    baru->durasi = durasi;
    baru->next = NULL;
    baru->prev = NULL;
    return baru;
}
// Tambah lagu ke akhir list tanpa cetak pesan (dipakai saat load file)
void tambahLaguDiam(const char *judul, const char *penyanyi, int durasi) {
    Lagu *baru = buatNode(judul, penyanyi, durasi);
    if (!baru) return;
    if (!head) {
        head = tail = baru;
    } else {
        baru->prev = tail;
        tail->next = baru;
        tail = baru;
    }
}

// Tambah lagu ke akhir list dengan cetak konfirmasi
void tambahLagu(const char *judul, const char *penyanyi, int durasi) {
    tambahLaguDiam(judul, penyanyi, durasi);
    printf("  [+] \"%s\" berhasil ditambahkan!\n", judul);
}

void hapusLagu(const char *judul) {
    Lagu *curr = head;
    while (curr) {
        if (bandingkanString(curr->judul, judul) == 0) {
            if (curr->prev) curr->prev->next = curr->next;
            else            head = curr->next;
            if (curr->next) curr->next->prev = curr->prev;
            else            tail = curr->prev;
            free(curr); // Bebaskan memori (free memory)
            printf("[+] Lagu \"%s\" berhasil dihapus!\n", judul);
            return;
        }
        curr = curr->next;
    }
    printf("[!] Lagu \"%s\" tidak ditemukan.\n", judul);
}

// FIX 4: Tampilkan playlist asli tanpa pengurutan apapun
void tampilkanPlaylist() {
    if (!head) {
        printf("  [i] Playlist kosong.\n");
        return;
    }

    int no = 1;
    int totalDetik = 0;
    Lagu *curr = head;

    printf("  %-4s %-30s %-25s %s\n", "No", "Judul", "Penyanyi", "Durasi");
    printf("  ----  ------------------------------  -------------------------  --------\n");

    while (curr) {
        int m, s;
        detikKeMenit(curr->durasi, &m, &s);
        printf("  %-4d %-30s %-25s %02d:%02d\n",
               no++, curr->judul, curr->penyanyi, m, s);
        totalDetik += curr->durasi;
        curr = curr->next;
    }

    int tm, ts;
    detikKeMenit(totalDetik, &tm, &ts);
    printf("  ----\n");
    printf("  Total lagu: %d | Total durasi: %02d:%02d\n", no - 1, tm, ts);
}

// ============================================================
// FUNGSI BANTU ARRAY (dipakai oleh Searching dan Sorting)
// ============================================================

// Hitung jumlah node dalam linked list
int hitungNode() {
    int n = 0;
    Lagu *curr = head;
    while (curr) { n++; curr = curr->next; }
    return n;
}

// Salin alamat node linked list ke dalam array pointer
void salinKeArray(Lagu **arr, int n) {
    Lagu *curr = head;
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = curr;
        curr = curr->next;
    }
}


// ==================
// SEARCHING
// ==================

// --- Sequential Search berdasarkan Judul ---
// Menelusuri linked list dari awal sampai akhir satu per satu
void sequentialSearchJudul(const char *keyword) {
    Lagu *curr = head;
    int ditemukan = 0;
    int no = 1;

    printf("  [Sequential Search] Mencari judul yang mengandung \"%s\":\n\n", keyword);
    printf("  %-4s %-30s %-25s %s\n", "No", "Judul", "Penyanyi", "Durasi");
    printf("  ----  ------------------------------  -------------------------  --------\n");

    // Telusuri setiap node dari head hingga NULL
    while (curr != NULL) {
        if (mengandungKata(curr->judul, keyword)) {
            int m, s;
            detikKeMenit(curr->durasi, &m, &s);
            printf("  %-4d %-30s %-25s %02d:%02d\n",
                   no, curr->judul, curr->penyanyi, m, s);
            ditemukan++;
        }
        no++;
        curr = curr->next;
    }

    if (ditemukan == 0)
        printf("  [!] Judul yang mengandung \"%s\" tidak ditemukan.\n", keyword);
    else
        printf("\n  Ditemukan %d lagu.\n", ditemukan);
}

// --- Binary Search berdasarkan Penyanyi ---
// Binary search membutuhkan data yang sudah terurut
// Kita salin ke array, urutkan dulu dengan bubble sort, lalu binary search
void binarySearchPenyanyi(const char *keyword) {
    int n = hitungNode();
    if (n == 0) {
        printf("  [!] Playlist kosong.\n");
        return;
    }

    // Salin linked list ke array pointer sementara
    Lagu **arr = (Lagu **)malloc(n * sizeof(Lagu *));
    if (!arr) {
        printf("[!] Gagal alokasi memori.\n");
        return;
    }
    salinKeArray(arr, n);

    // Urutkan array berdasarkan penyanyi dulu (A-Z) agar binary search bisa bekerja
    // Pakai bubble sort sederhana di sini karena hanya untuk kebutuhan search
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (bandingkanString(arr[j]->penyanyi, arr[j+1]->penyanyi) > 0) {
                Lagu *tmp  = arr[j];
                arr[j]     = arr[j+1];
                arr[j+1]   = tmp;
            }
        }
    }

    printf("  [Binary Search] Mencari penyanyi \"%s\":\n", keyword);
    printf("  (Data diurutkan A-Z dulu, lalu binary search dijalankan)\n\n");
    printf("  %-4s %-30s %-25s %s\n", "No", "Judul", "Penyanyi", "Durasi");
    printf("  ----  ------------------------------  -------------------------  --------\n");

    // Binary search: cari posisi awal yang cocok
    int kiri = 0, kanan = n - 1, tengah;
    int posisiAwal = -1;

    while (kiri <= kanan) {
        tengah = (kiri + kanan) / 2;
        int cmp = bandingkanString(arr[tengah]->penyanyi, keyword);

        if (cmp == 0) {
            posisiAwal = tengah; // Ketemu, catat posisi
            kanan = tengah - 1; // Cari lagi ke kiri kalau ada yang sama
        } else if (cmp < 0) {
            kiri = tengah + 1;  // Keyword lebih besar, geser ke kanan
        } else {
            kanan = tengah - 1; // Keyword lebih kecil, geser ke kiri
        }
    }

    int ditemukan = 0;
    if (posisiAwal != -1) {
        // Cetak semua hasil yang namanya sama persis (exact match)
        // Cek juga ke kanan dari posisiAwal kalau ada nama penyanyi sama
        int idx;
        int no = 1;
        for (idx = posisiAwal; idx < n; idx++) {
            if (bandingkanString(arr[idx]->penyanyi, keyword) == 0) {
                int m, s;
                detikKeMenit(arr[idx]->durasi, &m, &s);
                printf("  %-4d %-30s %-25s %02d:%02d\n",
                       no++, arr[idx]->judul, arr[idx]->penyanyi, m, s);
                ditemukan++;
            } else {
                break; // Sudah melewati nama penyanyi yang cocok
            }
        }
    }

    if (ditemukan == 0)
        printf("  [!] Penyanyi \"%s\" tidak ditemukan.\n", keyword);
    else
        printf("\n  Ditemukan %d lagu dari penyanyi \"%s\".\n", ditemukan, keyword);

    free(arr); // Bebaskan array sementara
}


// --- Menu Cari Lagu ---
// Pilih cari berdasarkan judul (sequential) atau penyanyi (binary)
void menuCariLagu() {
    clearScreen();
    printJudul("CARI LAGU");

    printf("  Cari berdasarkan:\n\n");
    printf("  1. Judul    (Sequential Search)\n");
    printf("  2. Penyanyi (Binary Search)\n");
    printf("  0. Batal\n");
    printGaris();
    printf("Pilihan: ");

    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == 0) return;

    if (pilihan != 1 && pilihan != 2) {
        printf("[!] Pilihan tidak valid.\n");
        pauseProgram();
        return;
    }

    char keyword[50];
    if (pilihan == 1) {
        printf("\nMasukkan judul lagu yang dicari: ");
    } else {
        printf("\nMasukkan nama penyanyi yang dicari: ");
    }
    scanf(" %49[^\n]", keyword);

    printf("\n");

    if (pilihan == 1) {
        sequentialSearchJudul(keyword);
    } else {
        binarySearchPenyanyi(keyword);
    }

    pauseProgram();
}
