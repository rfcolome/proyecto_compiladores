//un parser descendente determinístico para pl0
#include <stdlib.h>
#include "parametros.h"
#include "parser.h"
#include "pl0.h"
#include "conjuntos.h"
#include "tds.h"
#include "codigo_p.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones internas al parser
void declaracionconst(int *idat),declaracionvar(int *idat),instruccion(int toksig[]);
void expresion(int toksig[]),termino(int toksig[]),factor(int toksig[]),condicion(int toksig[]);

//bloque
//da inicio el análisis sintáctico
void bloque (int toksig[]) {
  int temp,tempniv;
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  int vacio[NOTOKENS];   //conjunto vacío

  int idat; //índice de asignación de memoria, comienza con 3 por ED, DR y EE
  int it0 ; //índice que "recuerda" en donde comienzan las instrucciones de este bloque

  init_set(vacio);

  idat=3;
  it0=it; //recordamos en donde comienzan en la TDS las declaraciones de este bloque

  //detalle técnico
  tabla[it].tipo=PROCEDIMIENTO;
  tabla[it].variante.nivdir.dir=ic;
	  
  gen(SAL,0,0); //un procedimiento significa un salto en el código. luego cambiaremos nivel y direccion,
  //los mostrados arriba (0 y 0) son 'paja', lo que pasa es que todavia no sabemos
  //todavía hacia donde saltar

  //viene una limitación especifica del compilador  
  if (niv > MAXNIV) {
    error(34); //anidamiento demasiado profundo para los procedures. decisión de Wirth. 
    estadisticas();
    fclose(fp);
    exit(1);   //el error es considerado fatal
  }

  do { 
    if (token==consttok) {
      obtoken();
      //do-while:equivale a insertar una coma
      do {
        declaracionconst(&idat);//llamada a función que checa
                                //que la constante esté bien
                                //definida. si lo está, se
                                //coloca el identificador en la
                                //TDS 
        while (token==coma) {
          obtoken();
          declaracionconst(&idat);
        }
        if (token==puntoycoma) 
          obtoken();
        else
          error(5); //error 5: falta una coma o un punto y coma
      } while (token==ident); 
    }
    
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
          if (token == integertok || token == realtok || token == booleantok) {
            if (token == integertok)
              setearTipos(ENTERO);
            else if (token == realtok)
              setearTipos(REAL);
            else if (token == booleantok)
              setearTipos(BOOLEANO);
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

    while (token==proctok) {
      obtoken();
      if (token==ident) {
        poner(PROCEDIMIENTO,&idat);
        obtoken();
      }
      else
        error(4); //error 4: Const, Var y Procedure deben ir seguidos de un identificador

      if (token==puntoycoma)
        obtoken();
      else
        error(5); //error 5: falta una coma o un punto y coma 

      temp=it;
      tempniv=niv;
      ++niv;
      copia_set(setpaso,toksig);
      setpaso[puntoycoma]=1; //setpaso=puntoycoma+toksig

      bloque(setpaso);       //sucesor+símbolos de cajón

      // TODO: esto debe ser reemplazado en la tds por una
      // instruccion de "limpieza"
      it=temp;
      niv=tempniv;

      if (token==puntoycoma) {
        obtoken();
        copia_set(setpaso,tokiniinst);
        setpaso[ident]=setpaso[proctok]=1; //setpaso=tokiniinst+ident+proctok
        test(setpaso,toksig,6); //¿símbolo incorrecto después de un procedimiento?
      }
      else 
        error(5);

    }

    copia_set(setpaso,tokiniinst);
    setpaso[ident]=1; //setpaso=tokiniinst+ident
    test(setpaso,tokinidecl,7); //se espera una instrucción

  } while (tokinidecl[token]==1); //salir cuando el token ya no sea de declaración

  //parchamos a la TDS y el código intermedio
  //backpatching sobre TDS y código
  codigo[tabla[it0].variante.nivdir.dir].di=ic;
  tabla[it0].variante.nivdir.dir=ic; //aquí en ic es donde comienza el código para este procedure  

  //se abre espacio en la memoria para un mínimo de 3 direcciones
  gen(INS,0,idat); 

  copia_set(setpaso,toksig);
  setpaso[puntoycoma]=setpaso[endtok]=1; //setpaso=puntoycoma+end+toksig
  instruccion(setpaso);

  gen(OPR,0,0); //retorno

  //aquí viene el chequeo explícito de que el token que viene a continuación
  //está en el conjunto de sucesores correctos (los sucesores de bloque)
  copia_set(setpaso,toksig);
  test(setpaso,vacio,8); //símbolo incorrecto detrás de las instrucciones de un bloque
}

//declaracionconst
//se checa que se tenga una definición de constante. si está bien
//el identificador se pone en la tabla de símbolos, otro caso
//el programador cometió un error al construir la definición de constante
void declaracionconst(int *idat) {
  if (token==ident) {
    obtoken();
    if (token == dospuntos) {
      obtoken();
      if (token == integertok || token == realtok || token == booleantok) {
        obtoken();
        if ( (token==igl) || (token==asignacion) ) {
          //mejora del tipo "adivinación" de la intención del programador
          if (token==asignacion) 
            error(1); //error 1: usar '=" en vez de ":=". es la misma técnica que en C & C++ se usa para if (x=) por if (x==)
          obtoken();
          if (token==entero || token == real || token == truetok || token == falsetok) {
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

//declaracionvar
//se checa que se tenga una declaración de variable. si está bien
//el identificador se pone en la tabla de símbolos, otro caso  
//el programador cometió un error al construir la declaración de variable
void declaracionvar(int *idat)
{
 if (token==ident) {
    poner(VARIABLE,&(*idat));
    obtoken();
 }
 else
    error(4) ; //error 4: Const, Var y Procedure deben ir seguidos de un identificador                               
}

//instruccion 
void instruccion(int toksig[]) {
  int i;
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  int vacio[NOTOKENS];   //conjunto vacío
  int ic1,ic2;           //variables para backpatching del código-p

  init_set(vacio);

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
    expresion(setpaso);

    //generar código-p
    gen(ALM,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);

  } 
  else if (token==calltok) {
    obtoken();
    if (token!=ident)
      error(14); //error 14: "CALL" debe ir seguido de un identificador 
    else {
      //buscar el nombre del procedimiento en la tabla de símbolos
      i=posicion();
      if (i==0)
        error(11); //error 11: Identificador no declarado 
      else if (tabla[i].tipo==PROCEDIMIENTO)
        //generar código-p
        gen(LLA,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
      else
        error(15); //error 15 : No tiene sentido llamar a una constante o a una variable 
      obtoken();
    }
  } 
  else if (token==iftok) {
    obtoken();
    copia_set(setpaso,toksig);
    setpaso[thentok]=setpaso[dotok]=1; //setpaso=thentok+dotok+toksig
    condicion(setpaso);
    if (token==thentok)
      obtoken();
    else
      error(16); //error 16: Se esperaba un "THEN" 

    ic1=ic; //recordar esta dirección en el código-p para "parchar" más adelante
    //generar código-p
    gen(SAC,0,0);

    copia_set(setpaso,toksig);
    instruccion(toksig);

    //aquí viene el "backpatching"
    codigo[ic1].di=ic;

  } 
  else if (token==begintok) {
    obtoken();
    copia_set(setpaso,toksig);
    setpaso[puntoycoma]=setpaso[endtok]=1; //setpaso=puntoycoma+endtok+toksig
    instruccion(setpaso);

    while (token==puntoycoma || token==calltok || token==begintok || token==iftok || token==whiletok ) {
      //aquí el while 'inserta' el punto y coma para continuar compilando cuando no lo encuentre
      //el compilador detecta la omisión clásica del punto y coma
      if (token==puntoycoma)
        obtoken();
      else
        error(10); //error 10: Falta un punto y coma entre instrucciones

      copia_set(setpaso,toksig);
      setpaso[puntoycoma]=setpaso[endtok]=1; //setpaso=puntoycoma+endtok+toksig
      instruccion(setpaso);
    }

    if (token==endtok)
      obtoken();
    else
      error(17); //error 17: Se esperaba un "END" o un punto y coma 
  } 
  else if (token==whiletok) {
    ic1=ic; //recordar en que lugar del codigo estamos
    obtoken();

    copia_set(setpaso,toksig);
    setpaso[dotok]=1;//setpaso=dotok+toksig

    condicion(setpaso);

    ic2=ic; //recordar este lugar
    gen(SAC,0,0); //nivel y dir son "paja". Luego parcharemos aqui

    if (token==dotok) 
      obtoken();
    else
      error(18); //error 18: Se esperaba un "DO" 

    copia_set(setpaso,toksig);
    instruccion(setpaso);

    //aqui backpatching
    gen(SAL,0,ic1);
    codigo[ic2].di=ic;
  }
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
          if (token == parenc){
            obtoken();
            gen(REA,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
            // hasta donde yo se, el primero es el nivel en el
            // que esta la variable, y el segundo es la
            // direccion dentro del nivel

          }
          else
            error(22); // falta un parentesis de cierre
        }
      }
    }
    else
      error(27); // se esperaba un parentesis de apertura
  }
  else if (token == writetok || token == writelntok) {
    int agregarnl = token == writelntok;
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
              gen(IMP,0,tabla[i].tipoDato); // el 1 es para enteros
              
              if (agregarnl) {
                // pongo un newline y lo imprimo si es writeln
                gen(LIT,0,'\0');
                gen(LIT,0,'\n');
                gen(IMP,0,0); // el 0 es para cadenas
              }

              obtoken();
              if (token == parenc)
                obtoken();
              else
                error(22); // falta un parentesis de cierre
            }
          }
        }
        else if (token == parenc) {
          if (agregarnl) {
            // pongo un newline y lo imprimo si es writeln
            gen(LIT,0,'\0');
            gen(LIT,0,'\n');
            gen(IMP,0,0); // el 0 es para cadenas
          }

          obtoken();
        }
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

          if (agregarnl) {
            // pongo un newline y lo imprimo si es writeln
            gen(LIT,0,'\0');
            gen(LIT,0,'\n');
            gen(IMP,0,0); // el 0 es para cadenas
          }
          
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

  //comprobación explícita de que los tokens que viene son sucesores de instrucción  
  copia_set(setpaso,toksig);
  test(setpaso,vacio,19); //error(19): Un simbolo incorrecto sigue a una instrucción
}

//expresion
void expresion(int toksig[])
{
 int setpaso[NOTOKENS]; //conjunto de paso por valor        
 enum simbolo opsuma;   //operadores unarios/binarios + o - 

 copia_set(setpaso,toksig);
 setpaso[mas]=setpaso[menos]=1; //setpaso=mas+menos+toksig

 //aquí viene conversión a rpn=reverse polish notation
 if (token==mas || token==menos) {
    opsuma=token; //es el operador unario + o -, se retiene el signo
    obtoken();
    termino(setpaso);
    if (opsuma==menos)
       gen(OPR,0,1);//si es + no hay necesidad de hacer nada
 }
 else 
    termino(setpaso);

 while (token==mas || token==menos) {
       //ahora + y - son operadores binarios
       //retrasamos la transmisión del operador (diferir la generación de código intermedio)
       opsuma=token;   
       obtoken();

       copia_set(setpaso,toksig);
       setpaso[mas]=setpaso[menos]=1; //setpaso=mas+menos+toksig

       termino(setpaso);
       if (opsuma==mas)
          gen(OPR,0,2);
       else
          gen(OPR,0,3);
 }
}

//termino
void termino(int toksig[])
{
 int setpaso[NOTOKENS]; //conjunto de paso por valor
 enum simbolo opmul;    //operador multiplicativo 

 //aquí conversión a rpn=reverse polish notation
 copia_set(setpaso,toksig);
 setpaso[por]=setpaso[barra]=1;//setpaso=por+barra+toksig

 factor(setpaso);

 while (token==por || token==barra) {
       opmul=token; //se retiene el operador(diferir la generación de código intermedio)
       obtoken();

       copia_set(setpaso,toksig);
       setpaso[por]=setpaso[barra]=1; //setpaso=por+barra+toksig

       factor(setpaso);
       if (opmul==por)
          gen(OPR,0,4);
       else
          gen(OPR,0,5);
 }
}

//factor
void factor(int toksig[]) {
  int i;
  int setpaso[NOTOKENS]; //conjunto de paso por valor

  test(tokinifact,toksig,24); //error(24): Una expresión no puede empezar con este símbolo

  while (tokinifact[token]==1) {
    //mientras el token sea un símbolo inicial de factor...
    if (token==ident) {
      i=posicion();
      if (i==0) 
        error(11); //error 11: Identificador no declarado
      else
        switch (tabla[i].tipo) {
        case CONSTANTE:
          gen(LIT,0,tabla[i].variante.val);
          break;
        case VARIABLE:
          gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
          break;
        case PROCEDIMIENTO:
          error(21); //error 21: Una expresión no debe contener un identificador de procedimiento
          break;     
        };
      obtoken();
    }
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
    else if (token==parena) {
      //solo se checa sintáxis pues los paréntesis no interesan en rpn
      obtoken();

      copia_set(setpaso,toksig);
      setpaso[parenc]=1; //setpaso=parenc+toksig

      expresion(setpaso);

      if (token==parenc)
        obtoken();
      else
        error(22); //error 22: Falta un paréntesis de cierre  
    }
    else if (token == rndtok) {
      obtoken();
      if (token == parena) {
        obtoken();
        if (token == entero) {
          gen(LIT, 0, valor);
        }
        else if (token == real) {
          gen(LIT, 0, valorReal);
        }
        else if (token == ident) {
          i=posicion();
          if (i==0) 
            error(11); //error 11: Identificador no declarado
          else
            switch (tabla[i].tipo) {
              
            case CONSTANTE:
              gen(LIT,0,tabla[i].variante.val);
              break;
            case VARIABLE:
              gen(CAR,niv-tabla[i].variante.nivdir.nivel,tabla[i].variante.nivdir.dir);
              break;
            case PROCEDIMIENTO:
              error(21); //error 21: Una expresión no debe contener un identificador de procedimiento
              break;
            }
        } else {
          // por defecto se generan numeros entre 0 y 100
          gen(LIT, 0, 100);
        }

        gen(OPR,0,14); // RND es la operacion 14
        if (token == parenc) {
          obtoken();
        }
        else
          error(22); // falta un parentesis de cierre
      }
      else
        error(27); // se esperaba un parentesis de apertura
    }
    else if (token == clrscrtok) {
      obtoken();
      gen(OPR,0,15); // CLRSCR es la operacion 15
      if (token == parena) {
        obtoken();
        if (token == parenc) {
          obtoken();
        }
        else
          error(22); // falta un parentesis de cierre
      }
      else
        error(27); // se esperaba un parentesis de apertura
    }
    else if (token == halttok) {
      obtoken();
      gen(HLT,0,0);
      if (token == parena) {
        obtoken();
        if (token == parenc) {
          obtoken();
        }
        else
          error(22); // falta un parentesis de cierre
      }
      else
        error(27); // se esperaba un parentesis de apertura
      
    }
    else if (token == pitagtok) {
      obtoken();
      if (token == parena) {
        obtoken();
        // TODO: agregarle los argumentos a esto.
        // TODO: agregar todas las operaciones nuevas y HLT
        //         al interprete de codigo-p
        if (token == parenc) {
          obtoken();
        }
        else
          error(22); // falta un parentesis de cierre
      }
      else
        error(27); // se esperaba un parentesis de apertura
    }


    init_set(setpaso);  //conjunto vacío
    setpaso[parena]=1;  //setpaso=parena
    test(toksig,setpaso,23); //error(23): El factor anterior no puede ir seguido de este simbolo o falta un punto y coma
  }
}


//condicion
void condicion(int toksig[])
{
 int setpaso[NOTOKENS]; //conjunto de paso por valor
 enum simbolo oprel;    //operador relacional     

 if (token==oddtok) {
    obtoken();
    copia_set(setpaso,toksig); //setpaso=toksig
    expresion(setpaso);
    gen(OPR,0,6);
 }
 else {
    //en este caso los sucesores de expresion son los operadores relacionales
    copia_set(setpaso,toksig);
    setpaso[igl]=setpaso[nig]=setpaso[mnr]=setpaso[myr]=setpaso[mei]=setpaso[mai]=1;
    //setpaso=igl+nig+mnr+myr+mei+mai+toksig

    expresion(setpaso);
    if ( (token!=igl) && (token!=nig) && (token!=mnr) && (token!=mei) && (token!=myr) && (token!=mai) )
       error(20); //error 20: Se esperaba un operador relacional 
    else {
       oprel=token;
       obtoken();

       copia_set(setpaso,toksig); //setpaso=toksig
       expresion(setpaso);

       switch(oprel) {
             case igl:
                  gen(OPR,0,8);
                  break;
             case nig:
                  gen(OPR,0,9);
                  break;
             case mnr:
                  gen(OPR,0,10);
                  break;
             case mai:
                  gen(OPR,0,11);
                  break;
             case myr:
                  gen(OPR,0,12);
                  break;
             case mei:
                  gen(OPR,0,13);
                  break;
       };
   
    }
 }
}

