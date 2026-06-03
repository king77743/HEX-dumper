#include <stdio.h>
#include <stdlib.h>
#include "clear_bufer.h"

int main() {
    system(""); 
    while (1){
        int command;
        printf("\033[1;37m[ 1: Analyze  |  2: Clear  |  3: Exit ]\033[0m\n");
        printf("\033[1;37mSelect command >\033[0m ");
        scanf("%d",&command);
        clear_buff();
        if (command==2){
            system("cls");
            continue;
        }
        else if (command==3){
            break;
        }
        else if (command<=0||command>3){
            printf("ERROR: Command is invalid\n");
            continue;
        }
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
            unsigned char magic[8] = {0};
            fread(magic, 1, 8, file);
            fseek(file, 0, SEEK_SET); 

            printf("\n\t\t\033[1;35m[== FORMAT ANALYSIS ==]\033[0m\n");
            if (magic[0] == 0x4D && magic[1] == 0x5A) {
                printf("\t\tFile type: Windows Executable (.EXE / .DLL)\n");
            } 
            else if (magic[0] == 0x89 && magic[1] == 0x50 && magic[2] == 0x4E && magic[3] == 0x47) {
                printf("\t\tFile type: PNG Image\n");
            } 
            else if (magic[0] == 0x50 && magic[1] == 0x4B && magic[2] == 0x03 && magic[3] == 0x04) {
                printf("\t\tFile type: ZIP Archive / Office Document (.docx, .xlsx)\n");
            } 
            else if (magic[0] == 0x25 && magic[1] == 0x50 && magic[2] == 0x44 && magic[3] == 0x46) {
                printf("\t\tFile type: PDF Document\n");
            } 
            else if (magic[0] == 0xFF && magic[1] == 0xD8 && magic[2] == 0xFF) {
                printf("\t\tFile type: JPEG Image\n");
            } 
            else {
                printf("\t\tFile type: Unknown Binary Format\n");
            }
            printf("\t\t\033[1;35m[=====================]\033[0m\n\n");
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
            clear_buff();

            if (response != 'y' && response != 'Y') {
                break; 
            }  
            printf("\n"); 
        }

    }
    return 0;
}
