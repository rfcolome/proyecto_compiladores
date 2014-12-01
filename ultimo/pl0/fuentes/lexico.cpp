#include "lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN","BOOLEAN","TRUE","FALSE","RND","CLRSCR","HALT",
  "PITAG","FOR","TO","PROGRAM","INLINE","LIT","OPR","CAR","ALM",
  "LLA","INS","SAL","SAC","REA","IMP","HLT"
};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok,booleantok,truetok,falsetok,rndtok,
  clrscrtok,halttok,pitagtok,fortok,totok,programtok,inlinetok,
  littok,oprtok,cartok,almtok,llatok,instok,saltok,sactok,
  reatok,imptok,hlttok
};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;
