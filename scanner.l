

/* 
 *  Ricardo Colomé
 *  
 *
 *  Para compilar (asumiendo que tienen flex):
 *
 *    lex proyecto.l
 *    cc lex.yy.c -o proyectoLex -lfl
 *
 */


%{
#include <stdio.h>
%}

%%
%{
  int cont = 0;
%}

"self"                           printf("%s -> SELFTOK\n"            , yytext);
"resend"                         printf("%s -> RESENDTOK\n"          , yytext);
"_Quit"                          printf("%s -> QUITTOK\n"            , yytext);
"_RunScript"                     printf("%s -> RUNSCRIPTTOK\n"       , yytext);
"_RemoveAllSlots"                printf("%s -> REMOVEALLSLOTSTOK\n"  , yytext);
"_Clone"                         printf("%s -> CLONETOK\n"           , yytext);
"_Mirror"                        printf("%s -> MIRRORTOK\n"          , yytext);
"_Print"                         printf("%s -> PRINTTOK\n"           , yytext);
"_AddSlots:"                     printf("%s -> ADDSLOTSTOK\n"        , yytext);
"_Define:"                       printf("%s -> DEFINETOK\n"          , yytext);
"_AddSlotsIfAbsent:"             printf("%s -> ADDSLOTSIFABSENTTOK\n", yytext);
"_RemoveSlot:"                   printf("%s -> REMOVESLOTTOK\n"      , yytext);
"_Eq:"                           printf("%s -> EQTOK\n"              , yytext);
"_IntAdd:"                       printf("%s -> INTADDTOK\n"          , yytext);
"_IntDiv:"                       printf("%s -> INTDIVTOK\n"          , yytext);

[a-z_][a-zA-Z0-9_]*:             printf("%s -> SMALLKEYWORD\n"       , yytext);
[a-z_][a-zA-Z0-9_]*              printf("%s -> IDENTIFIER\n"         , yytext);
[A-Z][a-zA-Z0-9_]:               printf("%s -> CAPKEYWORD\n"         , yytext);
:[a-z_][a-zA-Z0-9_]*             printf("%s -> ARGUMENTNAME\n"       , yytext);
'[^']*'                          printf("%s -> STRING\n"             , yytext);
\"[^\"]*\"                        /* no mostrar los comentarios */
[0-9]+                           printf("%s -> INTEGER\n"            , yytext);
[0-9][0-9]?[rR][0-9a-zA-Z]+      printf("%s -> INTEGER\n"            , yytext);
[0-9]+\.[0-9]+                   printf("%s -> REAL\n"               , yytext);
[0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+ printf("%s -> REAL\n"               , yytext);

"!"                              printf("%s -> BANGTOK\n"            , yytext);
"@"                              printf("%s -> ATTOK\n"              , yytext);
"#"                              printf("%s -> HASHTOK\n"            , yytext);
"$"                              printf("%s -> DOLLARTOK\n"          , yytext);
"%"                              printf("%s -> PCTTOK\n"             , yytext);
"^"                              printf("%s -> CARATTOK\n"           , yytext);
"&"                              printf("%s -> AMPERSANDTOK\n"       , yytext);
"*"                              printf("%s -> ASTERISKTOK\n"        , yytext);
"-"                              printf("%s -> MINUSTOK\n"           , yytext);
"+"                              printf("%s -> PLUSTOK\n"            , yytext);
"="                              printf("%s -> EQUALTOK\n"           , yytext);
"~"                              printf("%s -> TILDETOK\n"           , yytext);
"/"                              printf("%s -> DIVISIONTOK\n"        , yytext);
"?"                              printf("%s -> QUESTIONTOK\n"        , yytext);
"<"                              printf("%s -> LESSTHANTOK\n"        , yytext);
">"                              printf("%s -> GREATERTHANTOK\n"     , yytext);
","                              printf("%s -> COMMATOK\n"           , yytext);
";"                              printf("%s -> SEMICOLONTOK\n"       , yytext);
"|"                              printf("%s -> VERTBARTOK\n"         , yytext);
"\\"                             printf("%s -> BACKSLASHTOK\n"       , yytext);
"."                              printf("%s -> DOTTOK\n"             , yytext);
"("                              printf("%s -> OPENPARENTOK\n"       , yytext);
")"                              printf("%s -> CLOSEPARENTOK\n"      , yytext);
"["                              printf("%s -> OPENBRACKETTOK\n"     , yytext);
"]"                              printf("%s -> CLOSEBRACKETTOK\n"    , yytext);

[ \n\t]                          /* no mostrar los espacios en blanco */

.                                printf("lexema no reconocido: %s\n", yytext);

%%
