#ifndef LOGICA_H
#define LOGICA_H
#include <stdio.h>
#include "clear_bufer.h"
void run_analyze(FILE *file){
    fseek(file,0,SEEK_END);
    unsigned int size_file=ftell(file);
    fseek(file,0,SEEK_SET);
    int mode;
    unsigned int bytes;
    int quanity;
    printf("[ 1: Full analyze (%u bytes)  |  2: Custom rows limit ]\n",size_file);
    while (1){
        printf("Select analyze mode > ");
        scanf("%d",&mode);
        clear_buff();
        if (mode==1){
            bytes=size_file;
            break;
        }
        else if(mode==2){
            unsigned int stroki=size_file/16;
            unsigned int ostatok=size_file-(stroki*16);
            if (ostatok>0){
                stroki++;
            }
            while (1){
                printf("Enter row to read (max %u): ",stroki);
                if (scanf("%d",&quanity)!=1){
                    printf("ERROR: Invalid number!\n");
                    clear_buff();
                    continue;
                }
                clear_buff();
                if (quanity<0){
                    printf("bytes cannot be negative\n");
                    continue;
                }
                if ((unsigned int)quanity >stroki){
                    printf("you cannot read %d rows. File only has %u rows\n",quanity,stroki);
                    continue;
                }
                if ((unsigned)quanity==stroki && ostatok>0){
                    bytes=(stroki*16)+ostatok;
                }
                bytes=(unsigned int)quanity*16;
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
        while (total<bytes && (ch = fgetc(file)) != EOF) {
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
        if (mode==1){
            break;
        }
        printf("\nContinue Reading? (1: Yes / 2: NO): ");
        int choice;
        scanf("%d",&choice);
        clear_buff();
        if (choice==1) {
            unsigned int cont=size_file-(unsigned int)ftell(file);
            unsigned int cont_stroki=cont/16;
            unsigned int cont_ostatok=cont%16;
            if (cont_ostatok>0){
                cont_stroki++;
            }
            if (cont_stroki==0){
                printf("No more rows to read!\n");
                break;
            }
            while(1){ 
                printf("Enter next rows (max %u): ",cont_stroki);
                if (scanf("%d",&quanity)!=1){
                    printf("ERROR: Invalid number!\n");
                    clear_buff();
                    continue;
                }
                clear_buff();
                
                if (quanity<0){
                    printf("Rows cannot be negative!\n");
                    continue;
                }
                if ((unsigned)quanity>cont_stroki){
                    printf("Invalid rows! maximum: %u\n",cont_stroki);
                    continue;
                }
                if ((unsigned int)quanity == cont_stroki && cont_ostatok > 0) {
                    bytes = cont;
                }
                bytes=(unsigned)quanity*16;
                break;
            }
            printf("\033[2K\033[A\033[2K\033[A\033[2K\033[A");
            fflush(stdout);
        }
        else{
            
            break;
        }
    }   
}
#endif
