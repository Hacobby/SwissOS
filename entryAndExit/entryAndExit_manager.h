#ifndef entryAndExit_manager_h
#define entryAndExit_manager_h

//cantidad maxima de dispositivo que el sistema puede manejar
#define MAX_DISPOSITVOS 20

//La estructura que representa un dispositivo de entrada y salida
typedef struct{
    int id;
    char nombre[32];
    char tipo[16];
    int activo;
    int principal;
} Dispositivo;

//Inicializar el gestor
void iniciar_gestor();

//registra un nuevo dispositivo en el sistema
void registrar_dispositivo(char*nombre,char* tipo);

//Activar un dispositivo
void activar_dispositivo(int id);

//Desactivar un dispositivo
void desactivar_dispositivo(int id);

//Establecer un dispositivo principal
void establecer_principal(int id);

//Mostrar los dispositvo registrados
void mostrar_dispositivo();

void mostar_menu();

#endif
