CC = gcc -m32
AS = nasm -f elf32
LD = ld -m elf_i386

all: SwissOS.iso

kernel.bin: boot.o kernel.o idt.o exceptions.o io.o video.o
	$(LD) -T linker.ld -o kernel.bin boot.o kernel.o exceptions.o idt.o io.o video.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2

exceptions.o: exceptions.s
	$(AS) exceptions.s -o exceptions.o

idt.o: idt.c
	$(CC) -c idt.c -o idt.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2

io.o: io.c
	$(CC) -c io.c -o io.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2

video.o: drivers/video.c
	$(CC) -c drivers/video.c -o video.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2

SwissOS: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	grub2-mkrescue -o SwissOS.iso iso

clean:
	rm -rf *.o kernel.bin SwissOS.iso iso/boot/kernel.bin

run: SwissOS.iso
	qemu-system-i386 -cdrom SwissOS.iso -d int -no-reboot