//tds: la tabla de s�mbolos
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tds.h"
#include "auxiliares.h"
#include "scanner.h"

registro tabla[MAXIT+1];  //tabla de s�mbolos (+1 porque tabla[0] esta reservada)
registro *lista = NULL;
registro *finLista = NULL;
int it;                   //�ndice para recorrer la tabla

int inicializarTdS () {
  lista = (registro *)malloc(sizeof(registro));
  finLista = lista;
  if (lista == NULL) {
    printf("ERROR: no se pudo reservar suficiente memoria para la tabla de simbolos\n");
    exit(1);
  }
  else {
    (lista -> nombre)[0] = '\0';
    lista -> sig = NULL;
    lista -> prev = NULL;
  }
  return 1;
}
//poner: poner un objeto: CONSTANTE, VARIABLE o PROCEDIMIENTO en la tds
void poner(enum objeto k)
{
  ++it;
  //verificar si hay espacio en la tds
  if (it>MAXIT) {
    error(31) ; //error 31 : Limitaci�n del compilador (se rebas� el tama�o de la tds est�tica)
  }
  else {
    registro *tmp = NULL;
    tmp = (registro *)malloc(sizeof(registro));

    if (tmp) {
      strcpy(tmp -> nombre,lex);
      tmp -> tipo = k;
      tmp -> sig = NULL;
      tmp -> prev = finLista;
      finLista = tmp;
    }
    else {
    printf("ERROR: no se pudo reservar suficiente memoria para la tabla de simbolos\n");
    exit(1);      
    }
  }
}

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es sem�nticamente legal
//b�squeda en reversa...�porqu� en reversa?
int posicion()
{
 int i;
 strcpy(tabla[0].nombre,lex);
 i=it;

 while ((strcmp(tabla[i].nombre,lex)) !=0)
       --i;

 return(i);
}
