//tds: la tabla de símbolos
#include <string.h>
#include <stdlib.h>
#include "tds.h"
#include "auxiliares.h"
#include "scanner.h"

registro tabla[MAXIT+1];  //tabla de símbolos (+1 porque tabla[0] esta reservada)
int it;                   //índice para recorrer la tabla

//poner: poner un objeto: CONSTANTE, VARIABLE o PROCEDIMIENTO en la tds
void poner(enum objeto k)
{
 ++it;
 //verificar si hay espacio en la tds
 if (it>MAXIT) {
    error(31) ; //error 31 : Limitación del compilador (se rebasó el tamaño de la tds estática)
 }
 else {
    //poner la lexeme en la tds
    strcpy(tabla[it].nombre,lex); 

    //poner los atributos...por el momento solo el tipo de objeto
   tabla[it].tipo  =k; //poner el objeto
 }
}

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es semánticamente legal
//búsqueda en reversa...¿porqué en reversa?
int posicion()
{
 int i;
 strcpy(tabla[0].nombre,lex);
 i=it;

 while ((strcmp(tabla[i].nombre,lex)) !=0)
       --i;

 return(i);
}
