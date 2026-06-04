#include <stdio.h>
#include <stdlib.h>
#include "clear_bufer.h"
#include "magic_bytes.h"
#include "hex_dumper_run.h"

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
            type_file(file);
            run_analyze(file);
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