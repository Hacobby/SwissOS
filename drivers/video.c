#include "stdint.h"
#include "video.h"
#include "../io.h"

int cursor_x = 0; // Posicion actual del cursor en la columna
int cursor_y = 0; // Posicion actual del cursor en la fila

// Funcion para actualizar la posicion del cursor en la pantalla
void update_cursor(int x, int y){
    uint16_t pos = y * MAX_COLS + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Funcion para imprimir un caracter en la pantalla
void kprint_char(char character, int col, int row, char attribute_byte){
    volatile uint16_t* vidptr = (volatile uint16_t*) VIDEO_ADDRESS;
    if (!attribute_byte) attribute_byte = WHITE_ON_BLACK;

    int offset = row * MAX_COLS + col;
    vidptr[offset] = (uint16_t) character | (uint16_t) attribute_byte << 8;
}

// Desplazar la pantalla hacia arriba cuando el cursor llegue al final
void scroll(){
    volatile uint16_t* vidptr = (volatile uint16_t*) VIDEO_ADDRESS;

    // Mover cada fila hacia arriba
    for (int i = 0; i < MAX_ROWS - 1; i++){
        for (int j = 0; j < MAX_COLS; j++){
            vidptr[i * MAX_COLS + j] = vidptr[(i + 1) * MAX_COLS + j];
        }
    }

    // Limpiar la ultima fila
    for (int j = 0; j < MAX_COLS; j++){
        vidptr[(MAX_ROWS - 1) * MAX_COLS + j] = (uint16_t) ' ' | (uint16_t) WHITE_ON_BLACK << 8;
    }

    cursor_y = MAX_ROWS - 1; // Mantener el cursor en la ultima fila
}

// Limpiar la pantalla y resetear el cursor
void clear_screen(){
    volatile uint16_t* vidptr = (volatile uint16_t*) VIDEO_ADDRESS;
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++){
        vidptr[i] = (uint16_t) ' ' | (uint16_t) WHITE_ON_BLACK << 8;
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(cursor_x, cursor_y);
}

// Funcion para imprimir una cadena de texto en la pantalla
void kprint(char* message){
    for (int i = 0; message[i] != 0; i++){

        if (message[i] == '\n'){ // Nueva linea
            cursor_y++;
            cursor_x = 0;
        } else if (message[i] == '\b') { // Retroceso
            if (cursor_x > 0) { // Retroceder el cursor una posicion a la izquierda
                cursor_x--;
            } 
            // Borrar el caracter en la posicion actual
            kprint_char(' ', cursor_x, cursor_y, WHITE_ON_BLACK);
        }
        else { // Caracter normal
            kprint_char(message[i], cursor_x, cursor_y, WHITE_ON_BLACK);
            cursor_x++;
        }
        if (cursor_x >= MAX_COLS){
            cursor_x = 0;
            cursor_y++;
        }

        if (cursor_y >= MAX_ROWS){
            scroll();
        }
    }
    update_cursor(cursor_x, cursor_y);
}