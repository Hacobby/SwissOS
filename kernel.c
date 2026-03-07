#include <stdint.h>
#include "drivers/video.c"
#include "idt.h"
#include "io.h"

// Estructura para guardar el estado de los registros durante una interrupcion
typedef struct {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss; 
} __attribute__((packed)) registers_t;

// Traductor de Scancodes (Set 1) a caracteres ASCII (inglesUS)
const char kbd_US[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,        /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void kernel_main(){
    // direccion de memoria de video
    volatile uint16_t* terminalBuffer = (volatile uint16_t*) VIDEO_ADDRESS;
    char* msj = "Bienvenido a SwissOS\n";

    // Limpiar la pantalla llenando el buffer de video con espacios en blanco
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++){
        terminalBuffer[i] = (uint16_t) ' ' | (uint16_t) WHITE_ON_BLACK << 8;
    }

    // Escribir el mensaje en la pantalla 
    kprint(msj);

    // Inicializar la IDT
    idt_install();

    // Remapear el PIC
    pic_remap();
    __asm__ __volatile__("sti"); // Habilitar interrupciones
    kprint("IDT instalada y PIC remapeado. Interrupciones habilitadas.\n");

    // Comentado para evitar que el sistema se detenga al generar una excepcion de division por cero. 
    // Se puede descomentar para verificar que la IDT esta funcionando correctamente y que las excepciones se manejan adecuadamente.
    //__asm__ __volatile__("int $0x3");

    // Si este mensaje se mostro la IDT fallo.
    // kprint("Bodrio operativo. Sistema en decadecia. -2/10. Queso podrido. Incluso windows es mejor.\n");

    while(1){
        // Loop infinito para mantener el kernel corriendo
    };
}

// Mensajes de excepciones para las interrupciones del 0 al 19
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

// Manejador de interrupciones
void fault_handler(registers_t *registers){
    // Error critico de la CPU (0 al 31)
    if (registers->int_no < 32){
        kprint("ERROR CRITICO EN SISTEMA\n");
        kprint("Excepcion: ");
        kprint(exception_messages[registers->int_no]);
        kprint("\n");

        while(1){
            // halt para evitar que el sistema siga corriendo despues de una excepcion critica
            __asm__ __volatile__("cli; hlt");
        }
    }

    // Interrupcion del teclado (IRQ1)
    if (registers->int_no == 33){
        uint8_t scancode = inb(0x60); // Leer el scancode
        
        if (scancode < 0x80){ // Solo procesar teclas presionadas, no liberadas
            char key = kbd_US[scancode];
            if (key != 0){ // Si el scancode corresponde a un caracter imprimible
                char str[2] = {key, '\0'}; // Convertir el caracter a una cadena de 1 caracter + null terminator
                kprint(str);
            }
        }

        outb(0x20, 0x20); // Enviar EOI al PIC maestro
    }
}