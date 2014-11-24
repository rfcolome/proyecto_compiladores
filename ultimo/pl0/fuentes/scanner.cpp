//el analizador lexicográfico de pl0
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "parametros.h"
#include "scanner.h"
#include "lexico.h"
#include "auxiliares.h"
#include "tds.h"

char linea[MAXLINEA];     //buffer de líneas 
int ll;                   //contador de caracteres
int offset;               //corrimiento en la lectura de los caracteres del programa fuente
int fin_de_archivo;       //bandera de fin de archivo (obtch)   
int ch;                   //último caracter leído
char chNorm;              //último caracter leido, sensible a mayusculas
char lex[MAXID+1];        //último lexeme leído ( +1 para colocar "\0")
long int valor ;          //valor numérico de una lexeme correspondiente a un entero
double valorReal;         //valor numerico de una lexeme correspondiente a un real
char valorCad[MAXID+1];        //valor de la última cadena leida (+1 para colocar "\0")

int obtch(),getline(char s[],int lim); //funciones internas a scanner.cpp

//obtoken: obtiene el siguiente token del programa fuente                   
void obtoken() {
  char lexid[MAXID+1]; //+1 para colocar el marcador "\0"
  int i,j;
  int ok=0;

  //quitar blancos, caracter de cambio de línea y tabuladores
  while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;

  // revisamos si inicia un comentario
  if (ch == '{') {
    // de ser asi, leemos hasta que termine
    while (ch != '}') {
      ch = obtch();
    }
    ch = obtch(); // pasamos al primer caracter despues del comentario
    // leemos cualquier espacio en blanco despues
    while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch();

  }

  //si la lexeme comienza con una letra, es identificador o palabra reservada
  if (isalpha(ch)) {
    lexid[0]=ch;
    i=1;
    while ( isalpha( (ch=obtch()) ) ||  isdigit(ch)   ) 
      if (i<MAXID) lexid[i++]=ch;
    lexid[i]='\0';
  
    //¿es identificador o palabra reservada?.buscar en la tabla de palabras reservadas
	//una búsqueda lineal que tendrá que ser sustituída por otro tipo de búsqueda más efectiva. 
	//...en esa nueva búsqueda desaparecerá el "break"
    for (j=0;j<MAXPAL;++j) 
      if (strcmp(lexid,lexpal[j])==0) {
        ok=1;
        break;
      }

    if (ok==1) {
      token=tokpal[j]; //es palabra reservada
      if (token == truetok || token == falsetok) {
        valor = (token == truetok);
        tipoActual = BOOLEANO;
      }
    }
    else
      token=ident; //es identificador
 
    strcpy(lex,lexid); //copiar en lex
  }
  else if (isdigit(ch)) { //si comienza con un dígito...
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
    tipoActual = ENTERO;

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
        tipoActual = REAL;
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
        tipoActual = REAL;
      }
      else {
        // detectamos un exponente, pero sin digitos;
        // por lo tanto, esta mal escrito.
        lexid[i] = '\0';
        token=nulo;         
      }
    }
  }
  else if (ch == '"') { // reconocimiento de cadenas
    lexid[0]=ch;
    i=1;
    ch=obtch();
    while (ch != '"') {
      if (i<MAXID) {
        valorCad[i-1] = chNorm;
        lexid[i++]=ch;
      }
      ch=obtch();
    }
    valorCad[i-1] = '\0';
    if (i<MAXID) lexid[i++]=ch; // incluimos el " de cierre
    ch = obtch();
    lexid[i]='\0';
    token = cadena;
  }
  else if (ch=='<') { //reconocimiento de símbolos especiales,
    // incluyendo pares de simbolos
    // (aplicamos "lookahead symbol technique")
    ch=obtch();
    if (ch=='=') {
      token=mei;
      ch=obtch();
    }
    else if (ch=='>') {
      token=nig;
      ch=obtch();
    }
    else
      token=mnr;
  }
  else if (ch=='>') {
    ch=obtch();
    if (ch=='=') {
      token=mai;
      ch=obtch();
    }
    else 
      token=myr;
  }
  else if (ch==':') {
    ch=obtch();
    if (ch=='=') {
      token=asignacion;
      ch=obtch();
    }
    else
      token=dospuntos;
  }
  else {
    token=espec[ch]; //hashing directo en la tabla de tokens de símbolos especiales del lenguaje
    ch=obtch();
  }
}

//obtch: obtiene el siguiente caracter del programa fuente
int obtch() { 

  if (fin_de_archivo==1) {
    error(32);//error 32: el programa fuente esta incompleto
    estadisticas();
	fclose(fp);
    exit(1); //el error es fatal
  }
  
  if (offset==ll-1) {
    ll=getline(linea,MAXLINEA); //trae una línea del programa fuente al buffer de líneas
    if (ll==0) 
      //se retrasa en un blanco la deteccion de EOF, porque obtoken lleva un caracter adelantado. si no, en   
      //algunos casos tendríamos problemas, por ejemplo: no se reconoceria el punto final del programa (...end.)

      fin_de_archivo=1; 		             
    printf("%s",linea);
    offset=-1;
  }

  ++offset;

  if ( (linea[offset]=='\0') || (fin_de_archivo==1) )   
    return(' '); 
  else {
    chNorm = linea[offset];
    return(toupper(linea[offset])); //de esto depende si el lenguaje es sensitivo de mayúsculas o no.
  }

}

//getline: lee la siguiente línea del fuente y regresa su tamaño (incluyendo '\n') o 0 si EOF. (por ejemplo: para VAR regresa 4)
//es probablemente la rutina de más bajo nivel del compilador
//tomada de "El Lenguaje de programación C" - Kernighan & Ritchie - pag 28                        
int getline(char s[],int lim) {
  int c,i;

  for (i=0;i<lim-1 && (c=getc(fp))!=EOF && c!='\n';++i)
    s[i]=c;

  if (c=='\n') {
    s[i]=c;
    ++i;
  }

  s[i]='\0';
  return (i);
}


