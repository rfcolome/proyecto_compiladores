#include "parametros.h"

//definici�n de la tabla de s�mbolos - organizacion y acceso lineal
//sufrir� modificaciones cu�ndo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {CONSTANTE,VARIABLE,PROCEDIMIENTO}; //definidos aqu� en el encabezado

typedef struct registro{
  char nombre[MAXID+1];
  enum objeto tipo;
  struct registro *siguiente;
  struct registro *anterior;
} registro;

//tabla de s�mbolos
extern registro *tabla;  //MAXIT en parametros (+1 porque tabla[0] esta reservada)
extern registro *finDeTabla;
extern int it;                  //�ndice para recorrer la tabla

void inittds();
void poner(enum objeto k);
int posicion();
