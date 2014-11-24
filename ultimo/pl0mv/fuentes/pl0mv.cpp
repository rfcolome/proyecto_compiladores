//máquina virtual (intérprete-ejecutor) de código-p simplificado
//se usa una máquina de stack para realizar la ejecución del código
//la implementación del stack está desguarnecida: no se ha implementado 
//ningún nivel de abstracción ni de validación
//noviembre 2012,abril de 2011
//se usó Microsoft Visual C++ 2010 para escribir esta mv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXIC     200  //tamaño máximo del array de código-p
#define LONGSTACK 500  //tamaño-longitud del stack de datos                    

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,REA,IMP};

typedef struct {
 enum fcn f; //mnemónico
 int     ni; //nivel (0..MAXNIV)
 int     di; //dirección o desplazamiento (0..32767)
} codigo_intermedio;

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
int ic;                          //índice sobre el array de código-p

FILE *obj; //apuntador al archivo de código intermedio

int p[LONGSTACK] ; //memoria de datos-stack

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
  ";instruccion de impresion en pantalla"
};

//prototipos de funciones
void interpretar(void),listar_p(void);
int base(int ni,int b);

//main: inicia la ejecución
int main(int argc,char *argv[]) {
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
 p[0]=p[1]=p[2]=0;   //ED,EE y DR para el programa principal
 
 //bucle de ejecución
 do {
  
    i=codigo[d++];   
    printf("\n\nejecutando la instruccion %4d: %3s %5d %5d",d-1,mnemonico[i.f],i.ni,i.di); 
   
    switch(i.f) {
          case LIT:
               p[++s]=i.di;
	           printf("\nLIT : cargando la literal %d en la direccion %d (s en %d)",i.di,s,s);
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
	                    printf("- unario para %d (s en %d)",p[s],s);
                        p[s]=-p[s];
                        break;

                   case 2:
                        --s;
	                    printf("suma de %d + %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]+p[s+1];
	                    break;

                   case 3:
	                    --s;
                        printf("resta de %d - %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]-p[s+1];
                        break;

                   case 4:
	                    --s;
                        printf("multiplicacion de %d * %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]*p[s+1];
                        break;

                   case 5:
                        --s;
	                    printf("division entera de %d / %d (s en %d)",p[s],p[s+1],s);
                        if(p[s+1]==0) {
						  fclose(obj);
	                      printf("\nError a tiempo de ejecucion\nSe intenta dividir entre cero");
	                      printf("\n--Programa abortado--");
	                      exit(1); //el error es fatal
                        }
                        p[s]=p[s]/p[s+1];
                        break;

                   case 6:
                        printf("odd(%d)? (s en %d)",p[s],s);
                        p[s]=(p[s]%2!=0);
       
                   case 7: //sin uso
                        break;

                   case 8:
	                    --s;
                        printf("%d=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]==p[s+1]);
                        break;

                   case 9:
                        --s;
                        printf("%d!=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]!=p[s+1]);
                        break;

                   case 10:
                        --s;
                        printf("%d<%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]<p[s+1]);
                        break;

                   case 11:
                        --s;
                        printf("%d>=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]>=p[s+1]);
                        break;

                   case 12:
                        --s;
                        printf("%d>%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]>p[s+1]);
                        break;

                   case 13:
	                    --s;
                        printf("%d<=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]<=p[s+1]);
                        break;       
               };
               break;
    
          case CAR:
               p[++s]=p[base(i.ni,b)+i.di];
	           printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.di],s,s);
               break;

          case ALM:
               printf("\nALM : almacenando %d en la direccion %d (s en %d)",p[s],base(i.ni,b)+i.di,s-1);
               p[base(i.ni,b)+i.di]=p[s];
               --s;
               break;

          case REA:
               printf("\nREA : leyendo un numero en la direccion %d (s en %d)",p[base(i.ni,b)+i.di],s);
               printf("\n");
               scanf("%d",&(p[base(i.ni,b)+i.di]));
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
                 printf("\n%d",p[s]);
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
int base(int ni,int b)
{
 int b1;
 b1=b;
 while (ni>0) {
       b1=p[b1];
       --ni;
 }
 return (b1);
}

//listar_p: emitir listado de código-p
void listar_p()
{
 int i;
   
 printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");
  
 for(i=0;i<ic;++i) {
     printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,comentario[codigo[i].f]);
 }
}


