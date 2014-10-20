//pl0 versión 3.0-Parser con Tabla de Símbolos (analizador sintáctico)

//septiembre 2012, abril de 2011
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
#include "parser.h"

FILE *fp; //apuntador a archivo conteniendo el programa fuente

//main: inicia el compilador...solo parser
int main (int argc,char *argv[]) { 

 time_t timer;char fecha[LONG_FECHA]; //serviran para imprimir la fecha en el listado de salida

 //verificar si hay archivo fuente
 if (argc!=2)
	printf("\nNo se ha proporcionado el nombre del programa fuente (uso: parser1 progfuente)");
 else { 
	fp=fopen(argv[1],"r"); //abrir el fuente solo para lectura
	if (fp==NULL) 
	   printf("\nNo se encontro el programa fuente indicado");
	else {
	   timer=time(NULL);
	   strcpy(fecha,asctime(localtime(&timer)));
	   printf("\n\nCompilador de pl0 version 3.0/Parser1 --- abril de 2011 --- A2\n");
	   printf("%s - %s\n",argv[1],fecha);

       // inicializacion de tabla de simbolos
       inittds();

	   //inicializacion de tokens de símbolos especiales (en auxiliares.cpp)
	   inicializar_espec() ; 

	   //inicializacion de otras variables (en pl0.h y scanner.h)
	   ch=' ';
	   fin_de_archivo=0;
	   offset=-1;ll=0;
 	   	 
	   //invocar al scanner (en scanner.cpp)
	   obtoken(); 

	   it=0; //inicializamos el índice sobre la tds (it en tds.h)
	   
	   //activación del parser (en parser.h)
	   bloque(); 

	   if (token!=punto)	
		  error(9); //error 9: se esperaba un punto

	   //si llegamos a este punto, no se han detectado errores sintácticos en el programa fuente (estadisticas en auxiliares.cpp)
	   estadisticas();

	   //cerrar el programa fuente
	   fclose(fp);
	}
 }
 return (0);
}


 

