#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(){
    if (send_magic_packet() == EXIT_SUCCESS){
        printf("%Lf", main_send_ping("gooasdgle.it", 3));
        if (main_send_ping("google.it", 3) == 0.0){
            printf("bella raga\n");
        }
    }
    else{
        printf("sucks\n");
    }

}