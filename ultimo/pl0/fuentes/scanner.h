#include "parametros.h"

extern char linea[MAXLINEA]; //buffer de líneas
extern int ll;               //contador de caracteres
extern int offset;           //corrimiento en la lectura de los caracteres del programa fuente (se usa en scanner.cpp y auxiliares.cpp)
extern int fin_de_archivo;   //bandera de fin de archivo (se usa en pl0.cpp y scanner.cpp)
extern int ch;               //último caracter leído (se usa en pl0.cpp y scanner.cpp)
extern char lex[MAXID+1];    //último lexeme leído ( +1 para colocar "\0")
extern long int valor;       //valor numérico de una lexeme correspondiente a un entero
extern double valorReal;     //valor numerico de una lexeme correspondiente a un real
extern char valorCad[MAXID+1];    //valor de la última cadena leida (+1 para colocar "\0")

void obtoken();
