Proyecto Compiladores
=====================

proyecto de compiladores

###Requerimientos

1. seleccionar al menos 15 palabras reservadas del lenguaje.

  > self, resend 
 
2. especificar la expresion regular para definir identificadores.

  > /[a-z_][a-zA-Z0-9_]*/
 
3. especificar expresiones regulares para numeros enteros y numeros reales.

  > Enteros: /([0-9]+)|([0-9][0-9]?e[0-9a-zA-Z]+)/

  > Reales: /([0-9]+\.[0-9]+)|([0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+)/
 
4. seleccionar una lista de operadores y caracteres especiales (al menos 15
entre ambos).

  >  ! @ # $ % ^ & * - + = ~ / ? < > , ; | ' \ 

5. indicar como se construyen comentarios en el lenguaje.

  > "los comentarios se escriben con comillas dobles seguidas de cualquier
  > caracter distinto de comillas, finalizado por comillas dobles"
