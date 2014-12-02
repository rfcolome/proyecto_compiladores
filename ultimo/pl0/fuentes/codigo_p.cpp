#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "codigo_p.h"
#include "parametros.h"
#include "auxiliares.h"

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de código-p
int ic;                          //índice sobre el array de código 

//gen
//coloca una instrucción más en el array de código intermedio
//ic se deja al final una posición adelante.                    
void gen (enum fcn x,int y,int z,float r) {
  if (ic > MAXIC-1) {
    error(33); //error fatal: no queda más que terminar la compilación por una limitación específica del compilador
    //ya no hay más espacio para el código-p
    estadisticas();
    fclose(fp);
    exit(1);   // el error es fatal=exit
  }

  //generar una instrucción de código-p
  codigo[ic].f  = x;
  codigo[ic].ni = y;
  codigo[ic].di = z;
  codigo[ic].r  = r;

  ++ic;
}

//listar_p: emitir listado de código-p
void listar_p() {
  int i;

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
  
  printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");

  for(i=0;i<ic;++i) {
    printf("\n %4d  %3s %5d %5d %5f %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,codigo[i].r,comentario[codigo[i].f]);
  }
}

//escribe_codigop: escribe un archivo con extensión .p conteniendo el código-p generado
void escribe_codigop(char *fuente) {
  //construir nombre de archivo con el mismo nombre del fuente pero con extensión .p

  FILE *obj;
  FILE *objtxt;

  char *ptr;
  char codigo_p[TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P];
  char codigo_p_texto[TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P];
  int pos,i;

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


  //buscar un punto en el nombre del archivo fuente
  //en ptr queda la posición que ocupa el caracter a buscar
  ptr = strchr(fuente, '.');
  if (ptr) {
    //si el apuntador es distinto de NULL,hay un punto en el nombre del fuente
    //encontramos la posición del punto, relativa al inicio del string 
    pos=ptr-fuente; 

    //dejamos en "codigo_p" solo el nombre del archivo (sin el punto ni la extensión)
    strncpy(codigo_p, fuente, pos);
    codigo_p[pos] = '\0';
    strcpy(codigo_p_texto, "solcodigop.txt");

  }
  else {
    strcpy(codigo_p,fuente); //el fuente no tiene apellido
    strcpy(codigo_p_texto,fuente);
  }

  //finalmente,colocamos la extensión deseada
  strcat(codigo_p,".obp");

  //grabar en el disco el archivo con código_p,a partir del array código creado por el generador de código intermedio
  if  ( (obj=fopen(codigo_p,"w+b"))==NULL ||  (objtxt=fopen(codigo_p_texto,"w+"))==NULL ) {
    printf("\nOcurrio un error al intentar escribir el código intermedio (código-p)"); //error fatal.
    fclose(fp);
    fclose(obj);
    fclose(objtxt);
    exit(1);
  }

  for(i=0;i<ic;++i) {
    fwrite(&codigo[i],sizeof(codigo_intermedio),1,obj);
    fprintf(objtxt," %4d  %3s %5d %5d %5f %s\n",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,codigo[i].r,comentario[codigo[i].f]);
  }

  fclose(obj);
}

