#include "mem_manager.h"
#include "../../drivers/video.h"

void int_to_string(int n, char str[]){
    int i = 0;
    int is_negative = 0;

    if (n == 0){
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (n < 0){
        is_negative = 1;
        n = -n;
    }

    while (n != 0){
        str[i++] = (n % 10) + '0';
        n = n / 10;
    }

    if (is_negative) str[i++] = '-';

    str[i] = '\0';

    // Invertir la cadena
    for (int j = 0; j < i / 2; j++){
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

void mem(uint32_t total_memory){
    char mem_str[16];
    kprint("Memoria total: ");
    int_to_string(total_memory, mem_str);
    kprint(mem_str);
    kprint(" MB\n");
    kprint("Memoria utilizada: 0 MB\n");
    kprint("Memoria libre: ");  
    kprint(mem_str);
    kprint(" MB\n");
}