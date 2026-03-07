ENTRY(_start)
SECTIONS
{
    . = 1M;

    .text :
    {
        *(.multiboot)
        *(.text)
    }

    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
