//funciones auxiliares del compilador
#include <stdio.h>
#include <stdlib.h>
#include "pl0.h"
#include "auxiliares.h"
#include "mensajes_de_error.h"
#include "lexico.h"


//error: por el momento todo error es fatal  
 int error(int no)
{ 
 fclose(fp);
 printf ("\n^ error %d: %s\n",no,mensaje_de_error[no]);
 exit(1); //por el momento...cualquier error es fatal!
}

//estadísticas: por el momento solo se invocará cuándo el fuente no tenga errores
//sufrirá modificaciones cuándo se presente el compilador completo
int estadisticas()
{
 printf("\n\n***   Estadisticas globales   ***\n");
 printf("***  No se detectaron errores ***");
 return(0);
}

//inicializar_espec
//construcción de la tabla de tokens para operadores y símboles especiales de pl0
void inicializar_espec()
{
  int i;
  for (i=0;i<=254;++i) 
    espec[i]=nulo;

  espec[43]=mas;
  espec[45]=menos;
  espec[42]=por;
  espec[47]=barra;
  espec[40]=parena;
  espec[41]=parenc;
  espec[61]=igl;
  espec[46]=punto;
  espec[44]=coma;
  espec[59]=puntoycoma;
}