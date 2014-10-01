

/* 
 *  Ricardo Colomé
 *  
 *  NOTA: a pesar que en la gramatica estaban 
 *  definidos los números y letras como símbolos 
 *  terminales, me pareció mejor agruparlos en
 *  identificadores o números, pero a decir verdad
 *  no sé si estará bien. Lo que sí es que funciona.
 *  
 *  los espacios en blanco no se muestran como tokens
 *
 *  Para compilar (asumiendo que tienen flex):
 *
 *    lex tarea.l
 *    cc lex.yy.c -o tarea -lfl
 *
 */


%{
#include <stdio.h>
%}

%%
%{
  int cont = 0;
%}
const                 printf(" Elemento de Representación Uniforme: CONST, nil\n\n");
int                   printf(" Elemento de Representación Uniforme: INT, nil\n\n");
procedure             printf(" Elemento de Representación Uniforme: PROCEDURE, nil\n\n");
call                  printf(" Elemento de Representación Uniforme: CALL, nil\n\n");
begin                 printf(" Elemento de Representación Uniforme: BEGIN, nil\n\n");
end                   printf(" Elemento de Representación Uniforme: END, nil\n\n");
if                    printf(" Elemento de Representación Uniforme: IF, nil\n\n");
then                  printf(" Elemento de Representación Uniforme: THEN, nil\n\n");
else                  printf(" Elemento de Representación Uniforme: ELSE, nil\n\n");
while                 printf(" Elemento de Representación Uniforme: WHILE, nil\n\n");
do                    printf(" Elemento de Representación Uniforme: DO, nil\n\n");
read                  printf(" Elemento de Representación Uniforme: READ, nil\n\n");
write                 printf(" Elemento de Representación Uniforme: WRITE, nil\n\n");
odd                   printf(" Elemento de Representación Uniforme: ODD, nil\n\n");
":="                  printf(" Elemento de Representación Uniforme: ASSIG, nil\n\n");
"="                   printf(" Elemento de Representación Uniforme: EQ, nil\n\n");
","                   printf(" Elemento de Representación Uniforme: COM, nil\n\n");
";"                   printf(" Elemento de Representación Uniforme: SEMI, nil\n\n");
"<>"                  printf(" Elemento de Representación Uniforme: NEQ, nil\n\n");
"<"                   printf(" Elemento de Representación Uniforme: LT, nil\n\n");
"<="                  printf(" Elemento de Representación Uniforme: LTE, nil\n\n");
">"                   printf(" Elemento de Representación Uniforme: GT, nil\n\n");
">="                  printf(" Elemento de Representación Uniforme: GTE, nil\n\n");
"+"                   printf(" Elemento de Representación Uniforme: PLU, nil\n\n");
"-"                   printf(" Elemento de Representación Uniforme: MIN, nil\n\n");
"*"                   printf(" Elemento de Representación Uniforme: TIM, nil\n\n");
"/"                   printf(" Elemento de Representación Uniforme: DIV, nil\n\n");
"("                   printf(" Elemento de Representación Uniforme: LPAR, nil\n\n");
")"                   printf(" Elemento de Representación Uniforme: RPAR, nil\n\n");

[a-zA-Z][a-zA-Z0-9]*  {
  printf(" Elemento de Representación Uniforme: IDENT, %i\n", cont); 
  printf(" Elemento de tabla de símbolos: %s, %i\n\n", yytext, cont); 
  cont++;
}
[0-9]+                {
  printf(" Elemento de Representación Uniforme: NUM, %i\n", cont); 
  printf(" Elemento de tabla de símbolos: %s, %i\n\n", yytext, cont); 
  cont++;
}

[ \n\t]               /* no mostrar los espacios en blanco */

.                     printf("lexema no reconocido %s\n\n", yytext);

%%
