MAGIC    equ 0x1BADB002
FLAGS    equ 0x03
CHECKSUM equ -(MAGIC + FLAGS)

global idt_load
extern idtp

section .multiboot
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text

idt_load:
    lidt [idtp]
    ret
    
extern kernel_main
global _start

_start:
    mov esp, stack_space
    call kernel_main
    hlt

section .bss
resb 8192
stack_space:
