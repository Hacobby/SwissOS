#include <stdint.h>

void kernel_main(){
    // direccion de memoria de video
    volatile uint16_t* terminalBuffer = (volatile uint16_t*) 0xB8000;
    char* msj = "Bienvenido a SwissOS";

    for (int i = 0; i < 80 * 25; i++){
        terminalBuffer[i] = (uint16_t) ' ' | (uint16_t) 0x0F << 8;
    }

    for (int i = 0; (msj[i] != '\0'); i++){
        terminalBuffer[i] = (uint16_t) msj[i] | (uint16_t) 0x0F << 8;
    }

    while(1){

    };
}
