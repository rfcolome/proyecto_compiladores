extern char linea[MAXLINEA]; //buffer de l�neas
extern int ll;               //contador de caracteres
extern int offset;           //corrimiento en la lectura de los caracteres del programa fuente (se usa en scanner.cpp y auxiliares.cpp)
extern int fin_de_archivo;   //bandera de fin de archivo (se usa en pl0.cpp y scanner.cpp)
extern int ch;               //�ltimo caracter le�do (se usa en pl0.cpp y scanner.cpp)
extern char lex[MAXID+1];    //�ltimo lexeme le�do ( +1 para colocar "\0")
extern long double valor;    //valor num�rico de una lexeme correspondiene a un n�mero

void obtoken();
