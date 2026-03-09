CC = gcc -m32
AS = nasm -f elf32
LD = ld -m elf_i386

all: SwissOS.iso

kernel.bin: boot.o kernel.o idt.o exceptions.o io.o video.o mem_manager.o
	$(LD) -T linker.ld -o kernel.bin boot.o kernel.o exceptions.o idt.o io.o video.o mem_manager.o

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2 -fno-stack-protector

exceptions.o: exceptions.s
	$(AS) exceptions.s -o exceptions.o

idt.o: idt.c
	$(CC) -c idt.c -o idt.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2 -fno-stack-protector

io.o: io.c
	$(CC) -c io.c -o io.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2 -fno-stack-protector

video.o: drivers/video.c
	$(CC) -c drivers/video.c -o video.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2 -fno-stack-protector

mem_manager.o: modules/MemoryManager/mem_manager.c
	$(CC) -c modules/MemoryManager/mem_manager.c -o mem_manager.o -ffreestanding -O2 -Wall -mno-sse -mpreferred-stack-boundary=2 -fno-stack-protector

SwissOS: kernel.bin
	mkdir -p iso/boot/grub
	mv kernel.bin iso/boot/
	grub2-mkrescue -o SwissOS.iso iso

SwissOS2: kernel.bin
	mkdir -p iso/boot/grub
	mv kernel.bin iso/boot/
	grub-mkrescue -o SwissOS.iso iso

clean:
	rm -rf *.o kernel.bin SwissOS.iso iso/boot/kernel.bin

run: SwissOS
	qemu-system-i386 -cdrom SwissOS.iso -d int -no-reboot -m 512M

run2: SwissOS2
	qemu-system-i386 -cdrom SwissOS.iso -d int -no-reboot -m 512M
