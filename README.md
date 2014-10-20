Proyecto Compiladores
=====================

1. Agregar los comentarios: {comentario}

2. extender la estructura de datos del lenguaje para manejar los tipos ENTERO
y REAL

3. incorporar instrucciones de lectura y escritura:

read(variable)
readln(variable)
write([“cadena de caracteres”,] variable)
writeln(“cadena de caracteres”)
writeln([“cadena de caracteres”,] variable)
writeln(“cadena de caracteres”)

4. Cambiar en el compilador la estructura tipo array de la TDS, por una
estructura dinámica de datos (lista autorreferenciada), modificando en
consecuencia los mecanismos de almacenamiento y búsqueda que sean necesarios.

5. Asegurarse que no se puedan redefinir “objetos” en las declaraciones de un
mismo procedure.

6. Modificar la interfaz gráfica para permitir el uso del scanner y el uso de
la combinación scanner-parser.

7. Los resultados del análisis sintáctico se mostrarán en una "ventana de
resultados" en la interfaz. En el caso de que el análisis sea exitoso, se
mostrará un mensaje análogo al del parser visto en clase. En caso de error, el
análisis se detiene, la línea en el texto donde se ubica el único error será
resaltada con color amarillo y en la ventana de resultados aparecerá el
mensaje de error. Al dar doble clic sobre el mensaje de error el cursor se
posicionará en la línea donde se ha encontrado el error señalado. 8. La opción
de "ayuda" en la interfaz de usuario, mostrará ahora también los diagramas de
sintaxis actualizados y correspondientes a todas las instrucciones que el
lenguaje puede manejar.

9. Se requiere que el analizador sintáctico sea escrito en Visual C++. Ningún
otro lenguaje es elegible.

10. Se requiere que la interfaz con el usuario sea escrita en Visual C++,
Visual Basic, C# o Java. Ningún otro lenguaje es elegible.

11. Además de los puntos anteriores, a cada grupo le será asignada una de 5
variantes de PL0. Cada grupo deberá implementar las especificaciones propias
de la variante asignada. 


PL-4.

1. Extender la estructura de datos de PL-0 para manejar el tipo BOOLEAN y por
supuesto, su uso en un programa. 

2. Aumentar el encabezado de programa PROGRAM de manera optativa

3. Aumentar las instrucción FOR

4. Aumentar la posibilidad de escribir código-p 'inmerso' en el código fuente
de PL-0 usando al instrucción :
INLINE (i1/i2/i3/.../in)
donde 'in' es una instrucción de código-p. (Preguntar)

5. Aumentar las funciones predefinidas: RND,CLRSCR,HALT,PITAG (x,y,z) (que
devuelve true si x,y,z son números pitagóricos y false en caso contrario).





Implementación:

- **Agregar los comentarios: {comentario}**

Los comentarios en el lenguaje cuentan como espacios en blanco.
Estos se manejan en el lexer en scanner.cpp ~30:

//quitar blancos, caracter de cambio de línea y tabuladores
while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;

para agregar comentarios, revisamos si el caracter actual es igual a {, en
cuyo caso seguimos leyendo hasta encontrarnos con }.

Agregamos luego de ese while (scanner.cpp ~32)

 if (ch == '{') {
   // es un comentario. Por lo tanto seguimos leyendo hasta encontrar un }
   while (ch != '}'){
     ch = obtch();
   }
 }


- **extender la estructura de datos del lenguaje para manejar los tipos ENTERO
y REAL**

Para poder hacer esto debemos agregar los tipos a los tipos de lexema
permitidos:
en lexico.h ~7 hay una enumeración de símbolos. El tercero es el numero.
Separamos numero en entero y real:

enum simbolo
{nulo,ident,entero,real,mas,menos,por,barra,oddtok,igl,nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,asignacion,begintok,endtok,iftok,thentok,whiletok,dotok,calltok,consttok,vartok,proctok};
//definido aquí en el encabezado

ya que no existe el numero per se, debemos quitar cualquier referencia a el y
reemplazarla por entero (ya que los numeros viejos tecnicamente eran enteros)

colo@linux:~/Documents/clases/compi/proyecto_compiladores/fuentes> grep -Ril 'numero' ./
./mensajes_de_error.h
./scanner.cpp
./parametros.h
./parser.cpp
./lexico.h

scanner.cp ~74
parser.cpp ~83, 222

ahora la pregunta es: cuando detectamos un numero real?

la manera como se detectan numeros actualmente es en scanner.cpp, ~64:

if (isdigit(ch)) { // podría ser un entero o un real
   lexid[0]=ch;
   i=j=1;
   while ( isdigit( (ch=obtch()))) {
     if (i<MAXDIGIT) lexid[i++]=ch;
     j++;
   }
   lexid[i]='\0';
   if (j>MAXDIGIT)
      error(30); //este número es demasiado grande
   token=entero;
   valor=atol(lexid); //valor numérico de una lexeme correspondiene a un número	        
}

asi que debemos detectar si el numero actual es un punto. Si lo es, seguimos
leyendo y cambiamos el token a ser real en vez de entero:

if (isdigit(ch)) { // podría ser un entero o un real
  int esReal = 0;
  lexid[0]=ch;
  i=j=1;
  while (isdigit((ch=obtch()))) {
    if (i<MAXDIGIT) lexid[i++]=ch;
    j++;
  } // hasta aqui llega a leer el final del entero, o la parte entera del real
  if (ch == '.') {
    esReal = 1; // es un numero real!
    if (i<MAXDIGIT) lexid[i++]=ch; // agregamos el punto al final del lexema
    j++;
    while (isdigit((ch=obtch()))) {
      if (i<MAXDIGIT) lexid[i++]=ch;
      j++;
    }
  }
  lexid[i]='\0';
  if (j>MAXDIGIT)
    error(30); //este número es demasiado grande
  token = (esReal) ? real : entero;
  valor = atof(lexid); //valor numérico de una lexeme
}


por ultimo, reemplazamos cada vez que salga token == numero en parser.cpp
con token == entero || token == real, ya que estos pueden usarse como numeros.



- **incorporar instrucciones de lectura y escritura:**

read(variable)
readln(variable)
write([“cadena de caracteres”,] variable)
writeln(“cadena de caracteres”)
writeln([“cadena de caracteres”,] variable)
writeln(“cadena de caracteres”)


TODO:
esto es tanto a nivel lexico como nivel sintactico.
Por un lado, a nivel lexico se deben identificar como llamadas a functiones.
Por otra parte, las cadenas no son parte del lenguaje, asi que talvez se deban
agregar tambien.
A nivel sintactico, se debe identificar las llamadas a funcion

por otra parte, no es necesario incorporar cadenas de caracteres si se toma
todo como un solo token a nivel lexico...

que opcion deberiamos tomar? la primera es mas dificil pero permitiría agregar
mas funciones facilmente en el futuro.

hay mas funciones predefinidas que debemos incluir en el punto 5:
"Aumentar las funciones predefinidas: RND,CLRSCR,HALT,PITAG (x,y,z) (que
devuelve true si x,y,z son números pitagóricos y false en caso contrario)."

asumo que CLRSCR es "clear screen" y RND es "round".


- **Cambiar en el compilador la estructura tipo array de la TDS, por una
estructura dinámica de datos (lista autorreferenciada), modificando en
consecuencia los mecanismos de almacenamiento y búsqueda que sean
necesarios.**

para hacer esto, debemos:

    - encontrar adonde esta definida la tabla de simbolos

    - cambiar la definición por una lista 

    - crear funciones para buscar/agregar elementos a la lista


la tabla de simbolos esta definida en tds.cpp/h

las funciones para accesar a la tabla de simbolos son:

void poner(enum objeto k);
int posicion();

cree una funcion para inicializar la tabla de simbolos, para que tenga un
primer elemento ya puesto.
