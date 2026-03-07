; Macro para excepciones sin codigo de error
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    push 0      ; Empujamos un código de error falso (dummy)
    push %1     ; Empujamos el número de interrupción
    jmp isr_common_stub ; Saltamos a la rutina común
%endmacro

; Macro para excepciones con código de error 
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    ; No empujamos 0 porque la CPU ya puso el código de error en la pila
    push %1     ; Empujamos el número de interrupción
    jmp isr_common_stub
%endmacro

; --- GENERACIÓN DE LAS 32 RUTINAS ---
ISR_NOERRCODE 0  ; División por cero
ISR_NOERRCODE 1  ; Debug
ISR_NOERRCODE 2  ; Interrupción no enmascarable
ISR_NOERRCODE 3  ; Breakpoint
ISR_NOERRCODE 4  ; Into (Interrupt on Overflow) detected overflow
ISR_NOERRCODE 5  ; Out of bounds
ISR_NOERRCODE 6  ; Opcode inválido
ISR_NOERRCODE 7  ; Coprocesador no disponible
ISR_ERRCODE   8  ; Doble fallo (¡Con código de error!)
ISR_NOERRCODE 9  ; Coprocessor Segment Overrun
ISR_ERRCODE   10 ; TSS inválido (¡Con código de error!)
ISR_ERRCODE   11 ; Segmento no presente (¡Con código de error!)
ISR_ERRCODE   12 ; Fallo de pila (¡Con código de error!)
ISR_ERRCODE   13 ; Violación de protección general (¡Con código de error!)
ISR_ERRCODE   14 ; Fallo de página (¡Con código de error!)
ISR_NOERRCODE 15 ; Reservado por Intel
ISR_NOERRCODE 16 ; Error de punto flotante
ISR_ERRCODE   17 ; Alignment Check (¡Con código de error!)
ISR_NOERRCODE 18 ; Machine Check
ISR_NOERRCODE 19 ; SIMD Floating-Point Exception
ISR_NOERRCODE 20 ; Virtualization Exception
ISR_ERRCODE   21 ; Control Protection Exception (¡Con código de error!)
ISR_NOERRCODE 22 ; Reservado
ISR_NOERRCODE 23 ; Reservado
ISR_NOERRCODE 24 ; Reservado
ISR_NOERRCODE 25 ; Reservado
ISR_NOERRCODE 26 ; Reservado
ISR_NOERRCODE 27 ; Reservado
ISR_NOERRCODE 28 ; Reservado
ISR_NOERRCODE 29 ; Reservado
ISR_NOERRCODE 30 ; Reservado
ISR_NOERRCODE 31 ; Reservado

extern fault_hanlder

isr_common_stub:
    ; Guardar estado actual de la CPU
    pusha           ; Empuja registros generales (edi, esi, edp, esp, ebx, edx, ecx, eax)
    push ds         ; Empuja el segmento de datos
    push es         ; Empuja el segmento extra
    push fs         ; Empuja el segmento F
    push gs         ; Empuja el segmento G

    ; Cargar segmento de datos del kernel
    mov ax, 0x10    ; Offset en GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; fault_hanlder en C
    call fault_hanlder

    ; Restaurar estado de la CPU (se cargan inversamente)
    pop gs 
    pop fs 
    pop es 
    pop ds 
    popa

    ; Limpiar
    add esp, 8
    iret