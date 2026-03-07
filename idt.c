#include <stdint.h>
#include "idt.h"

extern  void idt_load();

// Configurar entrada individual en la IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags){
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Inicializar la IDT
void idt_install(){
    // Configurar el puntero de la IDT
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t) &idt;

    // Limpiar la IDT
    for (int i = 0; i < 256; i++){
        idt_set_gate(i, 0, 0, 0);
    }

    // Aqui se pueden configurar las entradas de la IDT para las interrupciones que se deseen manejar

    // Cargar la IDT usando ensamblador
    idt_load();
}