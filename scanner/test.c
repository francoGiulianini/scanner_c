#include <stdio.h>
#define TAMLIN 80+1

typedef enum {
INCIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT,
ERRORLEXICO
} TOKEN;

TOKEN scanner(char * s);

int main ( int argc , char * argv [] ) {

    if (argc == 1) {
        printf ("Debe ingresar el lexema para que sea asignado al TOKEN correspondiente por el programa\n");
        return -1;
    }

    if (argc != 2) {
        printf ("Debe ingresar el numero correcto de argumentos\n");
        return -2;
    }

    scanner (argv[1]);

    return 0;
}

scanner (char * s) {



}
