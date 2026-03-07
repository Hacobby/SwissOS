#ifndef IO_H
#define IO_H

#include <stdint.h>

// Funciones para escribir en puertos de E/S
void outb(uint16_t port, uint8_t data);

// Funciones para leer de puertos de E/S
uint8_t inb(uint16_t port);

#endif