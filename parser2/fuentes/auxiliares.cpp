//funciones auxiliares del compilador
#include <stdio.h>
#include "auxiliares.h"
#include "pl0.h"
#include "mensajes_de_error.h"
#include "lexico.h"
#include "scanner.h"

//error
//se marcan errores (sin pánico)                   
//en algunos casos la marca de error se "barrera" hasta el siguiente token, pues 
//se sabe que se tiene el error hasta que se encuentra el siguiente token. 
//ejemplo
// ..End                                                            
//		x = 3 ...                                                     
//      ^ Error 5 : Falta un punto y coma                              
 int error(int no)
{ 
 ++no_de_errores;
 printf("%*s^",offset," ");
 printf (" Error %d: %s\n",no,mensaje_de_error[no]);
 return(no_de_errores);
}

//estadísticas: ahora se indican un conteo de errores             
int estadisticas()
{
 printf("\n\n***   Estadisticas globales   ***\n");
 if (no_de_errores==0) 
  printf("***  No se detectaron errores ***");
 else {
  printf("*** %d error(es) detectado(s)  ***\n",no_de_errores);
  printf("*** No se genero el ejecutable ***");
 }
 return(no_de_errores);
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
  espec[58]=dospuntos;
  espec[59]=puntoycoma;
}
