#ifndef RECOVERY_H
#define RECOVERY_H

// Cek recovery file saat startup, muat isi ke buffer jika ada
// void checkRecovery(char *text);

// Tulis isi buffer ke recovery file (autosave / saat crash)
// void writeRecovery(const char *text);
void checkRecovery();
void writeRecovery();

// Hapus recovery file (dipanggil saat exit normal)
void clearRecovery();

#endif