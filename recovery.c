#include <stdio.h>
#include "recovery.h"

#define RECOVERY_FILE "recovery.tmp"

void checkRecovery(char *text) {
    FILE *fp = fopen(RECOVERY_FILE, "r");

    if (fp != NULL) {
        char ch;
        int i = 0;

        while ((ch = fgetc(fp)) != EOF) {
            text[i++] = ch;
        }
        text[i] = '\0';

        fclose(fp);

        printf("Recovery loaded ke buffer!\n");
    }
}
void writeRecovery(char *text) {
    FILE *fp = fopen(RECOVERY_FILE, "w");

    if (fp != NULL) {
        fprintf(fp, "%s", text);
        fclose(fp);
    }
}

void clearRecovery() {
    remove(RECOVERY_FILE);
}