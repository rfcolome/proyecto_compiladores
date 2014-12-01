
Ultimo entregable de compiladores

Variante: PL/4.



1. agregar los comentarios {comentario}
---------------------------------------

para que los comentarios se traten como espacio en blanco, vamos
al archivo scanner.cpp.

la linea 30 corresponde a quitar los espacios en blanco.
Despues de ella agregamos lo siguiente:

 // revisamos si inicia un comentario
 if (ch == '{') {
   // de ser asi, seguimos leyendo hasta que termine
   while (ch != '}') {
     ch = obtch();
   }
   ch = obtch(); // primer caracter despues del comentario
   // leemos cualquier espacio en blanco despues
   while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;


con eso integramos los comentarios al lenguaje.










2. Extender la estructura de datos del lenguaje para manejar los
----------------------------------------------------------------
tipo de datos ENTERO y REAL
---------------------------

actualmente los numeros que se manejan son esencialmente
enteros, asi que debemos implementar reales como pascal
asi que basicamente un numero real es

([0-9]+\.[0-9]+)|([0-9]+(\.[0-9]+)?[+-][eE][0-9]+)


los numeros se leen en scanner.cpp, desde la linea 67.
Si queremos agregar reales, debemos eliminar el tipo "numero", y
diferenciar entre ambos. El tipo "numero" es utilizado en
scanner.cpp, conjuntos.cpp (linea 23),
parser.cpp (linea 159 y 408), y se define en lexico.h (linea 7).

por lo tanto, primero lo redefinimos en lexico.h para dividirlos
en entero y real:

enum simbolo {
  nulo,ident,entero,real,mas,menos,por,barra,oddtok,igl,nig,mnr,
  mei,myr,mai,parena,parenc,coma,puntoycoma,punto,asignacion,
  begintok,endtok,iftok,thentok,whiletok,dotok,calltok,consttok,
  vartok,proctok
}; //definido aquí en el encabezado

una vez esta hecho eso, lo cambiamos en los demas archivos:

conjuntos.cpp, linea 22:
init_set(tokinifact);
tokinifact[ident]=tokinifact[entero]=tokinifact[real]=tokinifact[parena]=1;

scanner.cpp ~67
aqui es donde detectamos cuando un lexema es un numero...
Actualmente revisa si el primer caracter es un digito y sigue
leyendo hasta que ya no sean digitos; por lo tanto, cuando
detecta que ya no es un digito, debemos revisar si es un punto o
una letra e o E, y cambiar el token apropiadamente

creamos una nueva variable valorReal en la linea 19:
double valorReal;         //valor numerico de una lexeme correspondiente a un real

lo ponemos tambien en scanner.h para que lo puedan usar otros archivos:

extern double valorReal;     //valor numerico de una lexeme correspondiente a un real




El resultado final es éste:

 else //si comienza con un dígito...
   if (isdigit(ch)) {
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

     // es posible que únicamente se haya leido la parte entera
     // de un número real:
     if (ch == '.') {
       if (i<MAXDIGIT) lexid[i++]=ch;
       j++;
       ch=obtch();
       if (isdigit(ch)) {
         if (i<MAXDIGIT) lexid[i++]=ch;
         j++;
         while ( isdigit( (ch=obtch()))) {
           if (i<MAXDIGIT) lexid[i++]=ch;
           j++;
         }
         lexid[i]='\0';
         if (j>MAXDIGIT)
           error(30); //este número es demasiado grande
         token=real;
         valorReal = strtod (lexid, NULL);
       }
       else {
         // detectamos un punto, pero lo que seguía no era un
         // digito; por lo tanto, esta mal escrito.
         lexid[i] = '\0';
         token=nulo;
       }
     }

     // ahora hemos leido una secuencia de enteros, posiblemente
     // seguidos de un punto y otra secuencia de enteros.
     // ahora viene el exponente, que es la letra e o E, seguida
     // posiblemente de un + o un -, seguido de otra secuencia
     // de digitos.
     if ((ch == 'e' || ch == 'E') && token != nulo) {
       if (i<MAXDIGIT) lexid[i++]=ch;
       j++;
       ch=obtch();
       // despues de la e podría ir un signo
       if (ch == '-' || ch == '+') {
         if (i<MAXDIGIT) lexid[i++]=ch;
         j++;
         ch=obtch();
       }
       // tiene que haber por lo menos un digito en la secuencia
       if (isdigit) {
         if (i<MAXDIGIT) lexid[i++]=ch;
         j++;
         while ( isdigit( (ch=obtch()))) {
           if (i<MAXDIGIT) lexid[i++]=ch;
           j++;
         }
         lexid[i]='\0';
         if (j>MAXDIGIT)
           error(30); //este número es demasiado grande
         token=real;
         valorReal = strtod (lexid, NULL);
       }
       else {
         // detectamos un exponente, pero sin digitos;
         // por lo tanto, esta mal escrito.
         lexid[i] = '\0';
         token=nulo;         
       }
     }
   }

el ultimo es parser.cpp (linea 159 y 408):

en la línea 159 se declara que una declaración de constante
puede tener un número, y en la línea 408 se indica que un factor
puede ser un número. Ahora en ambos casos pueden ser un entero o
un real:

...
if (token==entero || token==real) {
...


ésto debería finalizar la diferenciación entre enteros y reales.
Sin embargo, debemos de agregar los tipos de dato en la
declaracion de variables. Para esto los agregamos a lexpal y
tokpal y luego cambiamos la sintaxis de declaraciones para que
incluyan el tipo igual que Pascal:

en lexico.cpp:

char *lexpal[MAXPAL]={"BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE","integer", "real"};

enum simbolo tokpal [MAXPAL]={begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,thentok,vartok,whiletok,integertok,realtok};

luego los agregamos a la enumeración de símbolos, junto a los dos puntos:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,mas,menos,por,barra,oddtok,igl,nig,mnr,
  mei,myr,mai,parena,parenc,coma,puntoycoma,punto,dospuntos,asignacion,
  begintok,endtok,iftok,thentok,whiletok,dotok,calltok,consttok,
  vartok,proctok,integertok,realtok
}; //definido aquí en el encabezado

y cambiamos el scanner para detectar los dos puntos por si mismos:

          if (ch==':') {
            ch=obtch();
            if (ch=='=') {
              token=asignacion;
              ch=obtch();
            }
            else
              token=dospuntos;

ahora que los dos puntos y las palabras "integer" y "real" son parte
del lexico, debemos agregarlos como parte de la sintaxis:

void declaracionconst(int *idat) {
  if (token==ident) {
    obtoken();
    if (token == dospuntos) {
      obtoken();
      if (token == integertok || token == realtok) {
        obtoken();
        if ( (token==igl) || (token==asignacion) ) {
          //mejora del tipo "adivinación" de la intención del programador
          if (token==asignacion) 
            error(1); //error 1: usar '=" en vez de ":=". es la misma técnica que en C & C++ se usa para if (x=) por if (x==)
          obtoken();
          if (token==entero || token == real) {
            poner(CONSTANTE,&(*idat));
            obtoken();
          }
          else
            error(2) ; //error 2: debe ir seguido de un número
        }
        else
          error (3) ; //error 3: el identificador debe ir seguido de "="
      }
      else
        error (26); // se esperaba el tipo de dato
    }
    else
      error (25); // se esperaban dos puntos
  }
  else
    error(4) ; //error 4: Const, Var y Procedure deben ir seguidos de un identificador
}


con eso agregamos los tipos en las constantes. Ahora falta
agregarlos a las variables. En pascal la sintaxis es
VAR
a, b, c, d, e:integer;
f, g, h, i, j:real;

para eso vamos a la definicion de bloque, en la parte en donde
se estan declarand variables, y agregamos los dos puntos y el
tipo de dato:


    if (token==vartok) {
      obtoken();
      //análogo al "do" de arriba
      do {
        declaracionvar(&idat); //llamada a función que checa que
                               //la variable esté bien declarada
                               //si lo está, se coloca el
                               //identificador en la TDS
        while (token==coma) {
          obtoken();
          declaracionvar(&idat);
        }
        // ahora que estoy agregando tipos de datos al lenguaje,
        // de la misma manera que en PASCAL, la declaracion es
        // VAR
        // a, b, c, d: integer;
        // f, g, h, i: real;
        // por lo tanto, ahorita va el dos puntos y el tipo
        if (token == dospuntos) {
          obtoken();
          if (token == integertok || token == realtok) {
            obtoken();
            if (token==puntoycoma)
              obtoken();
            else
              error(5); //error 5: Falta una coma o un punto y coma
          }
          else
            error(26); // se esperaba el tipo de dato
        }
        else
          error(25); //se esperaban dos puntos
      } while (token==ident);
    }



TODO: revisar si hay que cambiar algo en la maquina virtual para
que haga eso. Probablemente agregarle una nueva columna para
reales...

SI:
al hacer valorReal y ponerlo en el parser para que sea generado

agregar un nuevo parametro al generador de codigo function (gen)
que sea un valor real, y cambiar la maquina virtual para que
haga uso de el

crear un nuevo "registro" en la maquina virtual que guarde
numeros reales.

por otra parte, los "float" y los "long int" en C tienen el
mismo tamaño. Podemos guardarlos en el mismo lugar, e
interpretarlos de diferente manera, dependiendo del uso?




ahora por ultimo, debemos de cambiar los registros de la maquina
virtual de manera que acepten un numero real como un cuarto
argumento:

en codigo_p.h:


typedef struct {
  enum fcn f; //mnemónico
  int     ni; //nivel (0..MAXNIV)
  int     di; //dirección o desplazamiento (0..32767)
  float    r; // numero real
} codigo_intermedio;

extern codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
extern int ic;                          //índice sobre el array de código-p

void gen (enum fcn x,int y,int z,float r),listar_p(),escribe_codigop(char *fuente);


en codigo_p.cpp

...
  //generar una instrucción de código-p
  codigo[ic].f  = x;
  codigo[ic].ni = y;
  codigo[ic].di = z;
  codigo[ic].r  = r;
...
  for(i=0;i<ic;++i) {
    printf("\n %4d  %3s %5d %5d %5f %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,codigo[i].r,comentario[codigo[i].f]);
  }
...


y debemos de cambiar todos los usos de la funcion gen en
parser.cppde manera que hagan uso de este cuarto argumento.

luego de esto debemos cambiar la maquina virtual para que
tambien posean el valor de r:

en pl0mv.cpp:

...
typedef struct {
  enum fcn f; //mnemónico
  int     ni; //nivel (0..MAXNIV)
  int     di; //dirección o desplazamiento (0..32767)
  float     r; //numero real
} codigo_intermedio;
...
    printf("\n\nejecutando la instruccion %4d: %3s %5d %5d %5.2f",d-1,mnemonico[i.f],i.ni,i.di,i.r); 
...
    printf("\n %4d  %3s %5d %5d %5f %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,codigo[i].r,comentario[codigo[i].f]);
...



una vez tenemos estos, hay que considerar como se mandaran y
como se procesaran los numeros en la maquina virtual.
Primeramente, se utilizaran en las operaciones ssi el numero
entero es igual a cero y el numero real correspondiente es
notablemente distinto de cero. (i.e. mayor o igual a 0.001)

agregamos otro stack para reales en pl0mv.cpp:

int p[LONGSTACK]  ; //memoria de datos-stack
int pr[LONGSTACK] ; //memoria de datos-stack de numeros reales


luego agregamos:

 p[0]=pr[0]=p[1]=pr[1]=p[2]=pr[2]=0;   //ED,EE y DR para el programa principal





3. Incorporar las instrucciones de lectura y escritura
------------------------------------------------------

read(variable)
readln(variable)
write(["cadena de caracteres",] variable)
write("cadena de caracteres")
writeln(["cadena de caracteres",] variable)
writeln("cadena de caracteres")

cuando la instruccion termina con el sufijo "ln", se salta a la
siguiente linea en el dispositivo de entrada o salida luego de
efectuar la operación



para hacer esto, primero declaramos read, readln, write, y
writeln, y el tipo de dato cadena como tokens por si mismos:

en lexico.h y lexico.cpp:


//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok
}; //definido aquí en el encabezado



char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN"
};

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok
};


una vez se han establecido como palabras reservadas del
lenguaje, definimos tambien las cadenas como parte del lenguaje:


al final de la funcion instruccion:

  else if (token == readtok || token == readlntok) {
    obtoken();
    if (token == parena) {
      obtoken();
      if (token == ident) {
        i=posicion();
        if (i==0) 
          error(11); //error 11: Identificador no declarado
        else if (tabla[i].tipo != VARIABLE)
          error(28); // se esperaba una variable
        else {
          obtoken();
          if (token == parenc)
            obtoken();
          else
            error(22); // falta un parentesis de cierre
        }
      }
    }
    else
      error(27); // se esperaba un parentesis de apertura
  }

gen(REA,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);

codigo_p.h:
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA}; //definido en este encabezado

codigo_p.cpp:
 char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC", "REA"};
 char *comentario[]={";cargar una constante",";operacion aritmetica, relacional o retornar",";cargar una variable",
                     ";almacenamiento/instruccion de asignacion",";llamada a procedimiento",";asignacion de espacio de memoria",
                     ";salto incondicional",";salto condicional",";instruccion de lectura"};




finalmente, para hacer funcionar el READ y READLN,
debemos agregarlo a pl0mv.cpp:

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA};
...
//arrays para mostrar como string el mnemónico de la instrucción
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC","REA"};
...
  ";salto condicional",
  ";instruccion de lectura"
};
...
          case REA:
               printf("\nREA : leyendo un numero en la direccion %d (s en %d)",p[base(i.ni,b)+i.di],s);
               printf("\n");
               scanf("%d",&(p[base(i.ni,b)+i.di]));
               while (getchar() != '\n');  // limpiamos el buffer
               break;

la diferencia entre READ y READLN es que el READLN inserta una
nueva linea despues de leer. Para esto necesitamos las
instrucciones de escritura.





lo agregamos al final de las instrucciones:

  else if (token == writetok || token == writelntok) {
    obtoken();
    if (token == parena) {
      obtoken();
      // despues del parentesis se pueden poner una cadena de
      // caracteres seguido de una variable opcional, o
      // posiblemente solo la variable (i.e. uno o el otro)
      if (token == cadena) {
        // debemos generar el codigo para imprimir la cadena de
        // caracteres.
        int cadInd = 0;
        while (valorCad[cadInd] != '\0') cadInd++;
        while (cadInd >= 0) {
          // ponemos la cadena en la pila caracter por caracter,
          // pero al reves
          gen(LIT,0,valorCad[cadInd]);
          cadInd--;
        }
        gen(IMP,0,0); // el 0 es para cadenas
        obtoken();
        if (token == coma) {
          obtoken();
          if (token == ident) {
            //ve a buscarlo a la tabla de símbolos
            i=posicion();
            if (i==0)
              error(11); //error 11: identificador no declarado 
            else if (tabla[i].tipo != VARIABLE)
              error(28); // se esperaba una variable
            else {
              // cargamos la avariable al tope de la pila
              gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
              // luego la mostramos en pantalla
              gen(IMP,0,1); // el 1 es para enteros

              obtoken();
              if (token == parenc)
                obtoken();
              else
                error(22); // falta un parentesis de cierre
            }
          }
        }
        else if (token == parenc)
          obtoken();
        else
          error(22); // falta un parentesis de cierre
      }
      else if (token == ident) {
        //ve a buscarlo a la tabla de símbolos
        i=posicion();
        if (i==0)
          error(11); //error 11: identificador no declarado 
        else if (tabla[i].tipo != VARIABLE)
          error(28); // se esperaba una variable
        else {
          // cargamos la avariable al tope de la pila
          gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
          // luego la mostramos en pantalla
          gen(IMP,0,1); // el 1 es para enteros
          
          obtoken();
          if (token == parenc)
            obtoken();
          else
            error(22); // falta un parentesis de cierre
        }
      }
      else
        error(28); // se esperaba una variable
    }
    else
      error(27); // se esperaba un parentesis de apertura
  }


para poderagregar el codigo para esto
debemos poder representar cadenas dentro de la
maquina virtual, y debemos hacer un printf sobre estas cadenas
una cadena es simplemente una secuencia de caracteres, y un
caracter simplemente es un numero. Por lo tanto, puedo cargar una
cadena como si estuviera cargando un monton de numeros.

para hacer esto, podemos guardar la cadena caracter por caracter
simplemente como una secuencia de numeros terminados por el
caracter nulo, luego de lo cual estos se borran de memoria;
luego insertamos la variable en memoria y la imprimimos (debemos
poder diferenciar entre enteros, reales, y booleanos


gen(IMP,0,0) imprime el numero al tope de la pila, interpretado
como un caracter.
gen(IMP,0,1) imprime el numero al tope de la pila, interpretado
como un entero

codigo_p.h:

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP}; //definido en este encabezado


codigo_p.cpp:

  char *mnemonico[]={
    "LIT",
    "OPR",
    "CAR",
    "ALM",
    "LLA",
    "INS",
    "SAL",
    "SAC",
    "REA",
    "IMP"
  };
  
  char *comentario[]={
    ";cargar una constante",
    ";operacion aritmetica, relacional o retornar",
    ";cargar una variable",
    ";almacenamiento/instruccion de asignacion",
    ";llamada a procedimiento",
    ";asignacion de espacio de memoria",
    ";salto incondicional",
    ";salto condicional",
    ";instruccion de lectura"
    ";instruccion de impresión en pantalla"
  };



y finalmente lo agregamos a pl0mv.cpp:

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP};
...
//arrays para mostrar como string el mnemónico de la instrucción
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC","REA","IMP"};

//comentarios para una instrucción de codigo-p
char *comentario[]={
  ";cargar una constante",
  ";operacion aritmetica, relacional o retornar",
  ";cargar una variable",
  ";almacenamiento/instruccion de asignacion",
  ";llamada a procedimiento",
  ";asignacion de espacio de memoria",
  ";salto incondicional",
  ";salto condicional",
  ";instruccion de lectura",
  ";instruccion de impresión en pantalla"
};
...
          case IMP:
               printf("\nIMP : imprimiendo un dato en el tope de la pila (s en %d)",s);
               switch(i.di) {
               case 0: // cadena de caracteres
                 printf("\n");
                 while (p[s] != '\0') {
                   printf("%c",p[s]); // imprimo la cadena caracter por caracter
                   s--;
                 }
                 s--;
                 break;
               case 1: // entero
                 printf("\n%d",p[s]);
                 s--;
                 break;
               };
               break;














4. Cambiar en el compilador la estructura tipo array de la TDS,
---------------------------------------------------------------
por una estructura dinámica de datos (lista autoreferenciada),
--------------------------------------------------------------
modificando en consecuencia los mecanismos de almacenamiento y
--------------------------------------------------------------
busqueda que sean necesarios.
-----------------------------

**** no requiere de cambiar el codigo-p ****









5. Asegurese que no se puedan redefinir "objetos" en las
--------------------------------------------------------
declaraciones de un mismo procedure
-----------------------------------

**** no requiere de cambiar el codigo-p ****






6. Extender la estructura de datos de PL-0 para manejar el tipo
---------------------------------------------------------------
BOOLEAN y por supuesto, su uso en un programa.
----------------------------------------------

NOTA: el codigo-p de true puede ser 1, y de false 0

para esto seguimos mas o menos el mismo procedimiento que al
agregar reales:
primero los agregamos a nivel lexico, luego a nivel sintactico,
y finalmente a nivel de codigo intermedio:

a nivel lexico, debemos agregar las palabras reservadas
"boolean", "true", y "false", con tokens
booleantok, truetok, y falsetok, respectivamente:

en lexico.h:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok
}; //definido aquí en el encabezado


en lexico.cpp:

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN","BOOLEAN","TRUE","FALSE"
};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok,booleantok,truetok,falsetok
};



en parametros.h:

#define MAXPAL     20  //numero de palabras reservadas


con eso ya los tenemos como parte del lexico. Ahora falta
agregarlos en la sintaxis. "boolean" es un tipo de dato, por lo
que puede ir en el mismo lugar que integer or real:

en parser.cpp:

...
          if (token == integertok || token == realtok || token == booleantok) {
...
      if (token == integertok || token == realtok || token == booleantok) {
...

finalmente, un literal booleano puede ir en cualquier lugar que
podria ir un numero entero (al ser usado en expresiones
aritmeticas, true se interpretará como 1, y false como 0.

para esto, debemos cambiar el scanner para asignar en "valor" el
valor correspondiente cuando se detectan estos tokens:

en scanner.cpp:

    if (ok==1) {
      token=tokpal[j]; //es palabra reservada
      if (token == truetok || token == falsetok) {
        valor = (token == truetok);
      }
    }


y de vuelta en parser.cpp:

...
          if (token==entero || token == real || token == truetok || token == falsetok) {
            poner(CONSTANTE,&(*idat));
            obtoken();
          }
...
    else if (token==entero || token==real || token == truetok || token == falsetok) {
      if (valor>MAXD) {
        error(35); //error(35): Se rebasa el máximo entero de pl0--se asume cero
        valor=0;
      }
      if (token == entero || token == truetok || token == falsetok) {
        gen(LIT,0,valor);
      }
      else {
        gen(LIT,0,valorReal);
      }
      obtoken();
    }


y con eso ya no es necesario cambiar la generación de código,
porque los booleanos se tratan igual que los numeros enteros.


7. Aumentar el encabezado del programa PROGRAM de manera
--------------------------------------------------------
optativa
--------

**** no requiere de cambiar el codigo-p ****


encabezado PROGRAM.

basicamente, el programa empieza opcionalmente con
PROGRAM ident;

para hacer esto, primero agregamos PROGRAM como parte del lexico
en lexico.h y lexico.cpp:

lexico.h:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok,rndtok,clrscrtok,halttok,pitagtok,fortok,totok,
  programtok
}; //definido aquí en el encabezado


luego en lexico.cpp:



//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN","BOOLEAN","TRUE","FALSE","RND","CLRSCR","HALT",
  "PITAG","FOR","TO","PROGRAM"
};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok,booleantok,truetok,falsetok,rndtok,
  clrscrtok,halttok,pitagtok,fortok,totok,programtok
};


le agregamos uno a MAXPAL en parametros.h

#define MAXPAL     27  //numero de palabras reservadas


con esto, el string PROGRAM ya se reconoce como un token
individual.

Luego de eso, se agrega a la sintaxis. Sin embargo, no se agrega
al archivo parser.cpp, ya que esto es unicamente para el
programa como un todo, no para el bloque. Por lo tanto se agrega
al archivo pl0.cpp, antes de hacer la llamada a bloque

      if (token == programtok) {
        // esta el encabezado del programa
        obtoken();
        if (token == ident) {
          obtoken();
          if (token == puntoycoma) {
            obtoken();
          }
          else
            error(5); // falta una coma o punto y coma
        }
        else
          error(4); // PROGRAM debe ir seguido de un identificador
      }


y con eso ya esta el encabezado del programa







8. Aumentar la instruccion FOR
------------------------------

for i:= 1 to 10 do writeln(i);

NOTA: el codigo-p puede ser similar al while, con la unica
diferencia que primero inicializamos la variable especificada,
luego revisamos que la variable sea menor a la ultima constante
(10), luego generamos el codigo-p de lo de adentro, luego le
sumamos uno a la variable, y finalmente hacemos un SAL
(salto incondicional) a donde revisamos la condicion


para hacer el for, debemos agregar varios tokens nuevos al
lenguaje: primeramente la palabra FOR, y la palabra TO

para eso, lo agregamos a lexico.h:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok,rndtok,clrscrtok,halttok,pitagtok,fortok,totok
}; //definido aquí en el encabezado


luego los agregamos a lexico.cpp:

char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN","BOOLEAN","TRUE","FALSE","RND","CLRSCR","HALT",
  "PITAG","FOR","TO"
};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok,booleantok,truetok,falsetok,rndtok,
  clrscrtok,halttok,pitagtok,fortok,totok
};

y debemos aumentar MAXPAL en dos para poder agregarlos.
En parametros.h:

#define MAXPAL     26  //numero de palabras reservadas

con eso, FOR y TO ya son parte del lexico del lenguaje; lo
siguiente es agregarlos como parte de la sintaxis.
Como el FOR es similar al WHILE, sería parte de la funcion
instruccion (parser.cpp ~224).

La sintaxis es:


for i:= 1 to 10 do writeln(i);


  else if (token==fortok) {
    obtoken();

    copia_set(setpaso,toksig);
    setpaso[dotok]=setpaso[totok]=1;//setpaso=dotok+toksig

    // despues del for va una asignacion:
    if (token==ident) {
      //ve a buscarlo a la tabla de símbolos
      i=posicion();
      if (i==0)
        error(11); //error 11: identificador no declarado 
      else if (tabla[i].tipo != VARIABLE)
        error(12); //error 12: no están permitidas las asignaciones a constantes o a procedimientos
      obtoken();
      if (token==asignacion)
        obtoken();
      else
        error(13) ; //error 13: se esperaba el operador de asignación 

      copia_set(setpaso,toksig);
      setpaso[dotok]=setpaso[totok]=1;//setpaso=dotok+toksig
      expresion(setpaso);
      //generar código-p
      gen(ALM,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);

      ic1=ic; //recordar en que lugar del codigo estamos
      if (token==totok) { // FOR <asignacion> TO <expresion>...
        obtoken();
        // internamente, la manera como el FOR debe funcionar es
        // - evaluar la asignacion
        // - evaluar que la variable de la asignacion sea menor
        //     o igual a la expresion <expresion>
        // - hacer un salto condicional (SAC)
        // - evaluar la instruccion que sigue al DO
        // - hacer un salto incondicional (SAL) al lugar donde
        //     se evalua la condicion

        // cargamos la variable al tope de la pila
        gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
        copia_set(setpaso,toksig);
        setpaso[dotok]=1;
        expresion(setpaso);
        // ahora el resultado de la expresion esta al tope de la pila
        // y la variable esta justo abajo
        // asi que evaluamos que la variable sea menor o igual
        // al resultado de la expresion
        gen(OPR,0,13); // operacion <=

        ic2=ic; //recordar este lugar
        gen(SAC,0,0); //nivel y dir son "paja". Luego parcharemos aqui

        if (token==dotok) 
          obtoken();
        else
          error(18); //error 18: Se esperaba un "DO" 

        copia_set(setpaso,toksig);
        instruccion(setpaso);

        // se le suma uno a la variable
        gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
        gen(LIT,0,1);
        gen(OPR,0,2);
        gen(ALM,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);

        //aqui backpatching
        gen(SAL,0,ic1);
        codigo[ic2].di=ic;
      }
      else
        error(29); // se esperaba un TO
    }
    else 
      error(28); // se esperaba una variable
  }









9. Aumentar la posibilidad de escribir codigo-p 'inmerso' en el
---------------------------------------------------------------
codigo fuente de PL-0 usando la instrucción INLINE
--------------------------------------------------


para hacer el INLINE, primero agregamos la palabra y todas las
palabras del codigo-p como palabras reservadas en el lenguaje:

en lexico.h:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok,rndtok,clrscrtok,halttok,pitagtok,fortok,totok,
  programtok,inlinetok,littok,oprtok,cartok,almtok,llatok,
  instok,saltok,sactok,reatok,imptok,hlttok
}; //definido aquí en el encabezado


en lexico.cpp:

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

y cambiamos MAXPAL para agregarle todas estas nuevas palabras
reservadas:

#define MAXPAL     39  //numero de palabras reservadas
...
#define NOTOKENS   61  //número de tokens en el lenguaje


al hacer esto, cada una de estas palabras es reconocible como
tokens individuales en el lenguaje. Luego se debe agregarlas a
parser.cpp:

  else if (token == inlinetok) {
    obtoken();
    if (token == parena) {
      
      copia_set(setpaso,vacio);
      setpaso[littok]=setpaso[oprtok]=setpaso[cartok]=
        setpaso[almtok]=setpaso[llatok]=setpaso[instok]=
        setpaso[saltok]=setpaso[sactok]=setpaso[reatok]=
        setpaso[imptok]=setpaso[hlttok]=1;
      do {
        obtoken();
        test(setpaso,toksig,36); //¿codigo mnemonico no reconocido?
        if (token == littok ||
            token == oprtok ||
            token == cartok ||
            token == almtok ||
            token == llatok ||
            token == instok ||
            token == saltok ||
            token == sactok ||
            token == reatok ||
            token == imptok ||
            token == hlttok ) {
          enum fcn mnemonico = tokenAMnemonico(token);
          obtoken();
          if (token == entero) {
            int arg1 = valor;
            obtoken();
            if (token == entero) {
              int arg2 = valor;
              obtoken();
              if (token == entero || token == real) {
                float arg3;
                if (token == entero) {
                  arg3 = valor;
                }
                else {
                  arg3 = valorReal;
                }
                gen(mnemonico,arg1,arg2,arg3);
                obtoken();
              }
              else
                error(2); // debe ir seguido de un numero
            }
            else
              error(2); // debe ir seguido de un numero
          }
          else
            error(2); // debe ir seguido de un numero
        }
        else
          error(36);
      } while (token == coma);
      
      if (token == parenc) {
        obtoken();
      }
      else
        error(22); // falta un parentesis de cierre
    }
    else
      error(27); // se esperaba un parentesis de apertura   
  }


y con eso ya tenemos la funcion INLINE.


10. Aumentar las funciones predefinidas: RND, CLRSCR, HALT,
-----------------------------------------------------------
PITAG(x,y,z)
------------

para RND llamamos a random(),
para CLRSCR...
para HALT, detenemos el programa
para PITAG, creamos una nueva instruccion PIT que tome las
ultimas tres cosas en la pila, y revise si el cuadrado del mayor
es igual a la suma de cuadrados de los otros dos.



para implementar cada una de estas funciones, debemos primero
agregarlas como elementos del lexico:

en lexico.h:

//lista de tokens de pl0
enum simbolo {
  nulo,ident,entero,real,cadena,mas,menos,por,barra,oddtok,igl,
  nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,punto,
  dospuntos,asignacion,begintok,endtok,iftok,thentok,whiletok,
  dotok,calltok,consttok,vartok,proctok,integertok,realtok,
  readtok,readlntok,writetok,writelntok,booleantok,truetok,
  falsetok,rndtok,clrscrtok,halttok,pitagtok
}; //definido aquí en el encabezado


en lexico.cpp:

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={
  "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE",
  "THEN","VAR","WHILE","INTEGER","REAL","READ","READLN","WRITE",
  "WRITELN","BOOLEAN","TRUE","FALSE","RND","CLRSCR","HALT",
  "PITAG"
};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal [MAXPAL]={
  begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,
  thentok,vartok,whiletok,integertok,realtok,readtok,readlntok,
  writetok,writelntok,booleantok,truetok,falsetok,rndtok,
  clrscrtok,halttok,pitagtok
};


en parametros.h:

#define MAXPAL     24  //numero de palabras reservadas

con eso ya el canner puede distinguirlos como elementos del
lexico del lenguaje, asi que debemos de agregarlos a la
sintaxis:


dado que estos retornan numeros, pueden ser interpretados como
tales, asi que lo agregamos como factores

en parser.cpp:



TODO: hacer RND la operacion 14, CLRSCR la operacion 15, y PITAG
la operacion 16


TODO: agregar HLT

en codigo_p.h agregamos HLT:
//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP,HLT}; //definido en este encabezado

y en codigo_p.cpp:

  char *mnemonico[]={
    "LIT",
    "OPR",
    "CAR",
    "ALM",
    "LLA",
    "INS",
    "SAL",
    "SAC",
    "REA",
    "IMP",
    "HLT"
  };
  
  char *comentario[]={
    ";cargar una constante",
    ";operacion aritmetica, relacional o retornar",
    ";cargar una variable",
    ";almacenamiento/instruccion de asignacion",
    ";llamada a procedimiento",
    ";asignacion de espacio de memoria",
    ";salto incondicional",
    ";salto condicional",
    ";instruccion de lectura",
    ";instruccion de impresion en pantalla",
    ";instruccion para detener la ejecucion"
  };









