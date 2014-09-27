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

Estos parametros seran almacenados en un archivo de texto denominado
"param.txt", donde estaran escritos en texto plano con un parametro y un valor
separados por comillas en cada linea.

este archivo de parametros debe ser leido por el analizador lexicografico para
tomar en cuenta estos valores

5. Una opcion del menu de la interfaz debe ser la "Ayuda". en esta opcion se
accedera a un archivo llamado "ayuda.pdf", que contendra la siguiente
información para el usuario:

    * breve historia y aplicaciones del lenguaje asignado
    * palabras reservadas seleccionadas
    * expresion regular para los identificadores
    * expresiones regulares para numeros enteros y numeros reales
    * lista de operadores y caracteres especiales seleccionados
    * forma de construccion de comentarios en el lenguaje
    * un ejemplo de programa para escribir "hola mundo" en el lenguaje
    * cualquier otra observacion/limitacion que se considere necesaria


###Parte 2

Generar un analizador lexicografico para el lenguaje asignado usando Flex.
