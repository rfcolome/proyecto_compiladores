#include "lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={"BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE", "PROGRAM","FOR","IN"};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,thentok,vartok,whiletok,programtok,fortok,intok};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;
