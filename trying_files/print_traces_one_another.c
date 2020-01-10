#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void){

    size_t      len = 0;
    ssize_t     read;
    FILE        *file1, *file2, *file_ref;
    char        *pageMod, *hexAdr, *token, *line = NULL, *line1 = NULL, *line2 = NULL;
    long        decAddr, serial_num = 0;         
    int         i, j, k, num_of_address_read, pageNum, empty_frame, min_ser, min_pos, orio;


    file1 = fopen("bzip.trace", "r");
    if(file1==NULL){
        fprintf(stderr, "Didn't open bzip.trace.\n");
        return -1;
    }
    file2 = fopen("gcc.trace", "r");
    if(file2==NULL){
        fprintf(stderr, "Didn't open gcc.trace.\n");
        return -1;
    }


    file_ref = file1;
    line = line1;
    num_of_address_read = 0;
    orio = 15;
    printf("\n\nPROTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO ARXEIO\n\n");
    read = getline( &line, &len, file_ref );
    while( read != -1 && num_of_address_read<orio ){
        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / 4096;
        token = strtok(NULL, " \t");   // operation
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);

        num_of_address_read++;
        free(hexAdr);
        if(num_of_address_read<orio){
            read = getline( &line, &len, file_ref );
        }
        // if(num_of_address_read%15==0){
        //     printf("\n");
        // }
    }

    free(line);
    file_ref = file2;
    line = line2;
    num_of_address_read = 0;
    orio = 15;
    printf("\nDEUTEROOOOOOOOOOOOOOOOOOOOOOOOOOOOO ARXEIO\n\n");
    read = getline( &line, &len, file_ref );
    while( read != -1 && num_of_address_read<orio ){
        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / 4096;
        token = strtok(NULL, " \t");   // operation
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);

        num_of_address_read++;
        free(hexAdr);
        if(num_of_address_read<orio){
            read = getline( &line, &len, file_ref );
        }
        // if(num_of_address_read%15==0){
        //     printf("\n");
        // }
    }



    free(line);
    file_ref = file1;
    line = line1;
    num_of_address_read = 0;
    printf("\n\nPROTO ARXEIO PROTO\n\n");
    read = getline( &line, &len, file_ref );
    while( read != -1 && num_of_address_read<15 ){
        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / 4096;
        token = strtok(NULL, " \t");   // operation
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);

        num_of_address_read++;
        free(hexAdr);
        if(num_of_address_read<15){
            read = getline( &line, &len, file_ref );
        }
    }


    free(line);
    file_ref = file2;
    line = line2;
    num_of_address_read = 0;
    printf("\nDEUTERO ARXEIO PROTO\n\n");
    read = getline( &line, &len, file_ref );
    while( read != -1 && num_of_address_read<15 ){
        // printf("file line is %s", line);
        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / 4096;
        token = strtok(NULL, " \t");   // operation
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);

        num_of_address_read++;
        free(hexAdr);
        if(num_of_address_read<15){
            read = getline( &line, &len, file_ref );
        }
    }





    free(line);
    fclose(file1);
    fclose(file2);
    return 0;
}