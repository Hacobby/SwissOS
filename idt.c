#include <stdint.h>
#include "idt.h"
#include "io.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();

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

    // Configurar las entradas para las interrupciones de la CPU (0-31)
    idt_set_gate(0, (uint32_t) isr0, 0x10, 0x8E);
    idt_set_gate(1, (uint32_t) isr1, 0x10, 0x8E);
    idt_set_gate(2, (uint32_t) isr2, 0x10, 0x8E);
    idt_set_gate(3, (uint32_t) isr3, 0x10, 0x8E);
    idt_set_gate(4, (uint32_t) isr4, 0x10, 0x8E);
    idt_set_gate(5, (uint32_t) isr5, 0x10, 0x8E);
    idt_set_gate(6, (uint32_t) isr6, 0x10, 0x8E);
    idt_set_gate(7, (uint32_t) isr7, 0x10, 0x8E);
    idt_set_gate(8, (uint32_t) isr8, 0x10, 0x8E);
    idt_set_gate(9, (uint32_t) isr9, 0x10, 0x8E);
    idt_set_gate(10, (uint32_t) isr10, 0x10, 0x8E);
    idt_set_gate(11, (uint32_t) isr11, 0x10, 0x8E);
    idt_set_gate(12, (uint32_t) isr12, 0x10, 0x8E);
    idt_set_gate(13, (uint32_t) isr13, 0x10, 0x8E);
    idt_set_gate(14, (uint32_t) isr14, 0x10, 0x8E);
    idt_set_gate(15, (uint32_t) isr15, 0x10, 0x8E);
    idt_set_gate(16, (uint32_t) isr16, 0x10, 0x8E);
    idt_set_gate(17, (uint32_t) isr17, 0x10, 0x8E);
    idt_set_gate(18, (uint32_t) isr18, 0x10, 0x8E);
    idt_set_gate(19, (uint32_t) isr19, 0x10, 0x8E);
    idt_set_gate(20, (uint32_t) isr20, 0x10, 0x8E);
    idt_set_gate(21, (uint32_t) isr21, 0x10, 0x8E);
    idt_set_gate(22, (uint32_t) isr22, 0x10, 0x8E);
    idt_set_gate(23, (uint32_t) isr23, 0x10, 0x8E);
    idt_set_gate(24, (uint32_t) isr24, 0x10, 0x8E);
    idt_set_gate(25, (uint32_t) isr25, 0x10, 0x8E);
    idt_set_gate(26, (uint32_t) isr26, 0x10, 0x8E);
    idt_set_gate(27, (uint32_t) isr27, 0x10, 0x8E);
    idt_set_gate(28, (uint32_t) isr28, 0x10, 0x8E);
    idt_set_gate(29, (uint32_t) isr29, 0x10, 0x8E);
    idt_set_gate(30, (uint32_t) isr30, 0x10, 0x8E);
    idt_set_gate(31, (uint32_t) isr31, 0x10, 0x8E);

    // Interrupciones de hardware (IRQ0-IRQ15) se remapearan a partir de la interrupcion 32 (0x20)
    idt_set_gate(32, (uint32_t) isr32, 0x10, 0x8E); // IRQ0 - Reloj del sistema
    idt_set_gate(33, (uint32_t) isr33, 0x10, 0x8E); // IRQ1 - Teclado

    // Cargar la IDT
    idt_load();
}

// Remapear el PIC para evitar conflictos con las interrupciones de la CPU
void pic_remap(){
    // Inicializar el PIC maestro y el esclavo
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // Remapear offsets
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    // Configurar la cascada maestro-esclavo
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Configurar modo arquitectura 8086 (x86)
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Mascaras de interrupciones (deshabilitar todas las interrupciones)
    outb(PIC1_DATA, 0xFD); // 11111101 - Habilitar solo la interrupcion del timer (IRQ0)
    outb(PIC2_DATA, 0xFF); // 11111111 - Deshabilitar todas las interrupciones del PIC esclavo
}