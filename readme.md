# Linux

Antes de empezar a trabajar en el projecto por favor revisen que tengan los paquetes necesarios para trabajar,puesto que la mayor parte del equipo esta trabajando en linux solo le dare soporte a sistemas basados en fedora en 
este archivo.

Pueden usar este comando para instalar todos los paquetes necesarios para compilar y probar el codigo:  

```
sudo dnf install gcc nasm make qemu-system-x86 grub2-tools grub2-tools-extra grub2-pc-modules xorriso mtools glibc-devel.i686 libgcc.i68
```

En el projecto hay un archivo makefile, este archivo resume el uso de comandos para compilar, crear la iso y probar el sistema operativo en una maquina virtual, por favor cada que se cree un nuevo archivo .c para codigo en C o .s para ensamblador hay que añadir estos archivos ahi.  

#### Flatpak

Si instalaron Visual Studio Code por medio de flatpak (usualmente la tienda de apps) no usen la terminal integrada de vsCode, flatpak aisla las aplicaciones del resto del sistema por temas de seguridad asi que no sera capaz de leer los paquetes necesarios para compilar y testear. En cambio, pueden ir a la carpeta del repositorio (SwissOS), darle click derecho y darle a la opcion "Abrir terminal aqui", el resto se mantiene igual 'make clean' 'make run'

### Ejemplo
Hipoteticamente creamos un archivo moduloX.c, para que el compilador reconozca e incluya moduloX.c en la iso deben añadirlo a makefile asi:  
```
kernel.bin: boot.o kernel.o idt.o exceptions.o io.o video.o moduloX.o
    $(LD) -T linker.ld -o kernel.bin boot.o kernel.o exceptions.o idt.o io.o video.o moduloX.o

.
.

moduloX.o: moduloX.c
    $(CC) -c moduloX.c -o moduloX.o -ffreestanding -O2 -Wall -mno-sse -mpreffered-stack-boundary 2
```

# Windows

Windows es un caso especial, como dije en la seccion de linux ya que la mayoria del equipo trabaja en linux no le dare soporte a esta seccion. Si estan trabajando en windows les sugiero investigar sobre WSL (subsistema de windows para linux)