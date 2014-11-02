#include "parametros.h"

//conjuntos de tokens para manejo de errores         
//tokens iniciales de declaración, de instrucción y de factor
extern int tokinidecl[NOTOKENS],tokiniinst[NOTOKENS],tokinifact[NOTOKENS];

//conjunto de sincronización de arranque
extern int set_arranque[NOTOKENS]; 

void inicializa_conjuntos(),init_set(int conjunto[]),copia_set(int conjunto1[],int conjunto2[]);
void union_set(int conjunto1[],int conjunto2[],int conjunto3[]),test (int conjunto1[],int conjunto2[],int n);