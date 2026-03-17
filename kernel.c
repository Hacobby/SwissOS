#include <stdint.h>
#include "drivers/video.h"
#include "modules/MemoryManager/mem_manager.h"
#include "idt.h"
#include "io.h"
#include "multiboot.h"
#include "entryAndExit/entryAndExit_manager.h"

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

uint32_t total_memory_mb = 0; // Variable global para almacenar la memoria total detectada por el kernel

void kernel_main(uint32_t magic, multiboot_info_t* mbd){  
    // Limpiar la pantalla llenando el buffer de video con espacios en blanco
    clear_screen();

    // Verificar GRUB
    kprint("Verificando GRUB...\n");
    if (magic != 0x2BADB002){
        kprint("Error: GRUB no ha cargado el kernel correctamente.\n");
        while(1){ __asm__ __volatile__("cli; hlt"); } // Halt para evitar que el sistema siga corriendo
    }
    kprint("GRUB verificado correctamente.\n");

    // Almacenar la memoria total detectada por el kernel
    if (mbd->flags & 0x1){ // Verificar si el bit 0 del campo flags esta seteado, lo que indica que los campos mem_lower y mem_upper son validos
        total_memory_mb = (mbd->mem_lower + mbd->mem_upper) / 1024; // Calcular la memoria total en MB sumando la memoria baja y alta y dividiendo por 1024 para convertir de KB a MB
    }

    // Escribir el mensaje en la pantalla 
    kprint("Bienvenido a SwissOS\n");

    // Inicializar la IDT
    kprint("Instalando IDT...\n");
    idt_install();
    kprint("IDT instalada correctamente.\n");

    // Remapear el PIC
    kprint("Remapeando PIC...\n");
    pic_remap();
    kprint("PIC remapeado.\n");

    // Habilitar interrupciones
    kprint("Habilitando interrupciones...\n");
    __asm__ __volatile__("sti");
    kprint("Interrupciones habilitadas.\n");

    kprint("SwissOS> "); // Mostrar el prompt

    //Inializar el gestor de entrada y salida
    iniciar_gestor();
    //Registrar dispositivo
    registrar_dispositivo("teclado", "entrada");
    registrar_dispositivo("Mouse","entrada");
    registrar_dispositivo("Monitor","Salida");
    registrar_dispositivo("Vocina","Salida");

    establecer_principal(0);
    establecer_principal(2);

    // Pueden probar codigo aqui mientras se desarrolla la consola shell, por ejemplo:
    // kprint("Hola, mundo!\n"); para imprimir un mensaje, o leer el estado de algun puerto de E/S con inb().

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

char command_buffer[256]; // Buffer para almacenar el comando ingresado por el usuario
int buffer_index = 0; // Numero de caracteres escritos

// Funcion para comparar dos cadenas de texto
int strcmp(char *s1, char *s2){
    int i = 0;
    while (s1[i] == s2[i]){
        if (s1[i] == '\0') return 0; // Si llegamos al final de ambas cadenas y son iguales
        i++;
    }
    return s1[i] - s2[i]; // Devolver la diferencia entre los caracteres
}

void execute_command(){
    command_buffer[buffer_index] = '\0';// Terminar la cadena con un null terminator


    // Si no se escribio nada, no hacer nada
    if (buffer_index == 0) return;
    // Diccionario de comandos, se pueden añadir comandos aqui siguiendo el mismo formato
    if (strcmp(command_buffer, "help") == 0){
        kprint("Comandos disponibles:\nhelp\nping\necho\nclear\ndevices\nmenu\nact\ndes\npri\n");
    }
    else if (strcmp(command_buffer, "ping") == 0){
        kprint("pong\n");
    }
    else if (strcmp(command_buffer, "echo") == 0){
        kprint("Eco.. eco..\n");
    }
    else if (strcmp(command_buffer, "clear") == 0){
        clear_screen();
    }
    else if (strcmp(command_buffer, "mem") == 0){
        mem(total_memory_mb); // Llamar a la funcion de memoria y pasar la variable global para mostrar la memoria total
    }
    else if(strcmp(command_buffer, "devices") ==0){
        mostrar_dispositivo();
    }
    else if(strcmp(command_buffer, "menu") == 0){
        mostar_menu();
    }
    else if(strcmp(command_buffer,"devices") == 0){
        mostrar_dispositivo();
    }
    else if(command_buffer[0]=='a' && command_buffer[1]=='c' && command_buffer[2]=='t'){
    if(command_buffer[3] != ' '){
        kprint("comando correcto: act <ID>\n");
    }
    else if(command_buffer[4] < '0' || command_buffer[4] > '9'){
        kprint("ID invalido\n");
    }
    else{
        int id = command_buffer[4] - '0';
        if(id <= 0 || id > total_dispositivos){
            kprint("ID fuera de rango\n");
        }
        else{
            activar_dispositivo(id-1);
            }
        }
    }
    else if(command_buffer[0]=='d' && command_buffer[1]=='e' && command_buffer[2]=='s'){
    if(command_buffer[3] != ' '){
        kprint("comando correcto: des <ID>\n");
    }
    else{
        int id = command_buffer[4] - '0';
        if(id <= 0 || id > total_dispositivos){
            kprint("ID fuera de rango\n");
        }
        else{
            desactivar_dispositivo(id-1);
        }
    }

    }
    else if(command_buffer[0]=='p' && command_buffer[1]=='r' && command_buffer[2]=='i'){ 
    if(command_buffer[3] != ' ' || command_buffer[4] < '0' || command_buffer[4] > '9'){
        kprint("Uso: pri <ID>\n");
        return;
    }
    int id = command_buffer[4] - '0';
    establecer_principal(id-1);
    }
    else {
        kprint("Comando no reconocido. Escribe 'help' para ver los comandos disponibles.\n");
    }
    buffer_index = 0;
}

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

            if (key == '\b'){ // Retroceso
                if (buffer_index > 0){
                    buffer_index--; // Quitamos el ultimo caracter del buffer
                    kprint("\b"); // Retroceder el cursor
                }
            }
            else if (key == '\n'){ // Enter
                kprint("\n"); // Nueva linea
                execute_command(); // Ejecutar el comando ingresado
                buffer_index = 0; // Reiniciar el buffer para el siguiente comando
                kprint("SwissOS> "); // Mostrar el prompt
            }
            else if (key != 0){ // Caracter normal
                if (buffer_index < 255){
                    command_buffer[buffer_index] = key; // Agregar el caracter al buffer
                    buffer_index++;
                    char str[2] = {key, '\0'}; // Convertir el caracter a una cadena de 1 caracter + null terminator
                    kprint(str);
                }
            }
        }

        outb(0x20, 0x20); // Enviar EOI al PIC maestro
    }
}