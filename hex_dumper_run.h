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
            unsigned int stroki=size_file >> 4; 
            unsigned int ostatok=size_file & 15; 
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
                    bytes=(stroki << 4)+ostatok;
                }
                else{bytes=(unsigned int)quanity << 4;}
                
                break;
            }
            break;
        }
        else{
            printf ("ERROR: Invalid mode!\n");
            continue;
        }
    }
    int count, address = 0;
    int bytes_to_read;
    unsigned char buff[4096];
    unsigned char hex_char[]="0123456789ABCDEF";
    char print[30000]; 
    FILE *out_file=NULL;

    if (mode == 1) {
        char out_filename[256];
        while (1) {
            printf("\033[1;32m[+]\033[0m Path to output file: ");
            if (fgets(out_filename, sizeof(out_filename), stdin) == NULL) {
                printf("ERROR: Failed to read!\n");
                continue;
            }
            for (int i = 0; i < sizeof(out_filename); i++) {
                if (out_filename[i] == '\n') {
                    out_filename[i] = '\0';
                    break;
                }
                if (out_filename[i] == '\0') {
                    break;
                }
            }
            out_file = fopen(out_filename, "w");
            if (out_file == NULL) {
                printf("\033[1;31m[-] ERROR: \033[0m");
                perror(out_filename);
                continue;
            }
            break;
        }
    }

    while(1){
        int total=0;
        while (total<bytes) {
            int limit=bytes-total;
            int to_read;
            if(limit<4096){
                to_read=limit;
            }
            else{
                to_read=4096;
            }
            count=fread(buff,1,to_read,file);
            if (count==0){
                break;
            }

            int p=0; 

            for(int offset=0;offset<count;offset+=16){
                int ost=count-offset;
                if (ost<16){
                    bytes_to_read=ost;
                }
                else{
                    bytes_to_read=16;
                }

                if (mode == 2) {
                    print[p++] = '\033'; print[p++] = '['; print[p++] = '1'; 
                    print[p++] = ';';    print[p++] = '3'; print[p++] = '6'; print[p++] = 'm';
                }

                print[p++] = hex_char[(address >> 28) & 0x0F];
                print[p++] = hex_char[(address >> 24) & 0x0F];
                print[p++] = hex_char[(address >> 20) & 0x0F];
                print[p++] = hex_char[(address >> 16) & 0x0F];
                print[p++] = hex_char[(address >> 12) & 0x0F];
                print[p++] = hex_char[(address >> 8)  & 0x0F];
                print[p++] = hex_char[(address >> 4)  & 0x0F];
                print[p++] = hex_char[address         & 0x0F];

                if (mode == 2) {
                    print[p++] = '\033'; print[p++] = '['; print[p++] = '0'; print[p++] = 'm';
                }
                print[p++] = ' ';
                print[p++] = ' ';

                for (int i = 0; i < 16; i++) {
                    if (i < bytes_to_read) {
                        unsigned char ch = buff[offset + i];
                        print[p++] = hex_char[(ch >> 4) & 0x0F];
                        print[p++] = hex_char[ch & 0x0F];
                    } else {
                        print[p++] = ' ';
                        print[p++] = ' ';
                    }
                    print[p++] = ' ';
                }

                print[p++] = '|';
                print[p++] = ' ';

                if (mode == 2) {
                    print[p++] = '\033'; print[p++] = '['; print[p++] = '1'; 
                    print[p++] = ';';    print[p++] = '3'; print[p++] = '2'; print[p++] = 'm';
                }

                for (int i = 0; i < 16; i++) {
                    if (i < bytes_to_read) {
                        unsigned char ch = buff[offset + i];
                        if (ch >= 32 && ch <= 126) {
                            print[p++] = ch;
                        } else {
                            print[p++] = '.';
                        }
                    } else {
                        print[p++] = ' ';
                    }
                }

                if (mode == 2) {
                    print[p++] = '\033'; print[p++] = '['; print[p++] = '0'; print[p++] = 'm';
                }

                print[p++] = '\n'; 
                address += bytes_to_read;
            }
            print[p] = '\0';

            if (mode == 1) {
                fputs(print, out_file);
            } else {
                fputs(print, stdout);
            }

            total += count; 
        }
        
        if (feof(file)){
            if (mode == 1) {
                printf("Full analyze finished! Data saved successfully.\n");
            } else {
                printf("End Of File!\n");
            }
            break;
        }
        if (mode == 1){
            break;
        }
        printf("\nContinue Reading? (1: Yes / 2: NO): ");
        int choice;
        scanf("%d", &choice);
        clear_buff();
        if (choice == 1) {
            unsigned int cont = size_file - (unsigned int)ftell(file);
            unsigned int cont_stroki = cont >> 4;   
            unsigned int cont_ostatok = cont & 15; 
            if (cont_ostatok > 0){
                cont_stroki++;
            }
            if (cont_stroki == 0){
                printf("No more rows to read!\n");
                break;
            }
            while(1){ 
                printf("Enter next rows (max %u): ", cont_stroki);
                if (scanf("%d", &quanity) != 1){
                    printf("ERROR: Invalid number!\n");
                    clear_buff();
                    continue;
                }
                clear_buff();
                
                if (quanity < 0){
                    printf("Rows cannot be negative!\n");
                    continue;
                }
                if ((unsigned)quanity > cont_stroki){
                    printf("Invalid rows! maximum: %u\n", cont_stroki);
                    continue;
                }
                if ((unsigned int)quanity == cont_stroki && cont_ostatok > 0) {
                    bytes = cont;
                }
                else {
                    bytes = (unsigned)quanity << 4; 
                }
                
                break;
            }
            printf("\033[2K\033[A\033[2K\033[A\033[2K\033[A");
            fflush(stdout);
        }
        else {
            break;
        }
    }   

    if (out_file != NULL) {
        fclose(out_file);
    }
}
#endif
