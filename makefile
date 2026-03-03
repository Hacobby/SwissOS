CC = gcc -m32
AS = nasm -f elf32
LD = ld -m elf_i386

all: SwissOS.iso

kernel.bin: boot.o kernel.o
	$(LD) -T linker.ld -o kernel.bin boot.o kernel.o
boot.o: boot.s
	$(AS) boot.s -o boot.o
kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -ffreestanding -O2 -Wall
SwissOS: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	grub2-mkrescue -o SwissOS.iso iso
clean:
	rm -rf *.o kernel.bin SwissOS.iso iso/boot/kernel.bin
run: SwissOS.iso
	qemu-system-i386 -cdrom SwissOS.iso