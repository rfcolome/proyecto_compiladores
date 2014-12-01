//máquina virtual (intérprete-ejecutor) de código-p simplificado
//se usa una máquina de stack para realizar la ejecución del código
//la implementación del stack está desguarnecida: no se ha implementado 
//ningún nivel de abstracción ni de validación
//noviembre 2012,abril de 2011
//se usó Microsoft Visual C++ 2010 para escribir esta mv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAXIC     200  //tamaño máximo del array de código-p
#define LONGSTACK 500  //tamaño-longitud del stack de datos                    

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP,HLT};

typedef struct {
  enum fcn f; //mnemónico
  int     ni; //nivel (0..MAXNIV)
  int     di; //dirección o desplazamiento (0..32767)
  float     r; //numero real
} codigo_intermedio;

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
int ic;                          //índice sobre el array de código-p

FILE *obj; //apuntador al archivo de código intermedio

int p[LONGSTACK]  ; //memoria de datos-stack
float pr[LONGSTACK] ; //memoria de datos-stack de numeros reales

//arrays para mostrar como string el mnemónico de la instrucción
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC","REA","IMP","HLT"};

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
  ";instruccion de impresion en pantalla",
  ";instruccion para detener la ejecucion"
};

//prototipos de funciones
void interpretar(void),listar_p(void);
int base(int ni,int b);
void imprimirStack (void);

//main: inicia la ejecución
int main(int argc,char *argv[]) {
  srand (time(NULL));
 if (argc!=2) //analizar argumentos de main
    printf("\nuso: pl0mv nombre_de_archivo");
 else  
    if ( (obj=fopen(argv[1],"r+b"))==NULL ) 
       printf("\nerror al abrir archivo %s",argv[1]);
	else {
       //leer el código desde el archivo hacia un array en memoria
       ic=0;
       fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
       while ( !feof(obj) ) {
             ++ic;
             fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
       }
	   //cierra el archivo
       fclose(obj);

       listar_p(); //quita el comentario de esta línea si deseas verificar que el código se cargo al array correctamente

	   //interpreta el codigo-p 
	   interpretar();   	
    }
   return(0);
 }

//interpretar: interpreta código-p                
void interpretar(void) {

  register int d;       //registro de dirección del programa: apunta a la siguiente instrucción por ejecutar
  register int b;       //registro de dirección base
  register int s;       //apuntador al tope del stack
  codigo_intermedio i;  //registro de instrucción: contiene la siguiente instrucción a ejecutar		     
			
  s=-1;b=0;d=0;
  p[0]=pr[0]=p[1]=pr[1]=p[2]=pr[2]=0;   //ED,EE y DR para el programa principal
 
  //bucle de ejecución
  do {
    int   vali1, vali2;
    float valf1, valf2;
    i=codigo[d++];   
    printf("\n\nejecutando la instruccion %4d: %3s %5d %5d %5.2f",d-1,mnemonico[i.f],i.ni,i.di,i.r); 

    switch(i.f) {
    case LIT:
      p[++s]=i.di;
      pr[s]=i.r;
      vali1 = i.di ? i.di : i.r;
      printf("\nLIT : cargando la literal %d en la direccion %d (s en %d)",vali1,s,s);
      break;

    case OPR:
      printf("\nOPR : ");
      //determinar de que operador se trata
      switch(i.di) {  
      case 0: //retornar o fin
        s=--b;
        d=p[s+3];
        b=p[s+2];
        printf("retornar a la instruccion %d, base=%d (s en %d)",d,b,s);
        break;

      case 1:
        vali1 = p[s] ? p[s] : pr[s];
        printf("- unario para %d ",vali1);
        printf("(s en %d)",s);
        p[s]=-p[s];
        pr[s]=-pr[s];
        break;

      case 2:
        --s;
        if(p[s]) {
          valf1 = p[s];
        }
        else {
          valf1 = pr[s];
        }
        
        if(p[s+1]) {
          valf2 = p[s+1];
        }
        else {
          valf2 = pr[s+1];
        }

        printf("suma de %d + %d ", valf1, valf2);
        printf("(s en %d)",s);
        
        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]+p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s]   ? p[s]   : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = 0;
          pr[s] = valf1 + valf2;
        }
        break;

      case 3:
        --s;
        printf("resta de %d - %d ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]-p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s]   ? p[s]   : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = 0;
          pr[s] = valf1 - valf2;
        }
        break;

      case 4:
        --s;
        printf("multiplicacion de %d * %d ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]*p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s] ? p[s] : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = 0;
          pr[s] = valf1 * valf2;
        }

        break;

      case 5:
        --s;
        printf("division entera de %d / %d ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);
        if(p[s+1]==0 && fabs(pr[s+1]) < 0.001) {
          fclose(obj);
          printf("\nError a tiempo de ejecucion\nSe intenta dividir entre cero");
          printf("\n--Programa abortado--");
          exit(1); //el error es fatal
        }

        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]/p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s]   ? p[s]   : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = 0;
          pr[s] = valf1 / valf2;
        }

        break;

      case 6:
        printf("odd(%d)? (s en %d)",p[s],s);
        p[s]=(p[s]%2!=0);
        pr[s] = 0; // un numero real no es impar
       
      case 7: //sin uso
        break;

      case 8:
        --s;
        printf("%d=%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]==p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s]   ? p[s]   : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = valf1 == valf2;
          pr[s] = 0;
        }

        break;

      case 9:
        --s;
        printf("%d!=%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        if (fabs(pr[s]) < 0.001 && fabs(pr[s+1]) < 0.001) {
          p[s] = p[s]!=p[s+1];
          pr[s] = 0;
        }
        else {
          valf1 = p[s]   ? p[s]   : pr[s];
          valf2 = p[s+1] ? p[s+1] : pr[s+1];

          p[s] = valf1 != valf2;
          pr[s] = 0;
        }

        break;

      case 10:
        --s;
        printf("%d<%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);


        valf1 = p[s]   ? p[s]   : pr[s];
        valf2 = p[s+1] ? p[s+1] : pr[s+1];

        p[s] = valf1 < valf2;
        pr[s] = 0;

        break;

      case 11:
        --s;
        printf("%d>=%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        valf1 = p[s]   ? p[s]   : pr[s];
        valf2 = p[s+1] ? p[s+1] : pr[s+1];

        p[s] = valf1 >= valf2;
        pr[s] = 0;

        break;

      case 12:
        --s;
        printf("%d>%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        valf1 = p[s]   ? p[s]   : pr[s];
        valf2 = p[s+1] ? p[s+1] : pr[s+1];

        p[s] = valf1 > valf2;
        pr[s] = 0;

        break;

      case 13:
        --s;
        printf("%d<=%d? ",
               (p[s]   ? p[s]   : pr[s]  ),
               (p[s+1] ? p[s+1] : pr[s+1]));
        printf("(s en %d)",s);

        valf1 = p[s]   ? p[s]   : pr[s];
        valf2 = p[s+1] ? p[s+1] : pr[s+1];

        p[s] = valf1 <= valf2;
        pr[s] = 0;

        break;

      case 14: // operacion RND
        printf("RND(%d) ",(p[s] ? p[s] : pr[s]));
        printf("(s en %d)",s);

        pr[s] = rand() % (int)(p[s] ? p[s] : pr[s]);
        p[s] = 0;
        break;

      case 15: // operacion CLRSCR
        printf("CLRSCRN()");
        system("cls");
        break;

      case 16: // operacion PITAG
        s -= 2;
        printf("PITAG(%d,%d,%d) (s en %d)",p[s],p[s+1],p[s+2],s);
        int hip;
        int cat1;
        int cat2;
        if (p[s] > p[s+1]) {
          if (p[s] > p[s+2]) { // hip = s
            hip = p[s];
            cat1 = p[s+1];
            cat2 = p[s+2];
          }
          else { // hip = s+2
            hip = p[s+2];
            cat1 = p[s+1];
            cat2 = p[s];
          }
        }
        else {
          if (p[s+1] > p[s+2]) { // hip = s+1
            hip = p[s+1];
            cat1 = p[s+2];
            cat2 = p[s];
          }
          else { // hip = s+2
            hip = p[s+2];
            cat1 = p[s+1];
            cat2 = p[s];
          }
        }
        p[s] = hip*hip == cat1*cat1 + cat2*cat2;
        pr[s] = 0;
        break;
      };
      break;

    case CAR:
      p[++s]=p[base(i.ni,b)+i.di];
      pr[s]=pr[base(i.ni,b)+i.di];
      printf("\nCAR : cargando %d ",(p[base(i.ni,b)+i.di] ? p[base(i.ni,b)+i.di] : pr[base(i.ni,b)+i.di]));
      printf("en la direccion %d (s en %d)",s,s);
      break;

    case ALM:
      vali1 = p[s] ? p[s] : pr[s];
      printf("\nALM : almacenando %d ",vali1);
      printf("en la direccion %d (s en %d)",base(i.ni,b)+i.di,s-1);
      p[base(i.ni,b)+i.di]=p[s];
      pr[base(i.ni,b)+i.di]=pr[s];
      --s;
      break;

    case REA:
      printf("\nREA : leyendo un numero en la direccion %d (s en %d)",p[base(i.ni,b)+i.di],s);
      printf("\n");
      scanf("%f",&(pr[base(i.ni,b)+i.di]));
      p[base(i.ni,b)+i.di] = pr[base(i.ni,b)+i.di];
      while (getchar() != '\n');  // limpiamos el buffer
      break;

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

      case 2: // real
        printf("\n%f",pr[s]);
        s--;
        break;

      case 3: // booleano
        if (p[s])
          printf("\ntrue");
        else
          printf("\nfalse");
        s--;
        break;
      };
      break;

    case HLT:
      printf("\nHALT : deteniendo la ejecución del programa");
      exit(0);
      break;

    case LLA:
      //generar un nuevo bloque
      p[s+1]=base(i.ni,b);
      p[s+2]=b;
      p[s+3]=d;
      printf("\nLLA : activando subrutina, enlaces y DR: %d %d %d",p[s+1],p[s+2],p[s+3]);

      b=s+1;d=i.di;
      printf("\n    : nueva base %d, instruccion %d (s en %d)",b,d,s);
      break;

    case INS:
      printf("\nINS : asignando %d espacios en el stack (s en %d)",i.di,s+i.di);
      s+=i.di;
      break;

    case SAL:
      printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.di,s);
      d=i.di;
      break;

    case SAC:
      printf("\nSAC : ");
      if (p[s]==0) {
        d=i.di;
        printf("la condicion es falsa. saltando condicionalmente a la instruccion %d.",d);
      }
      else
        printf("la condicion es verdadera. prosigo en la instruccion %d",d);

      --s;
      printf("(s en %d)",s);
      break;
    };

  } while (d!=0);
}
 
//base: encuentra la base ni niveles más abajo en el stack para la operación actual (CAR,ALM,LLA)
int base(int ni,int b) {
  int b1;
  b1=b;
  while (ni>0) {
    b1=p[b1];
    --ni;
  }
  return (b1);
}

//listar_p: emitir listado de código-p
void listar_p() {
  int i;

  printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");

  for(i=0;i<ic;++i) {
    printf("\n %4d  %3s %5d %5d %5f %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,codigo[i].r,comentario[codigo[i].f]);
  }
}


void imprimirStack (void) {
  int i;
  for (i=0; i<=20; i++) {
    printf("\n %4d %4d %5f", i, p[i], pr[i]);
  }
}
