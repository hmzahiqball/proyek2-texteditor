# Terminal Text Editor - Proyek 2

Terminal Text Editor adalah aplikasi penyunting teks sederhana berbasis terminal yang dibuat sebagai bagian dari tugas mata kuliah **Proyek 2**.

Program ini terinspirasi dari aplikasi **Notepad** pada sistem operasi Windows yang berfungsi untuk membuat, membuka, mengedit, dan menyimpan file teks tanpa format (plain text).

Implementasi program menggunakan **bahasa C** dengan representasi data menggunakan **struktur array 2 dimensi**.

---

# Overview

Text editor ini dirancang untuk memungkinkan pengguna melakukan manipulasi file teks langsung dari terminal Windows.

Struktur data utama yang digunakan adalah **Array 2 Dimensi**:

- Baris → merepresentasikan **line teks**
- Kolom → merepresentasikan **karakter**

Akses karakter dilakukan menggunakan:

```c
text_buffer[row][col]
```

Posisi kursor dikontrol menggunakan:

```c
cursor_row
cursor_col
```

---

# Fitur

Fitur utama yang diimplementasikan dalam program ini:

- **Create File** – Membuat file teks baru
- **Open File** – Membuka file teks yang sudah ada
- **Update File** – Mengedit isi teks dalam editor
- **Save File** – Menyimpan isi buffer ke file (Ctrl+S)
- **Save As** – Menyimpan ke nama file baru jika belum pernah disimpan
- **Auto Recovery** – Menyimpan perubahan ke file sementara `recovery.tmp` secara otomatis setiap ada perubahan. Data dipulihkan otomatis saat program dibuka kembali setelah crash.

---

# Setup Environment

Agar program dapat dijalankan, pastikan environment berikut tersedia:

- Sistem operasi **Windows**
- **Command Prompt / PowerShell**
- **GCC Compiler** untuk bahasa C (via MinGW)
- **Git** untuk clone repository

Clone repository project:

```bash
git clone https://github.com/hmzahiqball/proyek2-texteditor.git
```

Masuk ke folder project:

```bash
cd proyek2-texteditor
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

# Cara Pakai

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

```
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

Pilih menu dengan menekan angka **1-5** di keyboard.

Jika ada data recovery dari sesi sebelumnya:

```
[!] Recovery ditemukan, 5 baris dimuat.
Tekan sembarang tombol untuk lanjut...
```

Program langsung masuk ke editor dengan data sesi sebelumnya.

---

### 2. Open File (Angka 1)

Membuka file teks yang sudah ada di disk:

```
[OPEN] Masukkan nama file: catatan.txt
[INFO] catatan.txt berhasil dibuka. 3 baris dimuat.
```

Setelah file terbuka, program langsung masuk ke mode editor.

---

### 3. Create File (Angka 2)

Membuat file baru dengan buffer kosong, langsung masuk mode editor.

---

### 4. Mode Editor — Shortcut Keyboard

Setelah masuk mode editor, gunakan shortcut berikut:

```
Ctrl+S  → Simpan file (Save jika sudah punya nama, Save As jika baru)
Ctrl+O  → Buka file lain dari dalam editor
Ctrl+N  → Buat file baru dari dalam editor
Ctrl+Q  → Keluar dari program
Ctrl+I  → Info aplikasi
Ctrl+G  → Bantuan shortcut
ESC     → Kembali ke menu utama
Panah   → Navigasi kursor
```

---

### 5. Simpan File (Ctrl+S)

**Jika file sudah punya nama (dari Open):**

```
[INFO] Perubahan berhasil disimpan ke file catatan.txt
Tekan sembarang tombol...
```

**Jika file baru (dari Create):**

```
[SAVE AS] Masukkan nama file baru: output.txt
[INFO] Perubahan berhasil disimpan ke file output.txt
Tekan sembarang tombol...
```

---

### 6. Status Bar

Di bagian bawah editor selalu tampil status terkini:

```
[Unsaved Changes] | File: catatan.txt | Baris: 5
Posisi: Baris 3, Kolom 7 | Ctrl+S: Save | ESC: Menu
```

- `[Unsaved Changes]` → ada perubahan yang belum disimpan
- `[Saved]` → semua perubahan sudah tersimpan ke file

---

### 7. Autosave & Recovery

- Setiap keystroke otomatis disimpan ke `recovery.tmp`
- Jika program crash atau di-kill mendadak, signal handler otomatis memanggil `writeRecovery()` sebelum program mati — data tetap aman
- Saat program dibuka kembali, data dipulihkan otomatis

Contoh saat crash:

```
[!] Program diinterupsi. Menyimpan recovery...
[!] Recovery tersimpan. Program keluar.
```

---

### 8. Arsitektur Buffer & Logika Manipulasi Teks

Bagian ini menjelaskan bagaimana data teks dikelola di dalam memori menggunakan struktur array 2 dimensi dan bagaimana manipulasi karakter dilakukan secara efisien melalui operasi memori langsung.

#### 1. Struktur Data Utama
Program menggunakan representasi **Array 2D Statis** sebagai basis penyimpanan teks:
* **`text_buffer[MAX_ROW][MAX_COL]`**: Array global yang berfungsi sebagai buffer teks utama untuk menyimpan setiap karakter.
* **`line_length[MAX_ROW]`**: Array pendukung yang melacak jumlah karakter secara presisi pada setiap baris.
* **`total_lines`**: Variabel global yang mencatat jumlah baris yang saat ini terisi di dalam buffer.

#### 2. Mekanisme Edit Teks
Untuk memberikan pengalaman penyuntingan yang responsif, program mengimplementasikan logika pergeseran memori (*memory shifting*):

* **Penyisipan Karakter (Insert)**: Saat karakter baru dimasukkan, program menggunakan fungsi `memmove` untuk menggeser seluruh karakter di sebelah kanan kursor sebanyak satu posisi ke kanan.
* **Penghapusan (Backspace)**: 
    * Jika dilakukan di tengah baris, karakter akan digeser ke kiri menggunakan `memmove`.
    * Jika dilakukan di awal baris (kolom 0), isi baris saat ini akan digabungkan ke baris sebelumnya menggunakan `memcpy`, lalu baris-baris di bawahnya digeser naik untuk mengisi kekosongan.
* **Pemisahan Baris (Enter/Newline)**: Teks di sebelah kanan kursor disalin ke baris baru menggunakan `memcpy`. Baris-baris di bawah posisi kursor kemudian digeser turun satu tingkat untuk menjaga urutan teks.

---

### 9. Sistem Kontrol Kursor & Viewport

Modul kursor mengelola navigasi pengguna dan memastikan tampilan terminal tetap sinkron dengan posisi input di dalam buffer.

#### 1. Boundary Protection (Limit Kursor)
Fungsi `limitCursorBounds` bertugas menjaga agar kursor tetap berada dalam area yang valid:
* Kursor tidak diizinkan bergerak melampaui indeks baris yang tersedia (`total_lines`).
* Kursor dibatasi secara horizontal oleh panjang teks pada baris yang bersangkutan (`line_length`).

#### 2. Navigasi dan Viewport Scrolling
* **Smart Navigation**: Saat berpindah baris (atas/bawah), jika baris tujuan memiliki panjang yang lebih pendek dari posisi kolom kursor saat ini, kursor secara otomatis akan berpindah ke akhir baris tersebut.
* **Viewport Scrolling**: Fungsi `adjust_viewport` mendeteksi jika kursor bergerak keluar dari batas vertikal layar terminal. Jika ini terjadi, variabel `view_row_offset` akan diperbarui sehingga tampilan layar bergeser mengikuti pergerakan kursor.

---

### 10. Spesifikasi Teknis Buffer

Implementasi buffer ini memiliki batasan teknis sebagai berikut:

| Komponen | Deskripsi | Batas Maksimal |
| :--- | :--- | :--- |
| **Kapasitas Baris** | Jumlah total baris dalam buffer | 100 Baris |
| **Kapasitas Kolom** | Jumlah total karakter per baris | 100 Karakter |
| **Tipe Data** | Format penyimpanan karakter | `char` (1 Byte) |
| **Scrolling** | Mekanisme tampilan | Vertikal (Row-offset) |

---

### 11. Keluar Program (Ctrl+Q)

**Jika ada perubahan belum disimpan:**

```
[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n):
```

**Jika sudah tersimpan:**

```
[QUIT] Keluar dari Saw<git>? (y/n):
```

Tekan `y` untuk konfirmasi. `recovery.tmp` otomatis dihapus saat keluar normal.

---

## Daftar Shortcut & Pemrosesan Input (ASCII)

Aplikasi ini menggunakan pustaka conio.h dengan fungsi \_getch() untuk menangkap input secara real-time tanpa menekan Enter. Berikut adalah pemetaan kode ASCII yang digunakan:

- Ctrl + N = 14 (Membuat dokumen baru kosong,handleNewFileAction)
- Ctrl + O = 15 (Membuka file dari direktori,handleOpenAction)
- Ctrl + Q = 17 (Keluar dari aplikasi (dengan proteksi),handleExitAction)
- Ctrl + S = 19 (Menyimpan perubahan (Save / Save As),handleSaveAction)
- Ctrl + I = 9 (Menampilkan informasi aplikasi,renderInfoScreen)
- Ctrl + G = 7 (Menampilkan panduan shortcut,renderHelpScreen)
- ESC = 27 (Kembali ke Menu Utama,is_in_editor = 0)
- Enter = 13 (Membuat baris baru atau memecah baris, insert_newline)
- Backspace = 8 (Menghapus karakter di belakang kursor, delete_char)

## Pengaman Data

Program menjamin keamanan data dengan memberikan peringatan [WARNING] pada aksi-aksi destruktif berikut jika variabel is_modified == 1:

- Membuka file lain saat sedang mengedit (Ctrl+O).
- Membuat file baru saat buffer masih berisi perubahan (Ctrl+N).
- Keluar dari aplikasi saat data belum tersimpan (Ctrl+Q).

## Cara Kerja Recovery

Program ini dilengkapi fitur **Auto Recovery** untuk mencegah kehilangan data.

### Alur Recovery:

1. Setiap kali user mengetik, data otomatis disimpan ke `recovery.tmp`
2. Jika program crash atau di-kill mendadak (SIGTERM), signal handler otomatis memanggil `writeRecovery()` sebelum program mati — data tetap aman
3. Saat program dibuka kembali, data dipulihkan otomatis dari `recovery.tmp`
4. Jika user berhasil Ctrl+S, `recovery.tmp` otomatis dihapus karena data sudah aman di file `.txt` permanen
5. Jika user keluar normal (Ctrl+Q + konfirmasi `y`), `recovery.tmp` dihapus otomatis

### File Recovery:

| File           | Fungsi                        |
| -------------- | ----------------------------- |
| `recovery.tmp` | File sementara untuk autosave |
| `recovery.c`   | Modul yang mengelola recovery |
| `recovery.h`   | Header file modul recovery    |

### Fungsi Recovery:

| Fungsi            | Keterangan                                                                                                                            |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `checkRecovery()` | Cek dan muat data recovery saat startup. Return `1` jika ditemukan (langsung buka editor), return `0` jika tidak ada (tampilkan menu) |
| `writeRecovery()` | Simpan isi buffer ke `recovery.tmp` — dipanggil otomatis setiap keystroke                                                             |
| `clearRecovery()` | Hapus `recovery.tmp` saat keluar normal atau setelah save berhasil                                                                    |

---

# Identitas Tim

| NIM       | Nama                 | ID Github   | Manager   |
| --------- | -------------------- | ----------- | --------- |
| 251511056 | Putra Suyapratama    | hmzahiqball | Pak Rizki |
| 251511057 | R. Neysa Rahma Velda | Neysavelda  | Pak Rizki |
| 251511061 | Tania Dwi Pangesti   | taniadwip   | Pak Rizki |

---

# Repository

Github Team: https://github.com/hmzahiqball/proyek2-texteditor
