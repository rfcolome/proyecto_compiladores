//un parser descendente determin�stico para pl0
#include <stdlib.h>
#include "parser.h"
#include "pl0.h"
#include "conjuntos.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones internas al parser
void declaracionconst(),declaracionvar(),instruccion(int toksig[]);
void expresion(int toksig[]),termino(int toksig[]),
  factor(int toksig[]),funcion(int toksig[]),condicion(int toksig[]),
  declaracionboolean();

int esFuncion();

//bloque
//da inicio el an�lisis sint�ctico
void bloque (int toksig[])
{
  int temp;
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  int vacio[NOTOKENS];   //conjunto vac�o
  
  init_set(vacio);
  
  do { 
    if (token==consttok) {
      obtoken();
      //do-while:equivale a insertar una coma
      do {
        declaracionconst(); //llamada a funci�n que checa que la constante est� bien definida.                                   */
        //si lo est�, se coloca el identificador en la TDS 
        while (token==coma) {
          obtoken();
          declaracionconst();
        }
        if (token==puntoycoma) 
          obtoken();
        else
          error(5); //error 5: falta una coma o un punto y coma
      } while (token==ident); 
    }
    
    if (token==vartok) {
      obtoken();
      //an�logo al "do" de arriba
      do {
        declaracionvar(); //llamada a funci�n que checa que la variable est� bien declarada.                                 */
        //si lo est�, se coloca el identificador en la TDS
        while (token==coma) {
          obtoken();
          declaracionvar();
        }
        if (token==puntoycoma)
          obtoken();
        else
          error(5); //error 5: Falta una coma o un punto y coma
      } while (token==ident);
    }
    
    while (token==proctok) {
      obtoken();
      if (token==ident) {
        poner(PROCEDIMIENTO);
        obtoken();
      }
      else
        error(4); //error 4: Const, Var y Procedure deben ir seguidos de un identificador
      
      if (token==puntoycoma)
        obtoken();
      else
        error(5); //error 5: falta una coma o un punto y coma 
      
      temp=it;
      copia_set(setpaso,toksig);
      setpaso[puntoycoma]=1; //setpaso=puntoycoma+toksig
      
      bloque(setpaso);       //sucesor+s�mbolos de caj�n
      
      it=temp;
      
      if (token==puntoycoma) {
        obtoken();
        copia_set(setpaso,tokiniinst);
        setpaso[ident]=setpaso[proctok]=1; //setpaso=tokiniinst+ident+proctok
        test(setpaso,toksig,6); //�s�mbolo incorrecto despu�s de un procedimiento?
      }
      else 
        error(5);
      
    }
    
    copia_set(setpaso,tokiniinst);
    setpaso[ident]=1; //setpaso=tokiniinst+ident
    test(setpaso,tokinidecl,7); //se espera una instrucci�n
    
  } while (tokinidecl[token]==1); //salir cuando el token ya no sea de declaraci�n
  
  copia_set(setpaso,toksig);
  setpaso[puntoycoma]=setpaso[endtok]=1; //setpaso=puntoycoma+end+toksig
  instruccion(setpaso);
  
  //aqu� viene el chequeo expl�cito de que el token que viene a continuaci�n
  //est� en el conjunto de sucesores correctos (los sucesores de bloque)
  copia_set(setpaso,toksig);
  test(setpaso,vacio,8); //s�mbolo incorrecto detr�s de las instrucciones de un bloque
}

//declaracionconst
//se checa que se tenga una definici�n de constante. si est� bien
//el identificador se pone en la tabla de s�mbolos, otro caso
//el programador cometi� un error al construir la definici�n de constante
void declaracionconst() {
  int i;
  if (token==ident) {
    i=posicion();
    if (i!=0) {
      error(41); //error 11: No se deben redefinir objetos
    }

    obtoken();
    if ( (token==igl) || (token==asignacion) ) {
      //mejora del tipo "adivinaci�n" de la intenci�n del programador
      if (token==asignacion) 
        error(1); //error 1: usar '=" en vez de ":=". es la misma t�cnica que en C & C++ se usa para if (x=) por if (x==)
      obtoken();
      if (token == entero || token == real) {
        poner(CONSTANTE);
        obtoken();
      }
      else
        error(2) ; //error 2: debe ir seguido de un n�mero
    }
    else
      error (3) ; //error 3: el identificador debe ir seguido de "=" 
  }
  else
    error(4) ; //error 4: Const, Var y Procedure deben ir seguidos de un identificador
}

//declaracionvar
//se checa que se tenga una declaraci�n de variable. si est� bien
//el identificador se pone en la tabla de s�mbolos, otro caso  
//el programador cometi� un error al construir la declaraci�n de variable
void declaracionvar() {
  int i;
  if (token==ident) {
    i=posicion();
    if (i!=0) {
      error(41); //error 11: No se deben redefinir objetos
    }

    poner(VARIABLE);
    obtoken();
  }
  else
    error(4) ; //error 4: Const, Var y Procedure deben ir seguidos de un identificador 
}

//instruccion 
void instruccion(int toksig[])
{
  int i;
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  int vacio[NOTOKENS];   //conjunto vac�o
  
  init_set(vacio);
  
  if (token==ident) {
    if (esFuncion()) {
      copia_set(setpaso,toksig);
      funcion(setpaso);
    }
    else {
      //ve a buscarlo a la tabla de s�mbolos
      i=posicion();
      if (i==0) {
        error(11); //error 11: identificador no declarado
      }
      else if (buscarElemento(i) -> tipo != VARIABLE)
        error(12); //error 12: no est�n permitidas las asignaciones a constantes o a procedimientos
      obtoken();
      if (token==asignacion)
        obtoken();
      else
        error(13); //error 13: se esperaba el operador de asignaci�n 
      
      copia_set(setpaso,toksig);
      expresion(setpaso);
    }
  } 
  else if (token==calltok) {
    obtoken();
    if (token!=ident)
      error(14); //error 14: "CALL" debe ir seguido de un identificador 
    else {
      //buscar el nombre del procedimiento en la tabla de s�mbolos
      i=posicion();
      if (i==0)
        error(11); //error 11: Identificador no declarado 
      else if (buscarElemento(i) -> tipo!=PROCEDIMIENTO)
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
    
    copia_set(setpaso,toksig);
    instruccion(toksig);
  } 
  else if (token==begintok) {
    obtoken();
    copia_set(setpaso,toksig);
    setpaso[puntoycoma]=setpaso[endtok]=1; //setpaso=puntoycoma+endtok+toksig
    instruccion(setpaso);
    
    while (token==puntoycoma || token==calltok || token==begintok || token==iftok || token==whiletok || token == fortok) {
      //aqu� el while 'inserta' el punto y coma para continuar compilando cuando no lo encuentre
      //el compilador detecta la omisi�n cl�sica del punto y coma
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
    obtoken();
    
    copia_set(setpaso,toksig);
    setpaso[dotok]=1;//setpaso=dotok+toksig
    
    condicion(setpaso);
    
    if (token==dotok) 
      obtoken();
    else
      error(18); //error 18: Se esperaba un "DO" 
    
    copia_set(setpaso,toksig);
    instruccion(setpaso);         
  }
  else if (token == fortok) {
    obtoken();
    
    copia_set(setpaso,toksig);
    setpaso[dotok]=1;//setpaso=dotok+toksig
    
    if (token == ident) {
      i = posicion();
      if (i == 0) { // no se encontro el identificador
        error(11); // identificador no declarado
      }
      else if (buscarElemento(i) -> tipo==PROCEDIMIENTO) {
        error(21); //error 21: identificador de procedimiento
      }
      else { // era un identificador valido
        obtoken();
        if (token == intok) {
          obtoken();
          if (token == entero) {
            obtoken();
            if (token == dospuntos) {
              obtoken();
              if (token == entero) {
                obtoken();
                if (token == dotok) {
                  obtoken();
                  copia_set(setpaso,toksig);
                  instruccion(setpaso);
                }
                else {
                  error(18);
                }
              }
              else {
                error(38); // se esperaba un numero entero
              }
            }
            else {
              error(39); // se esperaban dos puntos
            }
          }
          else {
            error(38); // se esperaba un numero entero
          }
        }
        else {
          error(29); // se esperaba un IN despues del FOR
        }
      }
    }
    else
      error(28);
    
  } else if (token == booleantok) {
    obtoken();
    declaracionboolean();
    
  }
  
  //comprobaci�n expl�cita de que los tokens que viene son sucesores de instrucci�n  
  copia_set(setpaso,toksig);
  test(setpaso,vacio,19); //error(19): Un simbolo incorrecto sigue a una instrucci�n
}	      

//expresion
void expresion(int toksig[])
{
  int setpaso[NOTOKENS]; //conjunto de paso por valor        
  
  copia_set(setpaso,toksig);
  setpaso[mas]=setpaso[menos]=1; //setpaso=mas+menos+toksig
  
  if (token==mas || token==menos) {
    obtoken();
    termino(setpaso);
  }
  else 
    termino(setpaso);
  
  while (token==mas || token==menos) {
    obtoken();
    
    copia_set(setpaso,toksig);
    setpaso[mas]=setpaso[menos]=1; //setpaso=mas+menos+toksig
    
    termino(setpaso);       
  }
}

//termino
void termino(int toksig[])
{
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  
  copia_set(setpaso,toksig);
  setpaso[por]=setpaso[barra]=1;//setpaso=por+barra+toksig
  
  factor(setpaso);
  
  while (token==por || token==barra) {
    obtoken();
    
    copia_set(setpaso,toksig);
    setpaso[por]=setpaso[barra]=1; //setpaso=por+barra+toksig
    
    factor(setpaso);
  }
}

//factor
void factor(int toksig[])
{
  int i;
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  
  test(tokinifact,toksig,24); //error(24): Una expresi�n no puede empezar con este s�mbolo
  
  while (tokinifact[token]==1) {
    //mientras el token sea un s�mbolo inicial de factor...
    if (token==ident) {
      i=posicion();
      if (i==0) {
        copia_set(setpaso,toksig);
        setpaso[parenc]=1; //setpaso=parenc+toksig
        funcion(setpaso);
        //error(11); //error 11: Identificador no declarado  
      }else if (buscarElemento(i) -> tipo==PROCEDIMIENTO)
        error(21); //error 21: Una expresi�n no debe contener un identificador de procedimiento
      obtoken();
    }
    else if (token == entero || token == real) 
      obtoken();          
    else if (token==parena) {
      obtoken();
      copia_set(setpaso,toksig);
      setpaso[parenc]=1; //setpaso=parenc+toksig
      
      expresion(setpaso);
      
      if (token==parenc)
        obtoken();
      else
        error(22); //error 22: Falta un par�ntesis de cierre  
    };
    
    init_set(setpaso);  //conjunto vac�o
    setpaso[parena]=1;  //setpaso=parena
    test(toksig,setpaso,23); //error(23): El factor anterior no puede ir seguido de este simbolo o falta un punto y coma
  }
}


void funcion(int toksig[]) {
  int i;
  if (strcmp(lex, "READ")   == 0 ||
      strcmp(lex, "READLN") == 0) {
    obtoken();
    if (token == parena) {
      obtoken();
      if (token == ident) {
        i = posicion();
        if (i == 0) { // no se encontro el identificador
          error(11); // identificador no declarado
        }
        else if (buscarElemento(i) -> tipo==PROCEDIMIENTO) {
          error(21); //error 21: identificador de procedimiento
        }
        else { // era un identificador valido
          obtoken();
          if (token == parenc) {
            // todo nice
            obtoken();
          }
          else
            error(27);// se esperaba un parentesis de cierre 
        }
      }
      else
        error(28);
    }
    else
      error(26); // se esperaba una lista de argumentos
  }
  else if (strcmp(lex, "WRITE")   == 0 ||
           strcmp(lex, "WRITELN") == 0 ){
    obtoken();
    if (token == parena) {
      obtoken();
      if (token == string) {
        obtoken();
        if (token == coma) {
          obtoken();
        }
        else
          error(5); // falta coma o punto y coma
      }
      
      if (token == ident) {
        i = posicion();
        if (i == 0) { // no se encontro el identificador
          error(11); // identificador no declarado
        }
        else if (buscarElemento(i) -> tipo==PROCEDIMIENTO) {
          error(21); //error 21: identificador de procedimiento
        }
        else { // era un identificador valido
          obtoken();
          if (token == parenc) {
            // todo nice
            obtoken();
          }
          else
            error(27);
        }
      }
      else
        error(28);
    }
    else
      error(26); // se esperaba una lista de argumentos
  }
  //random
  else if (strcmp(lex, "RND")   == 0 ||
           strcmp(lex, "CLRSCR") == 0 ||
           strcmp(lex, "HALT") == 0  ){
    obtoken();
    if (token == parena) {
      obtoken();
      if (token == parenc) {
        obtoken();
      }
      else
        error(27);//se esperaba parentesis de cierre
    }
  }
  
  //pitagoras
  else if (strcmp(lex, "PITAG")   == 0 ){
    obtoken();
    if (token == parena) {
      obtoken();
      if (token == ident) {
        obtoken();
        if (token == coma) {
          obtoken();
          if (token == ident) {
            obtoken();
            if (token == coma) {
              obtoken();
              if (token == ident) {
                obtoken();
                if (token == parenc) {
                  obtoken();
                }
                else
                  error(27); // se esperaba parentesis de cierre
              }
              else
                error(36); // se esperaba un identificador
            }
            else
              error(5); // falta coma o punto y coma
          }
          else
            error(36); // se esperaba un identificador
        }
        else
          error(5); // falta coma o punto y coma
      }
      else
        error(36); // se esperaba un identificador
    }
    else
      error(37); // se esperaba parentesis de apertura
  }
  
	void declaracioninline() {
		if(token == parena){
			while(true){
				obtoken();
					if(token == codigoptok){
						obtoken();
						if(token == entero){
							obtoken();
							if(token == entero){
								obtoken();
									if(token == parenc){
										break; //Sale del while, listo para esperar punto y coma para terminar el inline
									}else{
										if(token == puntoycoma){
											//No realiza nada pues sigue otra instrucci�n codigo p
										}else{
											error(42); //Se esperaba parentesis de cierre o separador de instrucciones
										}
									}
							}else{
								error(38); //Se esperaba un n�mero
							}
						}else{
							error(38); //Se esperaba un n�mero
						}
					}else{
						error(43); //Se esperaba instrucci�n de c�digo P
					}
			} //Fin del while de instrucciones codigo p
			obtoken();
			if(token == puntoycoma){
			obtoken(); //Termin� el inline y agarra el pr�ximo token
			}else{
				error(5); //No existe punto y coma para terminar el inline
			}
		}else{
			error(37); //Se esperaba parentesis de apertura
		}
	}
	else
    error(11); // identificador no declarado
}

int esFuncion() {
  return strcmp(lex, "READ")    == 0 ||
    strcmp(lex, "READLN")  == 0 ||
    strcmp(lex, "WRITE")   == 0 ||
    strcmp(lex, "WRITELN") == 0 ||
    strcmp(lex, "RND")     == 0 ||
    strcmp(lex, "CLRSCR")  == 0 ||
    strcmp(lex, "HALT")    == 0 ||
    strcmp(lex, "PITAG")   == 0;
}

void iniprograma(void){
  if(token==programtok){
    obtoken();
    if(token == ident) obtoken();
    else error(36);//se esperaba un identificador
    if (token== puntoycoma) obtoken();
    else error(5);//se esperaba un punto y coma
  }	
}

void declaracionboolean() { //boolean variable := [true,false,0,1]
  int i;
  if (token==ident) {
    //ve a buscarlo a la tabla de s�mbolos
    i=posicion();
    if (i==0) {
      error(11); //error 11: identificador no declarado
    }
    else if (buscarElemento(i) -> tipo != VARIABLE)
      error(12); //error 12: no est�n permitidas las asignaciones a constantes o a procedimientos
    obtoken();
    if (token==asignacion) {
      //mejora del tipo "adivinaci�n" de la intenci�n del programador
      obtoken();
      switch(token){
      case truetok: break;
      case falsetok: break;
      default: error(40); break; //No coinciden tipos
      }
      obtoken();
      if(token == puntoycoma){
        obtoken();
      }else{
        error(5); //punto y coma
      }
    }
    else
      error (3) ; //error 3: el identificador debe ir seguido de ":=" 
  }
  else
    error(4) ; //error 4: Boolean, Const, Var y Procedure deben ir seguidos de un identificador
}

//condicion
void condicion(int toksig[])
{
  int setpaso[NOTOKENS]; //conjunto de paso por valor
  
  
  if (token==oddtok) {
    obtoken();
    copia_set(setpaso,toksig); //setpaso=toksig
    expresion(setpaso);    
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
      obtoken();
      
      copia_set(setpaso,toksig); //setpaso=toksig
      expresion(setpaso);          
    }
  }
}

