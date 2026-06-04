#ifndef LOGICA_H
#define LOGICA_H
#include <stdio.h>
#include "clear_bufer.h"
void run_analyze(FILE *file){
    fseek(file,0,SEEK_END);
    long size_file=ftell(file);
    fseek(file,0,SEEK_SET);

    int mode;
    int bytes;
    printf("[ 1: Full analyze (%ld bytes)  |  2: Custom byte limit ]\n",size_file);
    
    while (1){
        printf("Select analyze mode > ");
        scanf("%d",&mode);
        clear_buff();
        if (mode==1){
            bytes=(int)size_file;
            break;
        }
        else if(mode==2){
            printf("File size: %ld bytes\n",size_file);
            while (1){
                printf("Enter bytes to read: ");
                if (scanf("%d",&bytes)!=1){
                    printf("ERROR: Invalid number!\n");
                    clear_buff();
                    continue;
                }
                clear_buff();
                if (bytes<0){
                    printf("bytes cannot be negative\n");
                    continue;
                }
                if (bytes>size_file){
                    printf("you cannot read %d bytes. File only has %ld bytes\n",bytes,size_file);
                    continue;
                }
                break;
            }
            break;
        }
        else{
            printf ("ERROR: Invalid mode!\n");
            continue;
        }
    }

    int count = 0, address = 0;
    int ch;
    char buff[16];
    
    while(1){
        int total=0;
    
        while ((ch = fgetc(file)) != EOF) {
            if (total>=bytes){
                break;
            }
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
            total++;
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
        
        if (ch==EOF){
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
            printf("End of file!\n");
            break;
        }

        printf("\nContinue Reading? (1: Yes / 2: NO): ");
        int choice;
        scanf("%d",&choice);
        clear_buff();
        
        if (choice==1) {
            long cont=size_file-ftell(file);
            if (cont<=0){
                printf("No more bytes to read!\n");
                if (count > 0) {
                    for (int i = count; i < 16; i++) printf("   ");
                    printf(" | \033[1;32m");
                    for (int i = 0; i < count; i++) printf("%c", buff[i]);
                    printf("\033[0m\n");
                }
                break;
            }
            while(1){
                printf("Enter next bytes (max %ld): ",cont);
                scanf("%d",&bytes);
                clear_buff();
                if (bytes<0 || bytes>cont){
                    printf("Invalid bytes!\n");
                    continue;
                }
                break;
            }
            
            printf("\033[2K\033[A\033[2K\033[A\033[2K\033[A");
            fflush(stdout);
        }
        else{
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
            break;
        }
    }   
}
#endif