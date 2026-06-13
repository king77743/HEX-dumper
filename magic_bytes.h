#ifndef MAGIC_BYTES_H
#define MAGIC_BYTES_H
#include <stdio.h>
void type_file(FILE *file){
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
}
#endif
