//tds: la tabla de símbolos
#include <string.h>
#include <stdlib.h>
#include "parametros.h"
#include "tds.h"
#include "pl0.h"
#include "auxiliares.h"
#include "scanner.h"

registro tabla[MAXIT+1];  //tabla de símbolos (+1 porque tabla[0] esta reservada)
int it;                   //índice para recorrer la tabla
tipoDato tipoActual;      // el tipo de la ultima constante escaneada

//poner: poner un objeto: CONSTANTE, VARIABLE o PROCEDIMIENTO en la tds
void poner(enum objeto k,int *idat) {
  ++it;
  //verificar si hay espacio en la tds
  if (it>MAXIT) {
    error(31) ; //error 31 : Limitación del compilador (se rebasó el tamaño de la tds estática)
    estadisticas();
	fclose(fp);
    exit(1); //el error es fatal
  }
  else {
    //poner la lexeme en la tds
    strcpy(tabla[it].nombre,lex); 

    //poner los atributos
    tabla[it].tipo  =k; //poner el objeto
    tabla[it].tipoDato = DESCONOCIDO;
    switch(k) {
    case CONSTANTE:
      if (valor>MAXD) {
        error(35); //error(35): Se rebasa el máximo entero en pl0--se asume cero 
        valor=0;
      }
      
      tabla[it].tipoDato = tipoActual;
      tabla[it].variante.val=valor;
      break;

    case VARIABLE:
      tabla[it].variante.nivdir.nivel=niv;
      tabla[it].variante.nivdir.dir  =*idat;
      ++(*idat);
      break;

    case PROCEDIMIENTO:
      tabla[it].variante.nivdir.nivel=niv; //a "dir" la parchamos en bloque
      break;
    };
  }
}

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es semánticamente legal
//búsqueda en reversa
int posicion() {
  int i;
  strcpy(tabla[0].nombre,lex);
  i=it;

  while ((strcmp(tabla[i].nombre,lex)) !=0)
    --i;

  return(i);
}

//setearTipos: setea los tipos de las ultimas variables
//encontradas cuyo tipo de dato es desconocido
void setearTipos(enum tipoDato tipo) {
  int i = it;
  while (i > 0 && tabla[i].tipoDato == DESCONOCIDO && tabla[i].tipo == VARIABLE) {
    tabla[i].tipoDato = tipo;
    i--;
  }
}
