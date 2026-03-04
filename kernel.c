#include <stdint.h>
#include "drivers/video.c"

void kernel_main(){
    // direccion de memoria de video
    volatile uint16_t* terminalBuffer = (volatile uint16_t*) VIDEO_ADDRESS;
    char* msj = "Bienvenido a SwissOS";

    // Limpiar la pantalla llenando el buffer de video con espacios en blanco
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++){
        terminalBuffer[i] = (uint16_t) ' ' | (uint16_t) WHITE_ON_BLACK << 8;
    }

    // Escribir el mensaje en la pantalla
    for (int i = 0; (msj[i] != '\0'); i++){
        terminalBuffer[i] = (uint16_t) msj[i] | (uint16_t) WHITE_ON_BLACK << 8;
    }

    while(1){
        // Loop infinito para mantener el kernel corriendo
    };
}
