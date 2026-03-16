#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

#define VIDEO_ADDRESS 0XB8000 // Direccion de memoria de video para texto en modo texto
#define MAX_ROWS 25 // Numero maximo de filas en la pantalla
#define MAX_COLS 80 // Numero maximo de columnas en la pantalla
#define WHITE_ON_BLACK 0x0a // Atributo de color para texto verde

void kprint_char(char character, int col, int row, char attribute_byte);
void kprint(char* message);
void update_cursor(int x, int y);
void clear_screen();

#endif