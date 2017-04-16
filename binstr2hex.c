/*
* Program: binstr2hex.c
* Purpose: Converting Binary Strings to hex
* Author: Binh Vo
* Date: 10/2/16
*/
#include <stdio.h>
#include <stdlib.h>
#define N 500

long biToHex(char grid[][N], int m);
int main() {
    char c;
    char BitStringGrid[N][N];
    int i, j;
    int length = 0;
    int cint;
    int bitString[N];
    //Scanning BitString into Grid

    for (i = 0; (c = getchar()) != EOF; i++) {
        scanf("\n");
        for (j = 0; j < 16; j++) {
            BitStringGrid[i][j] = c;
            c = getchar();
        }
        length++;
        //printf("\n");
    }
    //printf("Length is: %d\n", length);
    for (i = 0; i < length; i++) {
        printf("%04lX\n", biToHex(BitStringGrid, i));
    }
}

long biToHex(char grid[][N], int m) {
    //int biInt[16];
    char biChar[16];
    int j;
    long bitInt, hexaval = 0, k = 1, remainder;
    for (j = 0; j < 16; j++)
        biChar[j] = grid[m][j];
    bitInt = strtol(biChar, NULL, 10); //Binary char to Bitstring int
    
    while (bitInt != 0) {
        remainder = bitInt % 10;
        hexaval = hexaval + remainder * k;
        k = k * 2;
        bitInt = bitInt / 10;
    }
    biChar[0] = '\0';
    return hexaval;
  
}

