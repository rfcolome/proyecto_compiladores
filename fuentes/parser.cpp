//un parser descendente determin�stico para pl0
#include <stdlib.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones internas al parser
void declaracionconst(),declaracionvar(),instruccion();
void expresion(),termino(),factor(),condicion();

//bloque
//da inicio el an�lisis sint�ctico
void bloque ()
{
 int temp;
 
 if (token==consttok) {
    obtoken();
    declaracionconst();//llamada a funci�n que checa que la constante est� bien definida.                                   */
			           //si lo est�, se coloca el identificador en la tds 
	while (token==coma) {
	      obtoken();
	      declaracionconst();
	}
	if (token==puntoycoma) 
	   obtoken();
	else
	   error(5); //error 5: falta una coma o un punto y coma
 }

 if (token==vartok) {
    obtoken();
    declaracionvar(); //llamada a funci�n que checa que la variable est� bien declarada.                                 */
				      //si lo est�, se coloca el identificador en la tds
	while (token==coma) {
	      obtoken();
	      declaracionvar();
	}
	if (token==puntoycoma)
	   obtoken();
	else
	   error(5); //error 5: Falta una coma o un punto y coma
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
       bloque();       
	   it=temp;
         
       if (token==puntoycoma)
	      obtoken();
	   else 
	      error(5);
 }
 instruccion();
}

//declaracionconst
//se checa que se tenga una definici�n de constante. si est� bien
//el identificador se pone en la tabla de s�mbolos, otro caso
//el programador cometi� un error al construir la definici�n de constante
void declaracionconst()
{
 if (token==ident) {
    obtoken();
    if (token==igl) {
       obtoken();
       if (token==entero || token == real) {
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
void declaracionvar()
{
 if (token==ident) {
    poner(VARIABLE);
    obtoken();
 }
 else
    error(4) ; //error 4: Const, Var y Procedure deben ir seguidos de un identificador                               
}

//instruccion 
void instruccion()
{
 int i;
 if (token==ident) {
    //ve a buscarlo a la tabla de s�mbolos
    i=posicion() ;
    if (i==0)
       error(11); //error 11: identificador no declarado 
    else 
       if (tabla[i].tipo != VARIABLE)
          error(12); //error 12: no est�n permitidas las asignaciones a constantes o a procedimientos
    obtoken();
    if (token==asignacion)
        obtoken();
    else
       error(13) ; //error 13: se esperaba el operador de asignaci�n 

    expresion();	   
 } 
 else 
    if (token==calltok) {
       obtoken();
       if (token!=ident)
          error(14); //error 14: "CALL" debe ir seguido de un identificador 
       else {
            //buscar el nombre del procedimiento en la tabla de s�mbolos
            i=posicion();
            if (i==0)
	           error(11); //error 11: Identificador no declarado 
            else
	           if (tabla[i].tipo!=PROCEDIMIENTO)
	              error(15); //error 15 : No tiene sentido llamar a una constante o a una variable 
            obtoken();
       }
    } 
	else
       if (token==iftok) {
	      obtoken();
		  condicion();
		  if (token==thentok)
	         obtoken();
	      else
	         error(16); //error 16: Se esperaba un "THEN" 
		  instruccion();
       } 
	   else
	      if (token==begintok) {
	         obtoken();
			 instruccion();
			 while (token==puntoycoma) {
	               obtoken();
	               instruccion();
	         }
	         if (token==endtok)
	            obtoken();
	         else
	            error(17); //error 17: Se esperaba un "END" o un punto y coma 
	      } 
		  else
	         if (token==whiletok) {
	           	obtoken();
				condicion();
				if (token==dotok) 
	               obtoken();
	            else
	               error(18); //error 18: Se esperaba un "DO" 
				instruccion();				          
			 }
}	      

//expresion
void expresion()
{
 if (token==mas || token==menos) {
    obtoken();
    termino();
 }
 else 
    termino();

 while (token==mas || token==menos) {
       obtoken();
	   termino();
 }
}

//termino
void termino()
{
 factor();
 while (token==por || token==barra) {
       obtoken();
       factor();
 }
}

//factor
void factor()
{
 int i;
 if (token==ident) {
    i=posicion();
    if (i==0) 
       error(11); //error 11: Identificador no declarado
    else
       if (tabla[i].tipo==PROCEDIMIENTO)
	      error(21); //error 21: Una expresi�n no debe contener un identificador de procedimiento
	obtoken();
 }
 else 
    if (token==entero || token == real) 
       obtoken();
    else
       if (token==parena) {
          obtoken();
          expresion();
		  if (token==parenc)
             obtoken();
          else
             error(22); //error 22: Falta un par�ntesis de cierre  
       }
	   else 
		  error(23); //error 23: El factor anterior no puede ir seguido de este s�mbolo 
}


//condicion
void condicion()
{
 if (token==oddtok) {
    obtoken();
    expresion();
 }
 else {
    expresion();
    if ( (token!=igl) && (token!=nig) && (token!=mnr) && (token!=mei) && (token!=myr) && (token!=mai) )
       error(20); //error 20: Se esperaba un operador relacional 
    else {
       obtoken();
	   expresion(); 
	}
 }
}

