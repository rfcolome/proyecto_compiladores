#include "parametros.h"

//definici�n final de la tabla de s�mbolos - organizacion y acceso lineal
//la tabla de s�mbolos es una estructura con una uni�n como miembro

//tipos de objetos de pl0
enum objeto {CONSTANTE,VARIABLE,PROCEDIMIENTO}; //definidos aqu� en el encabezado

enum tipoDato {
  DESCONOCIDO,
  ENTERO,
  REAL,
  BOOLEANO
};

typedef struct {
  int nivel;
  int dir;
} nivel_y_direccion;

typedef struct {
  char nombre[MAXID+1];
  enum objeto tipo;
  enum tipoDato tipoDato;
  union {
    int val;
    nivel_y_direccion nivdir;
  } variante;
} registro;

extern registro tabla[MAXIT+1]; //tabla de s�mbolos (+1 porque tabla[0] esta reservada)
extern int it;                  //�ndice para recorrer la tabla
extern tipoDato tipoActual;

void poner(enum objeto k,int *idat);
void setearTipos(enum tipoDato);
int posicion();
