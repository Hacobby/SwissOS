# Linux

Antes de empezar a trabajar en el projecto por favor revisen que tengan los paquetes necesarios para trabajar, puesto que la mayor parte del equipo esta trabajando en linux solo le dare soporte a sistemas linux basados en fedora y debian en este archivo.

#### Fedora

Pueden usar este comando para instalar todos los paquetes necesarios para compilar y probar el codigo:  

```
sudo dnf install gcc nasm make qemu-system-x86 grub2-tools grub2-tools-extra grub2-pc-modules xorriso mtools glibc-devel.i686 libgcc.i68
```

En el projecto hay un archivo makefile, este archivo resume el uso de comandos para compilar, crear la iso y probar el sistema operativo en una maquina virtual, por favor cada que se cree un nuevo archivo .c para codigo en C o .s para ensamblador hay que añadir estos archivos ahi.  

#### Debian

El proceso es bastante similar a las instrucciones en fedora pero algunos paquetes cambian su nombre:

```
sudo apt install gcc nasm make qemu-system-x86 grub-tools grub-tools-extra grub-pc-modules xorriso mtools glibc-devel.i686 libgcc.i68
```

#### Flatpak

Si instalaron Visual Studio Code por medio de flatpak (usualmente la tienda de apps) no usen la terminal integrada de vsCode, flatpak aisla las aplicaciones del resto del sistema por temas de seguridad asi que no sera capaz de leer los paquetes necesarios para compilar y testear. En cambio, pueden ir a la carpeta del repositorio (SwissOS), darle click derecho y darle a la opcion "Abrir terminal aqui", el resto se mantiene igual `make clean` `make run`

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


#### Makefile

Actualmente el archivo makefile del proyecto cuenta con 3 comandos de utilidad:
- `make run`: Se usa para compilar todo el codigo, crear el archivo iso y abrir la maquina virtual en qemu para testing.  
- `make clean`: Usenlo despues de cada cambio en el codigo para limpiar los archivos compilados y la anterior iso generada, de esta forma se evitan errores a la hora de testear los nuevos cambios.  
- `make SwissOS`: Este comando lo pueden usar para compilar y crear la iso unicamente, en caso de que solo necesiten la iso para algo.  
- `make run2`: Lo mismo que `make run`, pero para debian.

# Windows

Windows es un caso especial, como dije en la seccion de linux ya que la mayoria del equipo trabaja en linux no le dare soporte a esta seccion. Si estan trabajando en windows les sugiero investigar sobre WSL (subsistema de linux para windows)