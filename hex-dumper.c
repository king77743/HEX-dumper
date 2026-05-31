#include <stdio.h>
#include <stdlib.h>

int main() {
    system(""); 
    while (1) { 
        FILE* file = NULL;
        char path[260];        
        while (1) {
            printf("\033[1;32m[+]\033[0m Path to file: ");
            if (fgets(path, sizeof(path), stdin) == NULL) {
                return 1;
            }
            for (int i = 0; i < sizeof(path); i++) {
                if (path[i] == '\n') {
                    path[i] = '\0';
                    break;
                }
                if (path[i] == '\0') {
                    break;
                }
            }
            file = fopen(path, "rb");
            if (file == NULL) {
                printf("\033[1;31m[-] ERROR: File not found!\033[0m\n");
                continue;
            }
            break;
        }
        int count = 0, address = 0;
        int ch;
        char buff[16];
        while ((ch = fgetc(file)) != EOF) {
            if (count == 0) {
                printf("\033[1;36m%08X\033[0m  ", address);
            }
            printf("%02X ", ch);
            if (ch >= 32 && ch <= 126) {
                buff[count] = (char)ch;
            } else {
                buff[count] = '.';
            }
            count++;
            if (count == 16) {
                printf(" | \033[1;32m");
                for (int i = 0; i < 16; i++) {
                    printf("%c", buff[i]);
                }
                printf("\033[0m\n");

                address += 16;
                count = 0;
            }
        }
        if (count > 0) {
            for (int i = count; i < 16; i++) {
                printf("   ");
            }
            printf(" | \033[1;32m");
            for (int i = 0; i < count; i++) {
                printf("%c", buff[i]);
            }
            printf("\033[0m\n");
        }
        fclose(file);
        printf("\n\033[1;33m[!]\033[0m Do you want to analyze another file? (y/n): ");
        int response = getchar();  
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (response != 'y' && response != 'Y') {
            break; 
        }  
        printf("\n"); 
    }
    return 0;
}
