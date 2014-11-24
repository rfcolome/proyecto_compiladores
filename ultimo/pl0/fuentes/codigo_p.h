#include "parametros.h"

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP,HLT}; //definido en este encabezado

typedef struct {
 enum fcn f; //mnemónico
 int     ni; //nivel (0..MAXNIV)
 int     di; //dirección o desplazamiento (0..32767)
} codigo_intermedio;

extern codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
extern int ic;                          //índice sobre el array de código-p

void gen (enum fcn x,int y,int z),listar_p(),escribe_codigop(char *fuente);
