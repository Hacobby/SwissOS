#ifndef IDT_H
#define IDT_H

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#include <stdint.h>

// Estructura de una entrada en la IDT
struct idt_entry{
    uint16_t base_low; //16 bits de la direccion de la rutina de interrupcion
    uint16_t sel; //16 bits del selector de codigo en el GDT
    uint8_t  always0; //8 bits que requiere la cpu, siempre 0
    uint8_t  flags; // descriptores de interrupcion, tipo de puerta, privilegios, etc
    uint16_t base_high; //16 bits de la direccion de la rutina de interrupcion
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit; // Limite de la IDT (tamaño de la tabla - 1)
    uint32_t base; // Direccion base de la IDT
} __attribute__((packed));

// Declaracion de la IDT con 256 entradas
extern struct idt_entry idt[256];
extern struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();
void pic_remap();

#endif