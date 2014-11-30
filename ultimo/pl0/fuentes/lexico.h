#include "parametros.h"

//lexemes de las palabras reservadas
extern char *lexpal[MAXPAL];

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok,rndtok,clrscrtok,halttok,pitagtok,fortok,totok,
  programtok
}; //definido aquí en el encabezado

extern enum simbolo token;

//tabla de tokens de palabras reservadas
extern enum simbolo tokpal [MAXPAL]; 

//tabla de tokens correspondientes a operadores y símbolos especiales
extern enum simbolo espec[255] ;
