#include "parametros.h"

//definición de la tabla de símbolos - organizacion y acceso lineal
//sufrirá modificaciones cuándo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {CONSTANTE,VARIABLE,PROCEDIMIENTO}; //definidos aquí en el encabezado

typedef struct {
 char nombre[MAXID+1];
 enum objeto tipo;
} registro;

extern registro tabla[MAXIT+1]; //tabla de símbolos (+1 porque tabla[0] esta reservada)
extern int it;                  //índice para recorrer la tabla 

void poner(enum objeto k);
int posicion();