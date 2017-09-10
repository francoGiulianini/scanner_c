#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define TAMLIN 80+1
#define numeroEstados 12
#define numeroColumnas 12

typedef enum {
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT,
ERRORLEXICO,ERROR
} TOKEN;

int scanner(char * s);
int obtenerColumna(char s);
int estadoFinal (int estado);
int strClasificar (char * s);

int main ( int argc , char * argv [] ) {



    if (argc == 1) {
        printf ("Debe ingresar el lexema para que sea asignado al TOKEN correspondiente por el programa\n");
        return -1;
    }

    if (argc != 2) {
        printf ("Debe ingresar el numero correcto de argumentos\n");
        return -2;
    }
	
	switch (scanner (argv [1])) {

	case INICIO: printf("%s", "El Token es una Palabra reservada."); break;
    case ESCRIBIR: printf("%s", "El Token es una Palabra reservada."); break;
    case LEER: printf("%s", "El Token es una Palabra reservada.") ; break;
    case FIN: printf("%s", "El Token es una Palabra reservada.") ; break;
    case ID: printf("%s", "El Token es un Identificador.") ; break;
    case CONSTANTE: printf("%s", "El Token es una Constante.") ; break;
    case PARENIZQUIERDO: printf("%s", "El Token es una Palabra reservada."); break;
    case PARENDERECHO: printf("%s", "El Token es una Palabra reservada.") ; break;
    case PUNTOYCOMA: printf("%s", "El Token es una Palabra reservada.") ; break;
	case COMA: printf("%s", "El Token es una Caracter de puntuación.") ; break;
	case ASIGNACION: printf("%s", "El Token es un Operador.") ; break;
	case SUMA: printf("%s", "El Token es un Operador.") ; break;
    case RESTA: printf("%s", "El Token es un Operador.") ; break;
	case ERROR: printf("ERROR: Esta palabra NO pertenece al lenguaje micro\n"); break;
	}
	


    return 0;
}

int scanner (char * s) {

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

    if(!estadoFinal(estado) || i>32) {
         return ERROR;
    }

    switch (estado) {

    case 1: return CONSTANTE;
    case 2: return (strClasificar (s));
    case 3: return PARENIZQUIERDO;
    case 4: return PARENDERECHO;
    case 5: return PUNTOYCOMA;
    case 6: return COMA;
    case 8: return SUMA;
    case 9: return RESTA;
    case 10: return ASIGNACION;

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

int strClasificar (char * t) {

    char palRes[4][8+1] = {"INICIO","FIN","LEER","ESCRIBIR"};
    int i = 0;

    for (i;i<4;i++) {
        if ((strcmp (t,palRes[i])) == 0) return i;
    }

    return ID;

}