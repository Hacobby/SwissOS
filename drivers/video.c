#include "stdint.h"

#define VIDEO_ADDRESS 0XB8000 // Direccion de memoria de video para texto en modo texto
#define MAX_ROWS 25 // Numero maximo de filas en la pantalla
#define MAX_COLS 80 // Numero maximo de columnas en la pantalla
#define WHITE_ON_BLACK 0X0f // Atributo de color para texto blanco

int cursor_x = 0; // Posicion actual del cursor en la columna
int cursor_y = 0; // Posicion actual del cursor en la fila

// Funcion para imprimir un caracter en la pantalla
void kprint_char(char character, int col, int row, char attribute_byte){
    volatile uint16_t* vidptr = (volatile uint16_t*) VIDEO_ADDRESS;
    if (!attribute_byte) attribute_byte = WHITE_ON_BLACK;

    int offset = row * MAX_COLS + col;
    vidptr[offset] = (uint16_t) character | (uint16_t) attribute_byte << 8;
}

// Funcion para imprimir una cadena de texto en la pantalla
void kprint(char* message){
    for (int i = 0; message[i] != 0; i++){
        if (message[i] == '\n'){
            cursor_y++;
            cursor_x = 0;
        } else{
            kprint_char(message[i], cursor_x, cursor_y, WHITE_ON_BLACK);
            cursor_x++;
        }
        if (cursor_x >= MAX_COLS){
            cursor_x = 0;
            cursor_y++;
        }
    }
}