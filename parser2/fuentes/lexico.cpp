#include "lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={"BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE","PROGRAM","FOR","IN","TRUE","FALSE","BOOLEAN","INLINE"};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,thentok,vartok,whiletok,programtok,fortok,intok,truetok,falsetok,booleantok,inlinetok};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;

/*    Instrucciones del ensamblador (codigo-p)    */
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC};
typedef struct {
 enum fcn f; /*Mnemotecnico                       */
 int     ni; /*Nivel 0..MAXNIV                    */
 enum tipoDato tdato;
 float     di;
 char    wr[100];
} ensamblador;
 
ensamblador codigo[MAXIC]; /*array con las instrucciones de codigo-p*/
int ic;                    /*indice sobre el array codigo       */
int niv;
char inlines[40];