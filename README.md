# Terminal Text Editor - Proyek 2

Terminal Text Editor adalah aplikasi penyunting teks sederhana berbasis terminal yang dibuat sebagai bagian dari tugas mata kuliah **Proyek 2**.

Program ini terinspirasi dari aplikasi **Notepad** pada sistem operasi Windows yang berfungsi untuk membuat, membuka, mengedit, dan menyimpan file teks tanpa format (plain text).

Implementasi program menggunakan **bahasa C** dengan representasi data menggunakan **struktur array 2 dimensi**.

---

# Overview

Text editor ini dirancang untuk memungkinkan pengguna melakukan manipulasi file teks langsung dari terminal.

Struktur data utama yang digunakan adalah **Array 2 Dimensi**:

- Baris → merepresentasikan **line teks**
- Kolom → merepresentasikan **karakter**

Akses karakter dilakukan menggunakan:

```
text[row][col]
```

Posisi kursor dikontrol menggunakan:

```
current_row
current_col
```

---

# Fitur

Fitur utama yang diimplementasikan dalam program ini:

- **Create File** – Membuat file teks baru  
- **Open File** – Membuka file teks yang sudah ada  
- **Update File** – Mengedit isi teks dalam file  
- **Save File** – Menyimpan isi array ke file  
- **Save As** – Menyimpan isi teks ke file baru  
- **Auto Recovery** – Menyimpan perubahan ke file sementara (recovery.tmp) secara otomatis setiap ada perubahan. Data akan dipulihkan otomatis saat program dibuka kembali setelah crash.

---

# Setup Environment

Agar program dapat dijalankan, pastikan environment berikut tersedia:

- Sistem operasi **Windows / Linux / MacOS**
- **Terminal / Command Prompt**
- **GCC Compiler** untuk bahasa C
- **Git** untuk clone repository

Clone repository project:

```bash
git clone https://github.com/hmzahiqball/proyek2-texteditor.git
```

Masuk ke folder project:

```bash
cd proyek2-texteditor
```

Compile program:

```bash
gcc main.c -o texteditor
```

---

# Instalasi

Jika GCC belum tersedia, install **MinGW** terlebih dahulu:

1. Download MinGW dari https://www.mingw-w64.org
2. Install dan tambahkan path GCC ke Environment Variables Windows
3. Verifikasi instalasi:

```bash
gcc --version
```

---

## 1. Compile Program

Pastikan semua file `.c` ada di satu folder, lalu compile menggunakan Command Prompt:

```bash
gcc main.c recovery.c file_io.c buffer.c render.c input.c cursor.c -o app
```

## 2. Jalankan Program

```bash
./app
```

---

## Contoh Penggunaan

### 1. Menu Utama
Saat program dibuka, akan tampil menu utama:
==================================================
=========== Saw<git> | Text Editor ===============

Open file
Create File
Info
Help
Quit
==================================================
Sawgit>

Pilih menu dengan menekan angka **1-5** di keyboard.

Jika ada data recovery dari sesi sebelumnya:
[!] Recovery ditemukan, 5 baris dimuat.
Tekan sembarang tombol untuk lanjut...

Program langsung masuk ke editor dengan data sesi sebelumnya.

---

### 2. Open File (Angka 1)
Membuka file teks yang sudah ada di disk:
[OPEN] Masukkan nama file: catatan.txt
[INFO] catatan.txt berhasil dibuka. 3 baris dimuat.
Setelah file terbuka, program langsung masuk ke mode editor.

---

### 3. Create File (Angka 2)
Membuat file baru dengan buffer kosong, langsung masuk mode editor.

---

### 4. Mode Editor — Shortcut Keyboard
Setelah masuk mode editor, gunakan shortcut berikut:
Ctrl+S  → Simpan file (Save jika sudah punya nama, Save As jika baru)
Ctrl+O  → Buka file lain dari dalam editor
Ctrl+N  → Buat file baru dari dalam editor
Ctrl+Q  → Keluar dari program
Ctrl+I  → Info aplikasi
Ctrl+G  → Bantuan shortcut
ESC     → Kembali ke menu utama
Panah   → Navigasi kursor

---

### 5. Simpan File (Ctrl+S)
**Jika file sudah punya nama (dari Open):**
[INFO] Perubahan berhasil disimpan ke file catatan.txt
Tekan sembarang tombol...

**Jika file baru (dari Create):**
[SAVE AS] Masukkan nama file baru: output.txt
[INFO] Perubahan berhasil disimpan ke file output.txt
Tekan sembarang tombol...
Nama file tidak boleh kosong — program akan minta input ulang.

---

### 6. Status Bar
Di bagian bawah editor selalu tampil status terkini:

[Unsaved Changes] | File: catatan.txt | Baris: 5
Posisi: Baris 3, Kolom 7 | Ctrl+S: Save | ESC: Menu
- `[Unsaved Changes]` → ada perubahan belum disimpan
- `[Saved]` → semua perubahan sudah tersimpan

---

### 7. Autosave & Recovery
- Setiap keystroke otomatis disimpan ke `recovery.tmp`
- Jika program crash atau di-kill mendadak (SIGTERM), data tetap aman
- Saat program dibuka kembali, data dipulihkan otomatis:
[!] Recovery ditemukan, 5 baris dimuat.
Tekan sembarang tombol untuk lanjut...
Program langsung masuk editor dengan data sesi sebelumnya.

Contoh saat crash:
[!] Program diinterupsi. Menyimpan recovery...
[!] Recovery tersimpan. Program keluar.

---

### 8. Keluar Program (Ctrl+Q)
**Jika ada perubahan belum disimpan:**
[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n):

**Jika sudah tersimpan:**
[QUIT] Keluar dari Saw<git>? (y/n):
Tekan `y` untuk konfirmasi keluar. `recovery.tmp` otomatis dihapus.

## Cara Kerja Recovery

Program ini dilengkapi fitur **Auto Recovery** untuk mencegah kehilangan data.

### Alur Recovery:
1. Setiap kali user mengetik lalu data otomatis disimpan ke `recovery.tmp`.
2. Jika program crash (mati mendadak), maka `recovery.tmp` tetap ada.
3. Saat program dibuka kembali maka data dipulihkan otomatis dari `recovery.tmp`.
4. Jika user berhasil Ctrl+S (save), recovery.tmp otomatis dihapus karena data sudah aman di file .txt permanen.
5. Jika user keluar normal (Ctrl+Q + konfirmasi), 
   recovery.tmp dihapus otomatis.
6. Jika program crash atau di-kill dari luar (SIGTERM), signal handler otomatis memanggil writeRecovery() sebelum program mati dan data tetap aman.

### File Recovery:

| File | Fungsi |
|------|--------|
| `recovery.tmp` | File sementara untuk autosave |
| `recovery.c` | Modul yang mengelola recovery |
| `recovery.h` | Header file modul recovery |

### Fungsi Recovery:

| Fungsi | Keterangan |
|--------|------------|
| `checkRecovery()` | Cek dan muat data recovery saat startup. Return `1` jika ditemukan (langsung buka editor), return `0` jika tidak ada (tampilkan menu) |
| `writeRecovery()` | Simpan isi buffer ke `recovery.tmp` — dipanggil otomatis setiap keystroke |
| `clearRecovery()` | Hapus `recovery.tmp` saat keluar normal atau setelah save berhasil |

---

### Fungsi Recovery:
1. `checkRecovery()` berfungsi untuk cek dan muat data recovery saat startup. Mengembalikan nilai 1 jika recovery ditemukan (program langsung buka editor), atau 0 jika tidak ada (program tampilkan menu utama).
2. `writeRecovery()` berfungsi untuk menyimpan buffer ke recovery.tmp.
3. `clearRecovery()` berfungsi untuk menghapus recovery.tmp saat exit normal.

---

# Identitas Tim

| NIM | Nama | ID Github | Manager |
|-----|-----|-----|-----|
| 251511056 | Putra Suyapratama | hmzahiqball | Pak Rizki |
| 251511057 | R. Neysa Rahma Velda | Neysavelda | Pak Rizki |
| 251511061 | Tania Dwi Pangesti | taniadwip | Pak Rizki |

---

# Repository

Github Team  
https://github.com/hmzahiqball/proyek2-texteditor