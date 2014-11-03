#include "parametros.h"

//definición de la tabla de símbolos - organizacion y acceso lineal
//sufrirá modificaciones cuándo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {CONSTANTE,VARIABLE,PROCEDIMIENTO}; //definidos aquí en el encabezado

typedef struct registro{
  char nombre[MAXID+1];
  enum objeto tipo;
  struct registro *sig;
  struct registro *prev;
} registro;


extern registro tabla[MAXIT+1]; 
extern registro *lista; //tabla de símbolos (+1 porque tabla[0] esta reservada)
extern registro *finLista;
extern int it;                  //índice para recorrer la tabla 

int inicializarTdS();
void poner(enum objeto k);
int posicion();

