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


###Parte 1

1. Se debe sustituir la busqueda lineal del analizador lexicografico por una
busqueda binaria.

2. se deben omitir los comentarios bien formados (los comentarios no son
tokenizados)

3. se escribira ademas una interfaz para el usuario con las siguientes
funciones basicas: abrir un archivo, crear y/o modificar texto, copiar texto,
pegar texto, guardar texto y ejecutar el analizador lexicografico sobre el
archivo abierto. La salida de la ejecucion debe ser el programa fuente (texto)
tokenizado. esta salida se debe mostrar en una ventana "hija" dentro de la
interfaz, en el formato:
    lexeme -> token
    lexeme -> token
    ...
    lexeme -> token

4. la interfaz para el usuario debe ofrecer la posibilidad de configurar los
siguientes parametros para el analizador lexicografico
	- tamaño maximo del buffer de lineas (MAXLINEA)
    - maximo numero de digitos en los enteros (MAXDIGIT)
    - maxima longitud de los identificadores (MAXID)
    - ubicacion fisica del archivo scanner.exe. este archivo contiene el
    codigo ejecutable del analizador lexicografico

este archivo de parametros debe ser leido por el analizador lexicografico para
tomar en cuenta estos valores
