#include <stdio.h>
#define TAMLIN 80+1
#define numeroEstados 12
#define numeroColumnas 12

typedef enum {
INCIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT,
ERRORLEXICO
} TOKEN;

TOKEN scanner(char * s);
int obtenerColumna(char s);
int estadoFinal (int estado);

int main ( int argc , char * argv [] ) {

    if (argc == 1) {
        printf ("Debe ingresar el lexema para que sea asignado al TOKEN correspondiente por el programa\n");
        return -1;
    }

    if (argc != 2) {
        printf ("Debe ingresar el numero correcto de argumentos\n");
        return -2;
    }

    scanner ( argv[1] );

    return 0;
}

TOKEN scanner (char * s) {

    int tabla [numeroEstados][numeroColumnas] = {

/*   | Digito | Letra | (  |  ) | ;  |  , | :  |  = | +  |  - | Espacio | Otros |   */

     {   1    ,   2   , 3  , 4  , 5  , 6  , 7  , 11 , 8  , 9  ,   0     ,   11  },
     {   1    ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   2    ,   2   , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 10 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11  }, /* Estado 11: Palabra que no pertenece al lenguaje micro */

    };

    int i = 0;
    int estado = 0;
    int columna;

    while ( (s[i] != '\0') && (estado != 11) ) {

        columna = obtenerColumna (s[i]);
        estado = tabla [estado][columna];

        i++;
    };

    if(!estadoFinal(estado)) {
        printf("ERROR: Esta palabra NO pertenece al lenguaje micro\n");
        return -3;
    }

    switch (estado) {

    case 1: return CONSTANTE;
    case 2: return 2; /* Aca habria que hacer una funcion que distinga un IDENTIFICADOR de INICIO, FIN, LEER, ESCRIBIR */
    case 3: return PARENIZQUIERDO;
    case 4: return PARENDERECHO;
    case 5: return PUNTOYCOMA;
    case 6: return COMA;
    case 8: return SUMA;
    case 9: return RESTA;
    case 10: ASIGNACION;

    }

};

int obtenerColumna (char c) {

    if ( isdigit(c) ) return 0;
    if ( isalpha(c) ) return 1;
    if ( c == '(' ) return 2;
    if ( c == ')' ) return 3;
    if ( c == ';' ) return 4;
    if ( c == ',' ) return 5;
    if ( c == ':' ) return 6;
    if ( c == '=' ) return 7;
    if ( c == '+' ) return 8;
    if ( c == '-' ) return 9;
    if ( isspace(c) ) return 10;
    return 11;

}

int estadoFinal (int estado) {

    if ((estado == 1) || (estado == 2) || (estado == 3) || (estado == 4) || (estado == 5) || (estado == 6) || (estado == 8) || (estado == 9) || (estado == 10)) return 1;
    return 0;

}














































