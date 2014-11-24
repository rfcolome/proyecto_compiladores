#include "conjuntos.h"
#include "parametros.h"
#include "lexico.h"
#include "auxiliares.h"
#include "scanner.h"

//conjuntos de tokens para manejo de errores         
//tokens iniciales de declaración, de instrucción y de factor
int tokinidecl[NOTOKENS],tokiniinst[NOTOKENS],tokinifact[NOTOKENS];

//conjunto de sincronización de arranque
int set_arranque[NOTOKENS]; 

void inicializa_conjuntos() {
//creación de conjuntos de tokens iniciales
init_set(tokinidecl);
tokinidecl[consttok]=tokinidecl[vartok]=tokinidecl[proctok]=1;
    
init_set(tokiniinst);
tokiniinst[calltok]=tokiniinst[begintok]=tokiniinst[iftok]=tokiniinst[whiletok]=tokiniinst[clrscrtok]=tokiniinst[halttok]=tokiniinst[writetok]=tokiniinst[writelntok]=tokiniinst[readtok]=tokiniinst[readlntok]=1;
   
init_set(tokinifact);
 tokinifact[ident]=tokinifact[entero]=tokinifact[real]=tokinifact[truetok]=tokinifact[falsetok]=tokinifact[parena]=tokinifact[rndtok]=tokinifact[pitagtok]=1;

//creación del set de arranque del parser con los tokens iniciales de declaración+tokens iniciales de instrucción+punto
union_set(set_arranque,tokinidecl,tokiniinst);
set_arranque[punto]=1;
}

//init_set                     
//crear al conjunto vacío
//0 en todas las posiciones del array
void init_set(int conjunto[])
{ 
 int i;
 for (i=0;i<NOTOKENS;++i) 
   conjunto[i]=0;
}

//copia_set
//copia el segundo conjunto en el primero
void copia_set(int conjunto1[],int conjunto2[])
{
 int i;
 for (i=0;i<NOTOKENS;++i)
  conjunto1[i]=conjunto2[i];
}

//union_set
//la unión de conjuntos: conjunto1=conjunto2+conjunto3
void union_set(int conjunto1[],int conjunto2[],int conjunto3[])
{
 int i;
 copia_set(conjunto1,conjunto2);
 for (i=0;i<NOTOKENS;++i)
  if (conjunto3[i]==1)
   conjunto1[i]=1;
}

//test
//rutina de comprobación (test) y posible salto de texto sobre el programa fuente
void test (int conjunto1[],int conjunto2[],int n)
{
 int conj_union[NOTOKENS];

 if (conjunto1[token]==0) { 
  //el token no está en el conjunto1
  error(n); //se marca el error
  //se arma un conjunto de estabilización
  union_set(conj_union,conjunto1,conjunto2);
  //se salta texto de manera "inteligente"
  while (conj_union[token]==0) 
   obtoken();
 }
}
