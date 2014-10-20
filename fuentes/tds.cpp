//tds: la tabla de s�mbolos
#include <string.h>
#include <stdlib.h>
#include "tds.h"
#include "auxiliares.h"
#include "scanner.h"

registro *tabla = NULL;  //tabla de s�mbolos (+1 porque tabla[0] esta reservada)
registro *finDeTabla = NULL;
int it;                   //�ndice para recorrer la tabla

void inittds() {
  registro *tmp = NULL;
  tmp = (registro *)malloc(sizeof(registro));
  if (tmp) {
    (tmp -> nombre)[0] = '\0';
    tmp -> siguiente = NULL;
    tmp -> anterior = NULL;
    finDeTabla = tmp;
    tabla = tmp;
  }
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
    registro *temp = NULL;
    temp = (registro *)malloc(sizeof(registro));
    if (temp) {
      // pude reservar memoria para el nuevo registro; ahora lo agrego al final de la tabla.
      strcpy(temp -> nombre, lex);
      temp -> tipo = k;
      temp -> anterior = finDeTabla;
      finDeTabla -> siguiente = temp;
      finDeTabla = temp;
      if (tabla == NULL) {
        tabla = finDeTabla;
      }
    }
    else {
      error(31); //error 31: no se pudo reservar memoria? TODO: agregar otro error
    }
  }
}

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es sem�nticamente legal
//b�squeda en reversa...�porqu� en reversa?
int posicion()
{
  registro *iterador = finDeTabla;

  strcpy(tabla->nombre, lex);

  while ((strcmp(iterador -> nombre, lex)) !=0)
    iterador = iterador -> anterior;

  return iterador != tabla; // 1 si el se encontro, o 0 de lo contrario
}
