#include "parametros.h"

//definici�n de la tabla de s�mbolos - organizacion y acceso lineal
//sufrir� modificaciones cu�ndo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {CONSTANTE,VARIABLE,PROCEDIMIENTO}; //definidos aqu� en el encabezado

typedef struct registro{
  char nombre[MAXID+1];
  enum objeto tipo;
  struct registro *sig;
  struct registro *prev;
} registro;


extern registro tabla[MAXIT+1]; 
extern registro *lista; //tabla de s�mbolos (+1 porque tabla[0] esta reservada)
extern registro *finLista;
extern int it;                  //�ndice para recorrer la tabla 

int inicializarTdS();
void poner(enum objeto k);
int posicion();

