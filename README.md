# Terminal Text Editor - Proyek 2

Terminal Text Editor adalah aplikasi penyunting teks sederhana berbasis terminal yang dibuat sebagai bagian dari tugas mata kuliah **Proyek 2**.

Program ini terinspirasi dari aplikasi **Notepad** pada sistem operasi Windows yang berfungsi untuk membuat, membuka, mengedit, dan menyimpan file teks tanpa format (plain text).

Implementasi program menggunakan **bahasa C** dengan representasi data memori dinamis menggunakan **struktur Doubly Linked List**.

---

## Overview

Text editor ini dirancang untuk memungkinkan pengguna melakukan manipulasi file teks langsung dari terminal Windows secara efisien dan aman.

Struktur data utama yang digunakan adalah **Doubly Linked List**:
- Setiap baris teks disimpan sebagai **node** (`LineNode`).
- Setiap node terhubung dengan pointer ke baris sebelumnya (`prev`) dan sesudahnya (`next`).
- Akses dan manipulasi baris dilakukan secara dinamis (menggunakan dialokasi memori `malloc` & `realloc`).
- Mencegah masalah batas tetap (fixed-size limit) yang sering ditemui pada array statis.

Posisi kursor dan viewport dikontrol secara presisi menggunakan kordinat baris dan kolom, didukung oleh integrasi **Windows API** (`SetConsoleCursorPosition`) untuk navigasi visual tanpa flicker.

---

## Fitur Utama

- **Create File** – Membuat file teks baru dari buffer kosong.
- **Open File** – Membuka file teks yang sudah ada ke dalam memori (RAM).
- **Update File** – Mengedit teks dengan kapabilitas insert karakter, backspace, delete, dan enter (memecah baris).
- **Save File (Ctrl+S)** – Menyimpan isi buffer kembali ke file disk.
- **Save As (Ctrl+A)** – Menyimpan isi teks dengan nama file baru.
- **Auto Recovery** – Autosave secara cerdas setiap **2 detik** ke `recovery.tmp`. Mendukung restorasi data jika terjadi crash atau program dihentikan paksa (interupsi OS).
- **Dynamic Viewport Scrolling** – Mendukung pengguliran layar (scrolling) vertikal maupun horizontal (kiri-kanan) secara adaptif.
- **Flicker-Free Rendering** – Menghilangkan kedipan (flicker) terminal pada saat layar di-render ulang dengan kontrol visibility kursor berbasis ANSI (`\033[?25l` & `\033[?25h`).

---

## Setup Environment

Agar program dapat dijalankan, pastikan environment berikut tersedia:
- Sistem operasi **Windows**
- **Command Prompt / PowerShell**
- **GCC Compiler** untuk bahasa C (via MinGW)
- **Git** untuk clone repository

Clone repository project:
```bash
git clone [https://github.com/hmzahiqball/proyek2-texteditor.git](https://github.com/hmzahiqball/proyek2-texteditor.git)
cd proyek2-texteditor

```

---

## Instalasi & Kompilasi

Jika GCC belum tersedia, install **MinGW** terlebih dahulu dari [mingw-w64.org](https://www.mingw-w64.org), lalu tambahkan path GCC ke Environment Variables.

**Cara Compile Program:**
Pastikan semua file `.c` ada di satu direktori, lalu kompilasi menggunakan Command Prompt:

```bash
gcc main.c recovery.c file_io.c buffer.c render.c input.c cursor.c -o app

```

**Cara Menjalankan:**

```bash
./app

```

---

## Panduan Penggunaan

### 1. Menu Utama

Saat program dibuka, layar menu utama akan muncul:

```text
==================================================
=========== Saw<git> | Text Editor ===============
1. Open file
2. Create File
3. Info
4. Help
5. Quit
==================================================
Sawgit>

```

Jika terdapat recovery data dari sesi yang crash sebelumnya, sistem akan mendeteksinya:

```text
[!] Recovery ditemukan, 5 baris dimuat.
Tekan sembarang tombol untuk lanjut...

```

### 2. Mode Editor — Shortcut Keyboard

Di dalam mode editor, berbagai aksi dapat dilakukan tanpa harus kembali ke menu utama:

| Shortcut | Aksi |
| --- | --- |
| **Ctrl + S** | Simpan file (Save jika sudah bernama, Save As jika *Untitled*) |
| **Ctrl + A** | Save As (Simpan file sebagai nama baru) |
| **Ctrl + O** | Buka file lain dari disk |
| **Ctrl + N** | Buat file baru (Clear buffer) |
| **Ctrl + I** | Tampilkan informasi aplikasi |
| **Ctrl + G** | Tampilkan bantuan / panduan shortcut |
| **Ctrl + Q** | Keluar dari program (dengan proteksi data belum tersimpan) |
| **ESC** | Kembali ke Menu Utama |
| **Panah** | Navigasi kursor (Atas, Bawah, Kiri, Kanan) |

### 3. Status Bar

Di bagian bawah antarmuka editor, terdapat status bar yang memonitor kondisi file secara real-time:

```text
========================================================================
 [UNSAVED CHANGES] | Berkas: catatan.txt | Total: 5 baris
========================================================================
 Posisi: Baris 3, Kolom 7 | Ctrl+S: Simpan | Ctrl+A: Save As | ESC: Menu

```

* `[UNSAVED CHANGES]`: Buffer memiliki modifikasi yang belum di-write ke file permanen.
* `[SAVED]`: Seluruh data termutakhir sudah aman di disk.

---

## Arsitektur Teks & Manajemen Buffer

Program menggunakan **Doubly Linked List** sebagai pondasi utama (bukan array statis) untuk menjaga fleksibilitas dan penggunaan memori yang dinamis.

### 1. Struktur Data `LineNode`

Setiap baris teks disimpan dalam bentuk struct dinamis:

```c
typedef struct LineNode {
    char *line;      // Isi teks (string dinamis)
    int length;      // Panjang teks saat ini
    int capacity;    // Kapasitas memori teralokasi (auto-resize)
    struct LineNode *prev;
    struct LineNode *next;
} LineNode;

```

### 2. Manipulasi Memori Berbasis Kapasitas

* **Kapasitas Adaptif**: Menggunakan fungsi `ensureCapacity()` yang secara matematis melipatgandakan ukuran alokasi (`realloc`) saat string mendekati batas kapasitasnya, mengoptimalkan rasio kecepatan & konsumsi RAM.
* **Operasi String**:
* Insert karakter mengandalkan `memmove` untuk pergeseran array di dalam buffer baris secara aman.
* Saat menekan Enter di tengah baris, baris dipotong dan sisa teks dipindahkan ke Node baru yang langsung disambung di antara `prev` dan `next` yang relevan.



---

## Sistem Navigasi Kursor & Viewport

Modul kursor (File `cursor.c`) bertugas menerjemahkan logika buffer menjadi posisi visual (tampilan).

### 1. Viewport Dinamis (Vertikal & Horizontal)

Untuk menunjang pengeditan baris panjang maupun dokumen bervolume tinggi, render menggunakan variabel offset:

* `view_row_offset`: Menggeser jendela tampilan ke bawah/atas (Vertical Scrolling).
* `view_col_offset`: Menggeser jendela tampilan ke kanan/kiri (Horizontal Scrolling).

### 2. Boundary Limit & Auto-Adjust

Setiap gerakan panah diproteksi oleh fungsi `limitCursorBounds()`. Jika bergerak dari baris panjang ke baris yang lebih pendek, kursor tidak akan *out-of-bounds*, melainkan otomatis bergeser ke batas maksimum (ujung karakter) baris tujuan. Layar juga otomatis melakukan `adjust_viewport` mengikuti kordinat.

---

## Keamanan Data (Auto Recovery)

Sistem ini didesain tangguh menghadapi Force Close (Crash) dengan intervensi *Signal Handling* (`SIGINT` & `SIGTERM`) pada `main.c`.

### Alur Auto-Recovery:

1. **Perekaman:** Setiap modifikasi yang dilakukan (`is_modified = 1`) akan mentrigger evaluasi autosave. File akan ditulis ulang ke `recovery_new.tmp` hanya setiap **2 detik sekali** (menggunakan fungsi `time()` & `difftime()`), mencegah *bottleneck* I/O pada disk keras.
2. **Safe Write:** Program merename file dari `.tmp` sementara menjadi `recovery.tmp` murni menggunakan `rename()`. Hal ini mencegah file terkorupsi jika aplikasi terhenti tepat pada mili-detik proses penulisan file.
3. **Signal Handler Catch:** Jika user tidak sengaja menutup CMD atau mengirim sinyal Kill, aplikasi akan memanggil `writeRecovery()` secara paksa sebelum shutdown.
4. **Restorasi Otomatis:** Saat program kembali dijalankan dan menemui fail *recovery.tmp*, pengguna secara otomatis akan dikembalikan pada file (*current_filename*) serta baris teks terakhir tanpa adanya data yang hilang.

---

## Daftar Pemrosesan Input (Kode ASCII via Conio.h)

Input ditangani secara *real-time* via `_getch()` (Tanpa menunggu tuts enter).

* `14` = Ctrl + N (Buat Baru)
* `15` = Ctrl + O (Buka File)
* `17` = Ctrl + Q (Keluar)
* `19` = Ctrl + S (Simpan)
* `1`  = Ctrl + A (Save As)
* `9`  = Ctrl + I (Info)
* `7`  = Ctrl + G (Bantuan)
* `27` = ESC (Menu Utama)
* `13` = Enter (Membuat Baris Baru / Split Baris)
* `8`  = Backspace (Hapus Karakter Sebelumnya)
* `224, 72` = Panah Atas
* `224, 80` = Panah Bawah
* `224, 75` = Panah Kiri
* `224, 77` = Panah Kanan
* `224, 83` = Delete (Hapus Karakter di Depan)

---

## Identitas Tim Pengembang

| NIM | Nama | ID Github | Manager |
| --- | --- | --- | --- |
| 251511056 | Putra Suyapratama | hmzahiqball | Pak Rizki |
| 251511057 | R. Neysa Rahma Velda | Neysavelda | Pak Rizki |
| 251511061 | Tania Dwi Pangesti | taniadwip | Pak Rizki |

**Repository GitHub Team:** [proyek2-texteditor](https://github.com/hmzahiqball/proyek2-texteditor)

```

```