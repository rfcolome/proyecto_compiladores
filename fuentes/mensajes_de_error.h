//mensajes de error para el compilador de pl0
//parser1 no está diseñado para mostrar todos los errores de la lista
//ésta es la lista de errores del compilador completo


char *mensaje_de_error[]={"",
/*Error 1 */ "usar ' = ' en vez de ' := ' ",
/*Error 2 */ "debe ir seguido de un numero",
/*Error 3 */ "el identificador debe ir seguido de ' = ' ",
/*Error 4 */ "CONST, VAR y PROCEDURE deben ir seguidos de un identificador",
/*Error 5 */ "falta una coma o un punto y coma",
/*Error 6 */ "simbolo incorrecto despues de una declaracion de procedimiento",
/*Error 7 */ "se esperaba una instruccion",
/*Error 8 */ "simbolo incorrecto detras de las instrucciones de un bloque",
/*Error 9 */ "se espera un punto",
/*Error 10*/ "falta un punto y coma entre instrucciones",
/*Error 11*/ "identificador no declarado",
/*Error 12*/ "no estan permitidas las asignaciones a constantes o procedimientos",
/*Error 13*/ "se esperaba el operador de asignacion",
/*Error 14*/ "CALL debe de ir seguido de un identificador",
/*Error 15*/ "no tiene sentido llamar a una constante o a una variable",
/*Error 16*/ "se espera un THEN",
/*Error 17*/ "se espera un END o un punto y coma",
/*Error 18*/ "se espera un DO",
/*Error 19*/ "un simbolo incorrecto sigue a una instruccion",
/*Error 20*/ "se esperaba un operador relacional",
/*Error 21*/ "una expresion no debe de contener un identificador de procedimiento",
/*Error 22*/ "falta un parentesis de cierre",
/*Error 23*/ "el factor anterior no puede ir seguido de este simbolo o falta un punto y coma",
/*Error 24*/ "una expresion no puede empezar con este simbolo",
/*Error 25*/ "",
/*Error 26*/ "",
/*Error 27*/ "",
/*Error 28*/ "",
/*Error 29*/ "",
/*Error 30*/ "este numero es demasiado grande - trunc(5)",
/*Error 31*/ "se rebaso el tamaño de la Tabla de Simbolos",
/*Error 32*/ "el programa fuente esta incompleto",
/*Error 33*/ "el programa fuente es demasiado largo",
/*Error 34*/ "anidamiento demasiado profundo para los procedures",
/*Error 35*/ "se rebasa el maximo entero de pl0--se asume cero"};

/*Errores sintacticos : 1,2,3,4,5,6,7,8,9,10,13,16,17,18,19,20,22,23,24,32 */

/*Errores semanticos  : 11,12,14,15,35*/

/*Limitaciones especificas del compilador : 30,31,33,34*/



    
