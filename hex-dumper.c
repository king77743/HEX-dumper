#include <stdio.h>
#include <stdlib.h>

int main() {
    system("");
    char path[260];
    
    printf("Path to file: ");
    if (scanf("%259s", path) != 1) {
        return 1;
    }

    FILE* in = fopen(path, "rb");
    if (in == NULL) {
        printf("ERROR: file not found!\n");
        return 2;
    }

    int count = 0, address = 0;
    int ch;
    char buff[16];
    
    printf("\033[1;36m%08X\033[0m  ", address);

    while (((ch = fgetc(in)) != EOF)) {
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
        printf("\r          \r"); 
    }

    fclose(in);
    printf("\nPress ENTER to continue...");
    int c;
    while((c=getchar())!='\n' && c!=EOF);
    getchar();
    return 0;
}