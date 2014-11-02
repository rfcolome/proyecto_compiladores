//pl0 versión 3.0-solo parser (analizador sintáctico)
//el parser ya fue ampliado para tratar los errores de una manera mas adecuada y realista, con las reglas de Wirth:                                             
// 1-regla de las palabras reservadas                    
// 2-don't panic rule                                    
// 3-suposiciones sobre la intención del programador, basadas en la experiencia en el uso del lenguaje   

//abril de 2011,octubre de 2012
//se usó Microsoft Visual C++ 2010 para escribir este parser

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "pl0.h"
#include "parametros.h"
#include "auxiliares.h"
#include "scanner.h"
#include "lexico.h"
#include "tds.h"
#include "conjuntos.h"
#include "parser.h"

FILE *fp;          //apuntador a archivo conteniendo el programa fuente
int no_de_errores; //contador de errores detectados (ya hay un contador de errores!---en pl0.h)

//main: inicia el compilador...solo parser, pero ahora "estabilizado"
int main (int argc,char *argv[]) { 

 time_t timer;char fecha[LONG_FECHA]; //serviran para imprimir la fecha en el listado de salida

 //verificar si hay archivo fuente
 if (argc!=2)
	printf("\nNo se ha proporcionado el nombre del programa fuente (uso: parser2 progfuente)");
 else { 
	fp=fopen(argv[1],"r"); //abrir el fuente solo para lectura
	if (fp==NULL) 
	   printf("\nNo se encontro el programa fuente indicado");
	else {
	   timer=time(NULL);
	   strcpy(fecha,asctime(localtime(&timer)));
	   printf("\n\nCompilador de pl0 version 3.0/Parser2 --- abril de 2011 --- A2\n");
	   printf("%s - %s\n",argv[1],fecha);

	   //inicializacion de tokens de símbolos especiales (en auxiliares.cpp)
	   inicializar_espec() ; 

	   //inicializacion de otras variables (en pl0.h y scanner.h)
	   ch=' ';
	   fin_de_archivo=0;
	   offset=-1;ll=0;
	   no_de_errores=0; 

	   //inicialización de conjuntos de estabilización (en conjuntos.cpp)
	   inicializa_conjuntos();
    	 
	   //invocar al scanner (en scanner.cpp)
	   obtoken(); 

	   //inicializamos el índice sobre la tds (it en tds.h)
	   it =0; 
	  
	   //activación del parser (en parser.h)
	   bloque(set_arranque); 

	   if (token!=punto)	
		  error(9); //error 9: se esperaba un punto

	   //indicar cuántos errores hay (si los hubieron) (en auxiliares.cpp)
	   estadisticas();

	   //cerrar el programa fuente
	   fclose(fp);	   	   
	}
 }
 return (0);
}


 

