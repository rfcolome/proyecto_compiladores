
-----------------------------------------------------------
- EXTENSION DE LA ESPECIFICACION DE CODIGO-P SIMPLIFICADO -
-----------------------------------------------------------

se agrego soporte para numeros reales

se agregaron los siguientes mnemonicos:

- REA
- IMP
- HLT

se agregaron las siguientes operaciones al mnemonico OPR:

- 14 (operacion RND)
- 15 (operacion CLRSCR)
- 16 (operacion PITAG)



* numeros reales *

se agrego soporte para numeros reales dentro del codigo p. Para
lograr esto, se hizo un nuevo stack especificamente para numeros
reales, utilizando el mismo "indice" del primer stack, de esa
manera tratandolos como uno solo.
Por otra parte, las instrucciones ahora incluyen un valor r
(por "real") y este valor ahora se incluye como tercer argumento
de los mnemonicos.

e.g.
LIT 0 0 2.5 ; carga el valor literal 2.5 al tope de la pila, en
              los reales, y el valor 0 en el tope de la pila de
              enteros.

OPR 0 2 0.0 ; representa la operacion de suma. el 0.0 no se
              utiliza en este caso.







* REA *

REA significa "read", y es la instruccion de lectura.
Basicamente, es un simple scanf que pone los resultados de la
lectura en la direccion especificada con el nivel y base.

e.g.
REA 1 3 0.0
lee un numero de la entrada estándar, y pone los resultados de
la lectura en la direccion tres del nivel uno del stack.




* IMP *

IMP significa "imprimir", y es la instruccion de escritura.
Basicamente, es un simple printf que mueve el puntero al tope
del stack en un espacio hacia "abajo" ya que "consume" el tope
del stack (y posiblemente otros elementos abajo del tope).

tiene tres opciones:

IMP 0 0 0.0
  se usa para imprimir cadenas de caracteres. Imprime uno por
  uno los "numeros" al tope de la pila, interpretandolos como
  el codigo ASCII de los caracteres, hasta que encuentra un
  caracter de fin de cadena ('\0').

IMP 0 1 0.0
  se usa para imprimir un numero entero al tope de la pila.

IMP 0 2 0.0
  se usa para imprimir un numero real en el tope de la pila.

IMP 0 3 0.0
  se usa para imprimir un booleano en el tope de la pila (true
  es un entero distinto de cero, y false es un entero igual a
  cero).





* HLT *

HLT significa "Halt", y se usa para detener la ejecución del
programa. Internamente lo unico que hace es llamar exit(0) para
finalizar la ejecucion.

e.g.
HLT 0 0 0.0





* OPR 0 14 0.0 *

RND. esta operacion toma el valor al tope de la pila, y genera
un numero real aleatorio entre cero y el valor especificado en
el tope de la pila.



* OPR 0 15 0.0 *

CLRSCRN. Esta operacion manda a llamar el comando cls, el cual
limpia la pantalla.




* OPR 0 16 0.0 *

PITAG. esta operacion toma los ultimos tres valores de la pila,
y revisa que formen un triplete pitagorico, es decir, si los
valores son a, b, y c, en donde a > b > c, entonces revisa que
a*a == b*b + c*c. Dado que la nocion "triplete pitagorico" se
aplica unicamente para numeros enteros, estos deben ser
necesariamente numeros enteros, no numeros reales.



