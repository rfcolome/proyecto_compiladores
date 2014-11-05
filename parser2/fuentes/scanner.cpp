//el analizador lexicogr�fico de pl0
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "parametros.h"
#include "scanner.h"
#include "lexico.h"
#include "auxiliares.h"

char linea[MAXLINEA];     //buffer de l�neas 
int ll;                   //contador de caracteres
int offset;               //corrimiento en la lectura de los caracteres del programa fuente
int fin_de_archivo;       //bandera de fin de archivo (obtch)
int ch;                   //�ltimo caracter le�do
char lex[MAXID+1];        //�ltimo lexeme le�do ( +1 para colocar "\0")
long double valor ;       //valor num�rico de una lexeme correspondiene a un n�mero

int obtch(),getline(char s[],int lim); //funciones internas a scanner.cpp

//obtoken: obtiene el siguiente token del programa fuente
void obtoken()
{
  char lexid[MAXID+1]; //+1 para colocar el marcador "\0"
  int i,j;
  int ok=0;

  //quitar blancos, caracter de cambio de l�nea y tabuladores
  while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;

  if (ch == '{') {
    // es un comentario. Por lo tanto seguimos leyendo hasta encontrar un }
    while (ch != '}'){
      ch = obtch();
    }
    ch = obtch();
  }

  // despues del comentario podr�a haber aun mas espacio en blanco, asi que
  // tambien lo quitamos
  while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;

  // detectamos si es una cadena de caracteres
  if (ch == '"') {
    lexid[0] = ch;
    i = 1;
    ch = obtch();
    while (ch != '"') {
      if (i<MAXID) {
        lexid[i++]=ch;
      }
      ch = obtch();
    }
    if (i<MAXID) {
      lexid[i++]=ch;
    }
    lexid[i]='\0';
    token=string; //es cadena
    strcpy(lex,lexid); //copiar en lex
    ch = obtch();
  }
  //si la lexeme comienza con una letra, es identificador o palabra reservada
  else if (isalpha(ch)) {
    lexid[0]=ch;
    i=1;
    while ( isalpha( (ch=obtch()) ) ||  isdigit(ch)   )
      if (i<MAXID) lexid[i++]=ch;
    lexid[i]='\0';

    //�es identificador o palabra reservada?.buscar en la tabla de palabras reservadas
    //una b�squeda lineal que tendr� que ser sustitu�da por otro tipo de b�squeda m�s efectiva.
    //...en esa nueva b�squeda desaparecer� el "break"
    for (j=0;j<MAXPAL;++j)
      if (strcmp(lexid,lexpal[j])==0) {
        ok=1;
        break;
      }

    if (ok==1)
      token=tokpal[j]; //es palabra reservada
    else
      token=ident; //es identificador

    strcpy(lex,lexid); //copiar en lex
  }
  //si comienza con un d�gito...
  else if (isdigit(ch)) {
    lexid[0]=ch;
    i=j=1;
    while ( isdigit( (ch=obtch()))) {
      if (i<MAXDIGIT) lexid[i++]=ch;
      j++;
    }
    lexid[i]='\0';
    if (j>MAXDIGIT)
      error(30); //este n�mero es demasiado grande
    token=entero;

    // hasta aqui, asumo que estaba leyendo un numero entero.
    // si luego me encuentro con un punto y mas numeros, es
    // un numero real.
    if (ch == '.') {
      if (i<MAXDIGIT) lexid[i++]=ch;
      j++;
      while ( isdigit( (ch=obtch()))) {
        if (i<MAXDIGIT) lexid[i++]=ch;
        j++;
      }
      lexid[i]='\0';
      if (!isdigit(lexid[i-1])) {
        // vi un numero seguido de un punto, seguido de algo que
        // no es un digito... error?
        error(25); // se esperaban digitos luego del punto
      }
      else {
        // lei el numero real y todo bien.
        if (j>MAXDIGIT)
          error(30); //este n�mero es demasiado grande
        token=real;
      }
    }
    valor=strtod(lexid, NULL); //valor num�rico de una lexeme correspondiene a un n�mero
  }
  //reconocimiento de s�mbolos especiales, incluyendo pares de simbolos (aplicamos "lookahead symbol technique")
  else if (ch=='<') {
    ch=obtch();
    if (ch=='=') {
      token=mei;
      ch=obtch();
    }
    else
      if (ch=='>') {
        token=nig;
        ch=obtch();
      }
      else
        token=mnr;
  }
  else {
    if (ch=='>') {
      ch=obtch();
      if (ch=='=') {
        token=mai;
        ch=obtch();
      }
      else
        token=myr;
    }
    else {
      if (ch==':') {
        ch=obtch();
        if (ch=='=') {
          token=asignacion;
          ch=obtch();
        }
        else
          token=dospuntos;
      }
      else {
        token=espec[ch]; //hashing directo en la tabla de tokens de s�mbolos especiales del lenguaje
        ch=obtch();
      }
    }
  }
}

//obtch: obtiene el siguiente caracter del programa fuente
int obtch()
{

  if (fin_de_archivo==1) {
    error(32);//error 32: el programa fuente esta incompleto
    estadisticas();
    fclose(fp);
    exit(1); //el error es fatal
  }

  if (offset==ll-1) {
    ll=getline(linea,MAXLINEA); //trae una l�nea del programa fuente al buffer de l�neas
    if (ll==0)
      //se retrasa en un blanco la deteccion de EOF, porque obtoken lleva un caracter adelantado. si no, en
      //algunos casos tendr�amos problemas, por ejemplo: no se reconoceria el punto final del programa (...end.)

      fin_de_archivo=1;
    printf("%s",linea);
    offset=-1;
  }

  ++offset;

  if ( (linea[offset]=='\0') || (fin_de_archivo==1) )
    return(' ');
  else
    return(toupper(linea[offset])); //de esto depende si el lenguaje es sensitivo de may�sculas o no.

}

//getline: lee la siguiente l�nea del fuente y regresa su tama�o (incluyendo '\n') o 0 si EOF. (por ejemplo: para VAR regresa 4)
//es probablemente la rutina de m�s bajo nivel del compilador
//tomada de "El Lenguaje de programaci�n C" - Kernighan & Ritchie - pag 28
int getline(char s[],int lim)
{
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
