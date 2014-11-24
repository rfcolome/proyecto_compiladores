//pl0 versi�n 3.0

//compilador de un paso, no optimizante, para el lenguaje pl0 (programming language 0)
//con generaci�n de codigo intermedio p (c�digo-p simplificado) via "syntax directed translation"
//el compilador genera codigo-p en un archivo de c�digo intermedio de salida con extension .p
//octubre de 2012,abril de 2011
//se us� Microsoft Visual C++ 2010 para escribir este compilador

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
#include "codigo_p.h"
#include "conjuntos.h"
#include "parser.h"

FILE *fp;          //apuntador a archivo conteniendo el programa fuente
int no_de_errores; //contador de errores detectados
int niv;           //nivel de anidamiento de los bloques

//main: inicia el compilador
int main (int argc,char *argv[]) { 

 time_t timer;char fecha[LONG_FECHA]; //serviran para imprimir la fecha en el listado de salida

 //verificar si hay archivo fuente
 if (argc!=2)
	printf("\nNo se ha proporcionado el nombre del programa fuente (uso: pl0 progfuente)");
 else { 
	fp=fopen(argv[1],"r"); //abrir el fuente solo para lectura
	if (fp==NULL) 
	   printf("\nNo se encontro el programa fuente indicado");
	else {
	   timer=time(NULL);
	   strcpy(fecha,asctime(localtime(&timer)));
	   printf("\n\nCompilador de pl0 version 3.0 --- abril de 2011 --- A2\n");
	   printf("%s - %s\n",argv[1],fecha);

	   //inicializacion de tokens de s�mbolos especiales (en auxiliares.cpp)
	   inicializar_espec() ; 

	   //inicializacion de otras variables (en pl0.h y scanner.h)
	   ch=' ';
	   fin_de_archivo=0;
	   offset=-1;ll=0;
	   no_de_errores=0; 

	   //inicializaci�n de conjuntos de estabilizaci�n (en conjuntos.cpp)
	   inicializa_conjuntos();
    	 
	   //invocar al scanner (en scanner.cpp)
	   obtoken(); 

	   it =0; //inicializamos el �ndice sobre la tds (it en tds.h)
	   niv=0; //inicializamos el nivel de anidamiento (niv en pl0.h)
	   ic =0; //inicializamos el �ndice sobre el codigo-p (ic en codigo_p.h)

	   //activaci�n del parser (en parser.h)
	   bloque(set_arranque); 

	   if (token!=punto)	
		  error(9); //error 9: se esperaba un punto

	   //indicar cu�ntos errores hay (si los hubieron) (en auxiliares.cpp)
	   estadisticas();

	   //cerrar el programa fuente
	   fclose(fp);

	   //listar y escribir en disco el c�digo-p resultado de la compilaci�n (en codigo_p.cpp)
	   if (no_de_errores==0) {
		  listar_p();          
		  escribe_codigop(argv[1]); 
	   }
	}
 }
 return (0);
}


 

