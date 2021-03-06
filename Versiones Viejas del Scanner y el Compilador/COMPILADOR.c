#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define TAMLIN 80+1
#define numeroEstados 12
#define numeroColumnas 12
#define TAMLEX 32

typedef enum {
	INICIO,
	FIN,
	LEER,
	ESCRIBIR,
	ID,
	CONSTANTE,
	PARENIZQUIERDO,
	PARENDERECHO,
	PUNTOYCOMA,
	COMA,
	ASIGNACION,
	SUMA,
	RESTA,
	FDT
} TOKEN;

typedef struct {
	TOKEN clase;
	char nombre[TAMLEX];
	int valor;
} REG_EXPRESION;

void objetivo (void);
void programa (void);
void listaDeSentencias (void);
void sentencia (void);
void expresion (void);
void operadorAditivo (void);

void ErrorSintactico(void);
void ErrorLexico(void);
void ErrorSemantico(void);

int match (char * c);
int scanner(char * s);
int obtenerColumna(char s);
int estadoFinal (int estado);
int strClasificar (char * s);
REG_EXPRESION GenInfijo(REG_EXPRESION e1, char *op, REG_EXPRESION e2);

TOKEN tokenActual;
int flagToken = 0;
FILE * in;
FILE * out;
char buffer[TAMLEX];

typedef struct {
    char lexema [TAMLEX];
    TOKEN token;
    } registroTabla;

 registroTabla tablaDeSimbolos [50] = {
	{"inicio", "INICIO"},
	{"fin", "FIN"},
	{"leer", "LEER"},
	{"escribir", "ESCRIBIR"}
	{"$", "$"}
};

int main ( int argc , char * argv [] ) {



    if (argc == 1) {
        printf ("Debe ingresar los archivos de entrada y salida");
        return -1;
    }

    if (argc != 2) {
        printf ("Debe ingresar el numero correcto de argumentos");
        return -2;
    }

	if ((in = fopen(argv[1],"r") == NULL){
		printf("No se puede abrir archivo de lectura");
		return -3;
	}

	if((out = fopen(,"w")) == NULL){
		printf("No se pudo abrir archivo de salida");
		return -4;
	}
	if (!(terminaCorrecto(argv[1])) {
		printf("El archivo de entrada debe terminar en .m");
		return -5;
	}

	objetivo();
	/*proceso pasar de mini al otro lenguaje*/

	fclose(in);
	fclose(out);
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
			ErrorSintactico();
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
	else ErrorSintactico();
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


int scanner (char * s) {
    int tabla [numeroEstados][numeroColumnas] = {
/*   | Digito | Letra | (  |  ) | ;  |  , | :  |  = | +  |  - | Espacio | Otros | FDT |  */
     {   1    ,   2   , 3  , 4  , 5  , 6  , 7  , 11 , 8  , 9  ,   0     ,   11   ,    },
     {   1    ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   2    ,   2   , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 10 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    },
     {   11   ,   11  , 11 , 11 , 11 , 11 , 11 , 11 , 11 , 11 ,   11    ,   11   ,    }, /* Estado 11: Palabra que no pertenece al lenguaje micro */
    };
    int i = 0;
    int estado = 0;
    int columna;
    while ( (s[i] != '\0') && (estado != 11) ) {
        columna = obtenerColumna (s[i]);
        estado = tabla [estado][columna];
        i++;
    };
    if(!estadoFinal(estado) || i>TAMLEX) {
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
}

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
    if ((estado != 7) && (estado != 11)) return 0;
    return 1;
}

int strClasificar (char * t) {
    char palRes[4] = {"INICIO","FIN","LEER","ESCRIBIR"};
    int i = 0;
    for (i;i<4;i++) {
        if ((strcmp (t,palRes[i])) == 0) return i;
    }
    return ID;
}

void Match( TOKEN t){
	if (!(t == ProximoToken())) ErrorSintactico();
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
void ErrorLexico(void) {
	printf ("Hubo error de tipo lexico");
	return -100;
}
void ErrorSemantico(void) {
	printf ("Hubo error de tipo semantico");
	return -101;
}
void ErrorSintactico(void) {
	printf ("Hubo error de tipo sintactico");
	return -102;
}
