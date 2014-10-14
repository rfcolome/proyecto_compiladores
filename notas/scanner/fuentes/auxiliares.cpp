//funciones auxiliares del compilador
#include <stdio.h>
#include <stdlib.h>
#include "pl0.h"
#include "auxiliares.h"
#include "lexico.h"

//error: por el momento todo error es fatal          
 int error(int no)
{ 
 printf ("\n^ Error %d: Este numero es demasiado grande",no);
 fclose(fp);//cerrar el programa fuente
 exit(1); //estoy en modo de p�nico...cualquier error es fatal
}

//inicializar_espec
//construcci�n de la tabla de tokens para operadores y s�mboles especiales de pl0
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

//imprime_token: transforma de enumerado a string. no aparecer� m�s en el compilador
void imprime_token()
{

 char *token_string[]={"nulo","ident","numero","mas","menos","por","barra","oddtok","igl","nig","mnr","mei","myr",
	                   "mai","parena","parenc","coma","puntoycoma","punto","asignacion","begintok","endtok","iftok",
					   "thentok","whiletok","dotok","calltok","consttok","vartok","proctok"};
 printf("(%10s) \n",token_string[token]);
  
}