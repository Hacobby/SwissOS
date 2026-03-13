#include "entryAndExit_manager.h"
#include "../drivers/video.h"


//Donde se almecena los dispositivos del sistena
Dispositivo dispositivo[MAX_DISPOSITVOS];

//Guarda cuantos dispositovos estan registrados
int total_dispositivos = 0;


//funcion para inicializar el gestor de entrada y salida
void iniciar_gestor(){
    //Al iniciar el sistema no hay disposivos registrados
    total_dispositivos = 0;
}

//funcion para copiar una cadena de texto a otra 
void copiar_texto(char* destino, char* origen){
    int i = 0;

    //copiar caracter por caracter hasta el final del texto
    while(origen[i] != 0){
        destino[i] = origen[i];
        i++;
    }

    //se agrega el terminador de cadena
    destino[i] = 0;
}


//funcion para Registra un dispositivo nuevo en el sistema
void registrar_dispositivo(char* nombre, char* tipo){

    //mira que no excesa el maximo de dispositivos
    if(total_dispositivos >= MAX_DISPOSITVOS){ 
    return;

    }
    //Asigna un ID al dispositivo
    dispositivo[total_dispositivos].id = total_dispositivos;
   
    //copia el nombre del dispositivo
    copiar_texto(dispositivo[total_dispositivos].nombre, nombre);

    //copia el tipo del dispositivo
    copiar_texto(dispositivo[total_dispositivos].tipo, tipo);

    //Por defecto el dispositivo se registra como activo
    dispositivo[total_dispositivos].activo = 1;
    //por defecto no es principal
    dispositivo[total_dispositivos].principal = 0;

    //aunmenta el contador de dispositvos conectados
    total_dispositivos++;

}


//Funcion para activar un dispositivo
void activar_dispositivo(int id){

    //verifica que el ID exista
    if(id < total_dispositivos){

        //Cambia el estado del a activo
        dispositivo[id].activo = 1;

    }
}

//Funcion para desactivar un dispositivo
void desactivar_dispositivo(int id){

    //verifica que el ID exista
    if(id < total_dispositivos){

        //no permite desactivar el dispositivo principal
        if(dispositivo[id].principal){
            kprint("No se puede desactivar un dispositivo principalm \n");
            return;
        }

        //Cambia el estado a desactivado
        dispositivo[id].activo = 0;
    }
}

//Funcion para establecer dispositivo principal
void establecer_principal(int id){

    //verifica que el dispositivo exista 
    if(id >= total_dispositivos){  
        return;
    }

        
        for(int i = 0; i < total_dispositivos; i++){

            // si es del mismo tipo
            if(dispositivo[i].tipo[0] == dispositivo[id].tipo[0]){

                //Se le quita el estado de principal
                dispositivo[i].principal = 0;
            }
        }

        //Se establece como nuevo estado de principal
        dispositivo[id].principal = 1;
    }


//Funcion para mostrar los dispositivos
void mostrar_dispositivo(){

    kprint("ID   NOMBRE      TIPO      ESTADO       PRINCIPAL\n");
    kprint("--------------------------------------------------\n");

    for(int i = 0; i < total_dispositivos; i++){

        // ID
        //kprint(dispositivo[i].id);
        //kprint("   ");

        // Nombre
        kprint(dispositivo[i].nombre);
        kprint("   ");

        // Tipo
        kprint(dispositivo[i].tipo);
        kprint("   ");

        // Estado
        if(dispositivo[i].activo)
            kprint("Activo   ");
        else
            kprint("Desactivado   ");

        // Principal
        if(dispositivo[i].principal)
            kprint("Si");
        else
            kprint("No");

        kprint("\n");
    }
}

//Menu para mostrar opciones
void mostar_menu(){

    kprint("MENU DE OPCIONES\n");
    kprint("1. Mostrar dispositivos\n");
    kprint("2. Activar dispositivo\n");
    kprint("3. Desactivar dispositivo\n");
    kprint("4. Establecer principal\n");


}



