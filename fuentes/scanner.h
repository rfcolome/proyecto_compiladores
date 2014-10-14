extern char linea[MAXLINEA]; //buffer de líneas
extern int ll;               //contador de caracteres
extern int offset;           //corrimiento en la lectura de los caracteres del programa fuente (se usa en scanner.cpp y auxiliares.cpp)
extern int fin_de_archivo;   //bandera de fin de archivo (se usa en pl0.cpp y scanner.cpp)
extern int ch;               //último caracter leído (se usa en pl0.cpp y scanner.cpp)
extern char lex[MAXID+1];    //último lexeme leído ( +1 para colocar "\0")
extern long double valor;    //valor numérico de una lexeme correspondiene a un número

void obtoken();
