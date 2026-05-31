#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* file= NULL;
    system("");
    char path[260];
    while(1){
        printf("\033[1;32m[+]\033[0m Path to file: ");
        if (fgets(path,sizeof(path),stdin)==NULL) {
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
            printf("\033[1;31m[-]\033[0m ERROR: File not found!\n");
            continue;
            
        }
        break; 
    }
    int count = 0, address = 0;
    int ch;
    char buff[16];
    
    printf("\033[1;36m%08X\033[0m  ", address);

    while (((ch = fgetc(file)) != EOF)) {
        printf("%02X ", ch);

        if (ch >= 32 && ch <= 122) {
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
            printf("\033[1;36m%08X\033[0m  ", address);
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
    } else {
        printf("          "); 
    }
    fclose(file);
    printf("\nPress ENTER to continue...");
    getchar();
    return 0;
}
