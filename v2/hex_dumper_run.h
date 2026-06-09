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
                else{bytes=(unsigned int)quanity*16;}
                
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
    char buff[16];
    while(1){
        int total=0;
        while (total<bytes) {
            
            int limit=bytes-total;
            if(limit<16){
                bytes_to_read=limit;
            }
            else{
                bytes_to_read=16;
            }
            count=fread(buff,1,bytes_to_read,file);
            if (count==0){
                break;
            }
            printf("\033[1;36m%08X\033[0m  ", address);
            for (int i=0;i<16;i++){
                if (i<count){
                    printf("%02X ",(unsigned char) buff[i]);
                }else{
                    printf("   ");
                }
            }
            printf(" | \033[1;32m");
            for (int i = 0; i < count; i++) {
                unsigned char ch=buff[i];
                if (ch>=32 && ch<=126){
                    printf("%c",ch);

                }
                else{
                    printf(".");
                }
            }
            printf("\033[0m\n");
            address+=count;
            total+=count;
            
        }
        
        if (feof(file)){
            printf("End Of File!\n");
            break;

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
                else {bytes=(unsigned)quanity*16;}
                
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
