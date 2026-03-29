#ifndef RECOVERY_H
#define RECOVERY_H

// Cek file recovery saat startup, muat isi ke buffer jika ada
void checkRecovery();

// Simpan isi buffer ke file recovery (autosave & crash handler)
void writeRecovery();

// Hapus file recovery saat keluar normal
void clearRecovery();

#endif