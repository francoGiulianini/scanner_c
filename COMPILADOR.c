#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define TAMLIN 80+1
#define numeroEstados 12
#define numeroColumnas 12
#define TAMLEX 32

FILE * in, out;
char buffer[TAMLEX];

typedef struct{
	TOKEN clase;
	char nombre[TAMLEX];
	int valor;
} REG_EXPRESION;

typedef enum {
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT,
ERRORLEXICO,ERROR
} TOKEN;

TOKEN tokenActual;
int flagToken = 0;

int scanner(char * s);
int obtenerColumna(char s);
int estadoFinal (int estado);
int strClasificar (char * s);

int main ( int argc , char * argv [] ) {


	
    if (argc == 1) {
        printf ("Debe ingresar el programa fuente a compilar\n");
        return -1;
    }

    if (argc != 2) {
        printf ("Debe ingresar el numero correcto de argumentos\n");
        return -2;
    }
	
	if ((in = fopen(argv[1],"r") == NULL){
		printf("No se puede abrir archivo de lectura\n");
		return -3;
	}
	
	if((out = fopen(arg[2],"w")) == NULL){
		printf("No se pudo abrir archivo de salida\n");
		return -4;
	}
	
	return 0;
}	
void Objetivo(void){
		Programa();
		Match(FDT);
}

void Programa(void){
	Match(INICIO);
	ListaSentencias();
	Match(FIN);
}
void ListaSentencias(void){
	Sentencia();
	while(1){
		switch(ProximoToken()){
			case ID : case LEER : case ESCRIBIR:
				Sentencia();
			break;
			default:
			return;
		}
	}	
}

void Sentencia(void){
	TOKEN tok = ProximoToken();
	switch(tok){
		case ID:
			Match(ID); Match(ASIGNACION>);Expresion();Match(PUNTOYCOMA);
		break;
		case LEER:
			Match(LEER); Match(PARENIZQUIERDO); ListaIdentificadores(); Match(PARENDERECHO); Match(PUNTOYCOMA);
		break;
		case ESCRIBIR:
			Match(ESCRIBIR) ; Match(PARENIZQUIERDO); ListaExpresiones(); Match(PARENDERECHO); Match(PUNTOYCOMA);
		break;
		default:
			ErrorSintactico(tok);
		break;
	}
}

void Expresion (void){
	TOKEN t;
	Primaria();
	for(t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken()){
		OperadorAditivo();
		Primaria();
	}
}

void OperadorAditivo(void){
	TOKEN t = ProximoToken();
	if( t == SUMA || t == RESTA) Match(t);
	else ErrorSintactico(t);
}
void Primaria(REG_EXPRESION * Presul){
	TOKEN tok = ProximoToken();
	switch(tok){
		case ID:
			identificador(Presul);
		break;
		case CONSTANTE:
			Match(CONSTANTE);
			*Presul = ProcesarCte();
		break;
		case PARENIZQUIERDO:
			Match(PARENIZQUIERDO);
			Expresion(Presul);
			Match(PARENDERECHO);
		break;
		default: return;
}
}

void ListaIdentificadores(void){
	TOKEN t;
	REG_EXPRESION reg;
	
	identificador(&reg);
	Leer(reg);
	
	for(t=ProximoToken(); t == COMA ; t = ProximoToken()){
		Match(COMA);
		identificador(&reg);
		Leer(reg);
	}
}	
	

	switch (scanner (argv [1])) {

	case INICIO: printf("%s", "El Token es una Palabra reservada."); break;
    case ESCRIBIR: printf("%s", "El Token es una Palabra reservada."); break;
    case LEER: printf("%s", "El Token es una Palabra reservada.") ; break;
    case FIN: printf("%s", "El Token es una Palabra reservada.") ; break;
    case ID: printf("%s", "El Token es un Identificador.") ; break;
    case CONSTANTE: printf("%s", "El Token es una Constante.") ; break;
    case PARENIZQUIERDO: printf("%s", "El Token es un Caracter de puntuacion."); break;
    case PARENDERECHO: printf("%s", "El Token es un Caracter de puntuacion.") ; break;
    case PUNTOYCOMA: printf("%s", "El Token es un Caracter de puntuacion.") ; break;
	case COMA: printf("%s", "El Token es una Caracter de puntuacion.") ; break;
	case ASIGNACION: printf("%s", "El Token es Asignacion.") ; break;
	case SUMA: printf("%s", "El Token es un Operador.") ; break;
    case RESTA: printf("%s", "El Token es un Operador.") ; break;
	case ERROR: printf("ERROR: Esta palabra NO pertenece al lenguaje micro\n"); break;
	}
	
	while(fgets(linea,TIMELIN + 1, in)) fputs(linea,out);
	
	fclose(in);
	fclose(out);
		

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

void Match( TOKEN t){
	if (!t == ProximoToken()) ErrorSintactico();
	flagToken = 0;
}

TOKEN ProximoToken(){
	if (!flagToken){
		tokenActual = scanner();
		if(tokenActual == ERRORLEXICO) ERRORLEXICO;
		flagToken = 1;
		if(tokenActual == ID) Buscar(buffer,TS,&tokenActual);
	}
		return tokenActual;
}
	
void listaDeExpresiones(void){
	TOKEN t;
	REG_EXPRESION reg;
	EXPRESIONES(&reg);
	Escribir(reg);
	
	for(t = ProximoToken();t == COMA; t = ProximoToken()){
		Match(COMA);
		Expresion(&reg);
		Escribir(reg);
	}
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, char *op, REG_EXPRESION e2){
	REG_EXPRESION reg;
	static unsigned int numTemp = 1;
	char cadTemp[TAMLEX] = "Temp&";
	char cadNum[TAMLEX];
	char cadOp[TAMLEX];
	if(op[0] == '-')stpcpy(cadOp,"Restar");
	if(op[0] == '+')strcpy(cadOp,"Sumar");
	sprintf(cadNum,"%d",numTemp);
	numTemp++;
	strcat(cadTemp,cadNum);
	if(e1.clase == ID)Chequear(Extraer(&e1));
	if(e2.clase == ID)Chequear(Extraer(&e2));
	Chequear(cadTemp);
	Generar(cadOp,Extraer(&e1),Extraer(&e2),cadTemp);
	stpcpy(reg.nombre,cadTemp);
	return reg;
}
