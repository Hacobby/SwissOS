#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

// Estructura que representa la cabecera multiboot que el cargador de arranque (bootloader) debe proporcionar al kernel
typedef struct multiboot_info {
    uint32_t flags; // Flags que indican las opciones de arranque
    uint32_t mem_lower; // Cantidad de memoria disponible por debajo de 1MB
    uint32_t mem_upper; // Cantidad de memoria disponible por encima de 1MB

} __attribute__((packed)) multiboot_info_t;

#endif