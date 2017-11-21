#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define longLexema 32+1          /*longitud maxima permitida de un lexema*/
#define longNombre 35+1          /*longitud maxima del nombre del archivo .m*/
#define cantEstados 15           /*cantidad de estados del AFD del scanner*/
#define cantColumnas 13          /*cantidad de columnas del AFD del scanner*/
#define cantRegTS 250           /*cantidad de registros que hay en la Tabla de Simbolos*/

typedef enum
{
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO,
PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
} TOKEN;


typedef struct
{
char id[longLexema];                /*identificador */
TOKEN t;                            /*valor del token asignado*/
} regTS;

typedef struct
{
 TOKEN tipo;
 char nombre[longLexema];
 int valor;
 } reg_expresion;

regTS tablaSimbolos[cantRegTS] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir",
ESCRIBIR}, {"$", 99} };



char buffer[longLexema];  /*se guardan las cadenas de letras y/o numeros para diferenciar palabras reservadas de identificadores y verificar si ya fueron declarados en la TS*/
FILE* in;                 /*Flujo de Entrada*/
TOKEN tokenActual;        /* variable en la que se guarda el token que se esta usando*/
int flagToken = 0;        /*este flag se encarga de garantizar que no se busque un nuevo token hasta que se haya usado en el analisis sintactico*/
int numTemp = 1;


/* prototipos */
int verificarFormato(char * c);
TOKEN scanner(void);
void objetivo(void);
void programa(void);
void listaSentencias(void);
void sentencia(void);
void listaIdentificadores(void);
void listaExpresiones(void);
void expresion(reg_expresion* e);
void primaria(reg_expresion* p);
void operadorAditivo(char* x);
int estadoFinal(int e);
int columna(int c);
TOKEN proximoToken();
int buscar(char* s, TOKEN* t);
void colocar(char* s);
void chequear(char* s);
void match(TOKEN t);
void errorLexico(void);
void errorSintactico(void);
void generar(char * opCode, char * arg1, char * arg2, char * arg3);
char * extraer(reg_expresion* regTSistro);
void comenzar(void);
void terminar(void);
void leer(reg_expresion in);
void escribir(reg_expresion out);
void asignar(reg_expresion iz, reg_expresion der);
reg_expresion procesarCte(void);
reg_expresion procesarId(void);
char* procesarOp(void);
void identificador(reg_expresion* p);
reg_expresion genInfijo(reg_expresion e1,char* op,reg_expresion e2);

int main(int argc, char * argv[])
{

char fNombre[longNombre];

/*validaciones*/

if ( argc == 1 )
{
    printf("Debe ingresar el archivo fuente\n");
    return -1;
}
if ( argc != 2 )
{
    printf("Numero incorrecto de argumentos\n");
    return -2;
}

strcpy(fNombre, argv[1]);

if (!verificarFormato(fNombre))
{
    printf ("El formato del archivo no es el adecuado (asegurese de que sea un archivo .m)\n");
    return -3;
}

if ( (in = fopen(fNombre, "r") ) == NULL )
{
    printf("No se pudo abrir el archivo\n");
    return -4;
}

/*empieza la ejecucion del compilador*/
objetivo();

/*terminada la ejecucion, cierro el flujo de entrada*/
fclose(in);

return 0;
}

/* esta funcion verifica que el nombre del archivo a compilar termine en .m */
int verificarFormato(char * c)
{
    int i = 0;
    while(c[i] != '\0') i++;
    if ( (c[i-1] == 'm') && (c[i-2] == '.') ) return 1;
    return 0;
}

/* <objetivo> -> <programa> FDT #terminar */
void objetivo(void)
{
  programa();
  match(FDT);
  terminar();
}

/* <programa> -> #comenzar INICIO <listaSentencias> FIN */
void programa(void)
{
    comenzar();
    match(INICIO);
    listaSentencias();
    match(FIN);
}

/* <listasentencias> -> <sentencia> { <sentencia> } */
void listaSentencias(void)
{
  sentencia();
  while(1){
    switch(proximoToken())
    {
       case ID: case LEER: case ESCRIBIR:
        sentencia();
        break;
       default: return;
    }
  }
}

void sentencia(void)
{
    char* ident;
   TOKEN tok = proximoToken();
   reg_expresion izq;
   reg_expresion der;
   switch(tok)
   {
   case ID: /*<sentencia> -> <identificador> := <expresion> #asignar PUNTOYCOMA */
    identificador(&izq); match(ASIGNACION); expresion(&der);asignar(izq,der); match(PUNTOYCOMA);
    break;
   case LEER: /*<sentencia> -> LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA */
    match(LEER); match(PARENIZQUIERDO); listaIdentificadores(); match(PARENDERECHO); match(PUNTOYCOMA);
    break;
   case ESCRIBIR: /*<sentencia> -> ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA */
    match(ESCRIBIR);match(PARENIZQUIERDO); listaExpresiones(); match(PARENDERECHO); match(PUNTOYCOMA);
    break;
   default:
    errorSintactico();break;
   }

}

/* <identificador> -> ID #procesarId */
void identificador(reg_expresion* p)
{
    match(ID);
    *p = procesarId();
}

/*<listaIdentificadores> -> <identificador> #leer(id) { COMA <identificador> #leer(id)} */
void listaIdentificadores(void)
{
    TOKEN t;
    reg_expresion r;
	identificador(&r);
    leer(r);
    for ( t = proximoToken(); t == COMA; t = proximoToken() )
    {
      match(COMA);
      identificador(&r);
      leer(r);
    }
}

/* <listaExpresiones> -> <expresion> #escribir(expresion) {COMA <expresion> #escribir(expresion)} */
void listaExpresiones(void)
{
    TOKEN t;
    reg_expresion r;
    expresion(&r);
    escribir(r);
    for ( t = proximoToken(); t == COMA; t = proximoToken() )
    {
      match(COMA);
      expresion(&r);
      escribir(r);
    }
}

/* <expresion> -> <primaria> {<operadorAditivo> <primaria> #genInfijo} */
void expresion(reg_expresion* resultado)
{
    TOKEN t;
    reg_expresion operandoIzq,operandoDer;
    char op[longLexema];
    primaria(&operandoIzq);
    for(t= proximoToken();t == SUMA || t == RESTA; t= proximoToken())
    {
        operadorAditivo(op); primaria(&operandoDer);
        operandoIzq=genInfijo(operandoIzq,op,operandoDer);
    }
    *resultado=operandoIzq;

}

void primaria(reg_expresion* op)
{
	switch(proximoToken())
	{
		case ID: identificador(op); break;  /* <primaria> -> <identificador>*/
		case CONSTANTE: match(CONSTANTE); *op=procesarCte(); break;/* <primaria> -> CONSTANTE #procesarCte*/
		case PARENIZQUIERDO:     /* <primaria> -> PARENIZQUIERDO <expresion> PARENDERECHO */
		match(PARENIZQUIERDO);
		expresion(op);
		match(PARENDERECHO);
		break;
		default:
        errorSintactico();break;
	}
}

void operadorAditivo(char* x) /* <operadorAditivo> -> SUMA #procesarOp || RESTA #procesarOp */
{
    TOKEN t = proximoToken();
    if(t == SUMA || t == RESTA)
    {
        match(t);
        strcpy(x,procesarOp());
    }else
        errorSintactico();
}

char* procesarOp(void)
{
    return buffer;
}

reg_expresion genInfijo(reg_expresion e1,char* op,reg_expresion e2)
{
    reg_expresion e3;
    char cadTemp[longLexema] ="Temp&";
    char cadNum[longLexema];
    char cadOp[longLexema];
    if ( op[0] == '-' ) strcpy(cadOp, "Resta");
    if ( op[0] == '+' ) strcpy(cadOp, "Suma");
    sprintf(cadNum, "%d", numTemp);
    numTemp++;
    strcat(cadTemp, cadNum);
    if ( e1.tipo == ID) chequear(extraer(&e1));
    if ( e2.tipo == ID) chequear(extraer(&e2));
    chequear(cadTemp);
    generar(cadOp, extraer(&e1), extraer(&e2), cadTemp);
    strcpy(e3.nombre, cadTemp);
    return e3;
}

void match(TOKEN t)/*verifica si el argumento es el proximo token, inicializa flagToken*/
{
    if( t != proximoToken())
        errorSintactico();
    flagToken = 0;
}

TOKEN proximoToken()/*verifica si hay un TokenActual == false -> toma tokenActual y flagToken == true, si tokenActual es ID ->lo busca y lo almacena en tabla*/
{

    if(!flagToken)
    {
        tokenActual = scanner();
        if(tokenActual == ERRORLEXICO)
            errorLexico();
        flagToken = 1;
        if(tokenActual == ID) buscar(buffer,&tokenActual);
    }
    return tokenActual;
}

void chequear(char* s) /* verifica si se encuentra el s en ts = false -> lo almacena y genera en MV*/
{
    TOKEN t;
    if(!buscar(s,&t))
    {
        colocar(s);
        generar("declara",s,"Entera","");
    }
}

int buscar (char*ident, TOKEN* t) /* Determina si un identificador esta en la TS y lo reasigna */
{

int i = 0;
while ( strcmp("$", tablaSimbolos[i].id) )
{
    if ( !strcmp(ident, tablaSimbolos[i].id) )
    {
        *t = tablaSimbolos[i].t;
       return 1;
    }
        i++;
}
return 0;
}

/* Agrega un identificador a la TS al final y le agrega el $ */
void colocar(char * ident)
{
int i = 4;
while ( strcmp("$", tablaSimbolos[i].id) ) i++;
if ( i < cantRegTS ) {
 strcpy(tablaSimbolos[i].id, ident );
 tablaSimbolos[i].t = ID;
 strcpy(tablaSimbolos[++i].id, "$" );
}
}

/*funciones auxiliares semantica*/

/*produce salida a MV*/
void generar(char * codigoOp, char * arg1, char * arg2, char * arg3)
{
   printf("%s, %s, %s, %s \n", codigoOp, arg1, arg2, arg3);
}

/* Retorna la cadena(nombre) del registro semantico */
char * extraer(reg_expresion* punteroreg)
{
return (*punteroreg).nombre;
}

void comenzar(void) /*inicializacion semantica*/
{
    printf("Instrucciones para la maquina virtual\n");
}
void terminar(void)
{
    generar("Finalizar","","","");
}

void asignar(reg_expresion iz,reg_expresion der)
{
    generar("almacena",extraer(&der),iz.nombre,"");
}

void leer(reg_expresion in)
{
    generar("leer",extraer(&in),"Entera","");
}

void escribir(reg_expresion out)
{
    generar("escribir",extraer(&out),"Entera","");
}

/*convierte cadena(buffer) a numero entero y devuelve reg_expresion nuevo*/
reg_expresion procesarCte(void)
{
    reg_expresion t;
    t.tipo = CONSTANTE;
    strcpy(t.nombre, buffer);
    sscanf(buffer,"%d",&t.valor);
    return t;
}

/*declara id chequea buffer y construye reg_expresion nuevo (retorna)*/
reg_expresion procesarId(void)
{
    reg_expresion t;
    t.tipo = ID;
    chequear(buffer);
    strcpy(t.nombre,buffer);
    return t;
}

TOKEN scanner(){


	int tabla[cantEstados][cantColumnas] =
               /*L   D   +   -   (   )   ,    ;  :    =  EOF sp  OTRO */
/*0*/ 	     { { 1,  3 , 5 , 6 , 7 , 8 , 9 , 10, 11, 14, 13, 0,  14 },
/*1*/	       { 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2  },
/*2+ ID*/	   { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*3 */	       { 4 , 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4  },
/*4+ CONST*/   { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*5+ +*/       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*6+ -*/       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*7+ (*/       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*8+ )*/       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*9+ ,*/       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*10+ ;*/      { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*11 */        { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
/*12+ :=*/     { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*13+ FDT*/	   { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
/*14 ERLEX*/   { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 } };

int car;
int col;
int estado = 0; /*estado inicial */
int i = 0;      /*inicializa buffer*/

do
{
  car = fgetc(in);
  col = columna(car);
  estado = tabla[estado][col];
 if ( col != 11 )  /* col 11 son los espacios*/
   {
    buffer[i] = car;
    i++;
   }
}while ( !estadoFinal(estado) && estado != 14); /*14 es de rechazo*/

buffer[i] = '\0'; /*SE COMPLETA LA CADENA*/

switch ( estado )
{
 case 2 : if ( col != 11 )
          {
             ungetc(car, in);
             buffer[i-1] = '\0';
          }
          return ID;
 case 4 : if ( col != 11 )
        {
         ungetc(car, in);
         buffer[i-1] = '\0';
        }
         return CONSTANTE;
 case 5 : return SUMA;
 case 6 : return RESTA;
 case 7 : return PARENIZQUIERDO;
 case 8 : return PARENDERECHO;
 case 9 : return COMA;
 case 10 : return PUNTOYCOMA;
 case 12 : return ASIGNACION;
 case 13 : return FDT;
 case 14 : return ERRORLEXICO;
}
return 0;
}

/* int estadofinal devuelve si es estado final */
int estadoFinal(int e)
{
  if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 )
    return 0; /*falso*/
return 1;
}


/* int columna devuelve el valor de la columna de TT dependiendo el caracter*/

int columna(int c)
{
if ( isalpha(c) ) return 0;
if ( isdigit(c) ) return 1;
if ( c == '+' ) return 2;
if ( c == '-' ) return 3;
if ( c == '(' ) return 4;
if ( c == ')' ) return 5;
if ( c == ',' ) return 6;
if ( c == ';' ) return 7;
if ( c == ':' ) return 8;
if ( c == '=' ) return 9;
if ( c == EOF ) return 10;
if ( isspace(c) ) return 11;
return 12;
}

void errorLexico(void)
{
    printf("Error lexico\n");
}

void errorSintactico()
{
printf("Error Sintactico\n");
}



