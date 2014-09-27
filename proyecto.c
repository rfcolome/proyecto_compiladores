
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 500
#define MAXLEX  100
#define TRUE    1
#define FALSE   !TRUE



typedef enum tokenName {
  NIL,
  SELFTOK,
  RESENDTOK,
  IDENTIFIER,
  SMALLKEYWORD,
  CAPKEYWORD,
  ARGUMENTNAME,
  STRING,
  COMMENT,
  INTEGER
} tokenName;


// data structs
typedef struct lineList {
  char line[MAXLINE + 1];
  struct lineList *nxt;
} lineList;

typedef struct token {
  char lexeme[MAXLEX+1];
  tokenName tok;
} token;

typedef struct tokenTree {
  struct token *tok;
  struct tokenTree *left;
  struct tokenTree *right;
} tokenTree;


// functions
int  readFile           (char *f, lineList *buffer);
void printSource        (lineList *source);
void deleteLineList     (lineList **list);

void getTokens          (lineList *source);

int  isWhitespace       (char c);
int  isLowerCase        (char c);
int  isUpperCase        (char c);
int  isSign             (char c);
int  isDigit            (char c);
int  isUnderscore       (char c);
int  isColon            (char c);
int  isQuote            (char c);
int  isDoubleQuote      (char c);
int  isAlphabetic       (char c);
int  isAlphaNumeric     (char c);
int  isIdentifierChar   (char c, int isFirstLetter);
int  isCapKeywordChar   (char c, int isFirstLetter);
int  isArgumentNameChar (char c, int currLexemeChar);
int  isIntegerChar      (char c, int isFirstLetter, int hasBase);

int  addToken           (char *lexeme, tokenName tok);
int  addTokenToTree     (tokenTree **tree, token *tok);
int  isLeaf             (tokenTree *tree);
int  treeHeight         (tokenTree *tree);
int  isBalanced         (tokenTree *tree);
int  balanceTree        (tokenTree **tree);
void displayTree        (tokenTree *tree);

int  max                (int a, int b);

// variables
lineList *source = NULL;
tokenTree *tree = NULL;

int main (int argc, char **argv) {
  // el argumento del programa es la direccion del archivo fuente
  if (argc != 2) {
    printf("se espera un argumento: La direccion de un archivo de código fuente.\n");
    return 0;
  }


  /*
   *  lectura del archivo.
   *  guardamos el archivo en memoria (una lista de "lineas" de MAXLINE chars c/u)
   */

  source = (lineList *)malloc(sizeof(lineList));
  if (source == NULL) {
    printf("no se pudo reservar memoria suficiente para el archivo.\n");
    return 0;
  }
  source -> nxt  = NULL;
  source -> line[0] = '\0';
  printf("archivo: %s\n", argv[1]);
  // leemos todos sus contenidos y los ponemos en la estructura de datos
  readFile(argv[1], source);
  printSource(source);


  /*
   *  hacemos el analisis lexico
   */
  printf("getting tokens...\n");
  getTokens(source);



  /*
   *  finalizamos la ejecucion.
   */
  deleteLineList(&source); // liberamos la memoria
  return 0;
}

int readFile(char *filename, lineList *buffer) {
  FILE *f = fopen(filename, "r");
  int  i  = 0;
  char c  = fgetc(f);

  while (c != EOF) {

    if (i >= MAXLINE) { // si ya llenamos este nodo de la lista
      buffer -> line[i] = '\0'; // ponemos un fin de cadena al final
      buffer -> nxt     = (lineList *)malloc(sizeof(lineList)); // memoria para otra linea
      if (buffer -> nxt == NULL) { // revisamos que la memoria se reservo
        printf("no se pudo reservar memoria suficiente para el archivo.");
        exit(0);
      }
      buffer = buffer -> nxt; // nos pasamos al nuevo espacio reservado para escribir ahi
      buffer -> nxt = NULL; // nos aseguramos de poner el puntero a nxt como NULL
      i = 0; // como es nuevo, el contador se reinicia.
    }

    buffer -> line[i] = c;
    c = fgetc(f);
    i++;
  }
  buffer->line[i] = '\0';
  fclose(f);

  return c != EOF;
}

void printSource(lineList *source) {
  printf("%s", source -> line);
  while (source -> nxt) {
    source = source -> nxt;
    printf("%s", source -> line);
  }
  return;
}

void deleteLineList(lineList **list) {
  if ((*list) -> nxt) {
    deleteLineList(&((*list) -> nxt));
  }
  free(*list);
  list = NULL;
}



void getTokens(lineList *source) {
  lineList *currLine          = source;
  int       currLineChar      = 0;
  int       shouldGetNextChar = TRUE;
  int       currLexemeChar    = 0;
  char      lexeme[MAXLEX+1];
  lexeme[0]                   = '\0';
  char      c;                          // c es el caracter actual
  tokenName currGuessedTok    = NIL;
  int       integerHasBase    = FALSE;
  
  // selftok       -> self
  // resendtok     -> resend
  // identifier    -> [a-z_][a-zA-Z0-9_]*
  // small-keyword -> [a-z_][a-zA-Z0-9_]*:
  // cap-keyword   -> [A-Z][a-zA-Z0-9_]:
  // argument-name -> :[a-z_][a-zA-Z0-9_]*
  // string        -> '[^']*'
  // comment       -> "[^"]"
  //
  // note: . is a token by itself...

  // while there is anything on the source code
  printf("about to enter while loop\n");
  while (currLine -> line[currLineChar] != '\0' || currLine -> nxt) {
    shouldGetNextChar = TRUE;
    if (currLine -> line[currLineChar] == '\0') {
      currLine = currLine -> nxt;
      currLineChar = 0;
    }
    c = currLine->line[currLineChar];

    if (currLexemeChar != 0) {
      if (currGuessedTok == IDENTIFIER) {
        if (isIdentifierChar(c, FALSE)) {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
        }
        else { // end of the identifier
          if (strcmp(lexeme, "self") == 0) {
            // the lexeme is a SELFTOK
            addToken(lexeme, SELFTOK);
            currLexemeChar = 0;
            lexeme[currLexemeChar] = '\0';
            shouldGetNextChar = FALSE;
          }
          else if (strcmp(lexeme, "resend") == 0) {
            // the lexeme is a RESENDTOK
            addToken(lexeme, RESENDTOK);
            currLexemeChar = 0;
            lexeme[currLexemeChar] = '\0';
            shouldGetNextChar = FALSE;
          }
          else if (isColon(c)) {
            lexeme[currLexemeChar] = c;
            lexeme[currLexemeChar+1] = '\0';
            addToken(lexeme, SMALLKEYWORD);
            currLexemeChar = 0;
            lexeme[currLexemeChar] = '\0';
          }
          else {
            // not a keyword and doesn't end in colon. It's an identifier
            addToken(lexeme, IDENTIFIER);
            currLexemeChar = 0;
            lexeme[currLexemeChar] = '\0';
            shouldGetNextChar = FALSE;
          }
        }
      }
      else if (currGuessedTok == CAPKEYWORD) {
        if (isCapKeywordChar(c, FALSE)) {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
        }
        else if (isColon(c)) { // end of the capital keyword
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
          addToken(lexeme, IDENTIFIER);
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
        }
        else {
          // wth is it?
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
          addToken(lexeme, NIL);
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
        }
      }
      else if (currGuessedTok == ARGUMENTNAME) {
        if (isArgumentNameChar(c, currLexemeChar)) {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
        }
        else {
          addToken(lexeme, ARGUMENTNAME);
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
          shouldGetNextChar = FALSE;          
        }
      }
      else if (currGuessedTok == STRING) {
        if (isQuote(c)) {
          // end of the string
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
          addToken(lexeme, STRING);
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
        }
        else {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
        }
      }
      else if (currGuessedTok == COMMENT) {
        if (isDoubleQuote(c)) {
          // end of the comment
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
          // we don't store it
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
        }
        else {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
        }
      }
      else if (currGuessedTok == INTEGER) {
        if (c == 'r' || c == 'R') {
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';
          integerHasBase = TRUE;
        }
        else if (isIntegerChar(c, FALSE, integerHasBase)){
          lexeme[currLexemeChar] = c;
          currLexemeChar++;
          lexeme[currLexemeChar] = '\0';          
        }
        else {
          // if it's not an integer character, we're done
          addToken(lexeme, INTEGER);
          currLexemeChar = 0;
          lexeme[currLexemeChar] = '\0';
          shouldGetNextChar = FALSE;
        }
      }
      else {
        // wth is this thing?
        printf("ERROR: unknown token for lexeme %s: %d\n", lexeme, currGuessedTok);
        exit(1);
      }

    }
    else { // starting out with a new lexeme
    // if I'm just starting out with a new lexeme,
    // guess if it's an identifier, a capkeyword,
    // argument name, string, or comment.
    // if it's nothing I'm aware of, mark it as 
    // weird and exit.
      if (isIdentifierChar(c, TRUE)) { // [a-z_]
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = IDENTIFIER; // could also be a small keyword
      }
      else if (isCapKeywordChar(c, TRUE)) {
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = CAPKEYWORD;
      }
      else if (isArgumentNameChar(c, currLexemeChar)) {
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = ARGUMENTNAME;
      }
      else if (isQuote(c)) {
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = STRING;
      }
      else if (isDoubleQuote(c)) {
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = COMMENT;
      }
      else if (isWhitespace(c)) {
        // nothing to see here... carry on...
      }
      else if (isIntegerChar(c, TRUE, FALSE)) {
        // beginning of a number?
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        integerHasBase = FALSE;
        currGuessedTok = INTEGER;
      }
      // TODO: add all the operators
      else {
        // it's nothing I'm aware of.... NIL!
        lexeme[currLexemeChar] = c;
        currLexemeChar++;
        lexeme[currLexemeChar] = '\0';
        currGuessedTok = NIL;
        addToken(lexeme, currGuessedTok);
        lexeme[0] = '\0';
        currLexemeChar = 0;
      }
    }
    if (shouldGetNextChar) currLineChar++; // move on to the next character...
  }
  // end of the while loop; if there's a dangling lexeme, add it.

  // If I'm currently scanning a lexeme but the current
  // char doesn't match anything, then add a \0 to the lexeme,
  // put the lexeme
  // in the symbol table (with its token), 
  // set the current lexeme to the
  // empty string, and iterate.

  // if I reached the end of the source code, return
  // the list of tokens?
  displayTree(tree);
}


int isWhitespace (char c) {
  return c == ' ' || c == '\n' || c == '\t';
}

int isLowerCase (char c) {
  return c >= 'a' && c <= 'z';
}

int isUpperCase (char c) {
  return c >= 'A' && c <= 'Z';
}

int isSign (char c) {
  return c == '+' || c == '-';
}

int isDigit (char c) {
  return c >= '0' && c <= '9';
}

int isUnderscore (char c) {
  return c == '_';
}

int isColon (char c) {
  return c == ':';
}

int isQuote (char c) {
  return c == '\'';
}

int isDoubleQuote (char c) {
  return c == '"';
}

int isAlphabetic (char c) {
  return isUpperCase(c) || isLowerCase(c);
}

int isAlphaNumeric (char c) {
  return isAlphabetic(c) || isDigit(c);
}

int isIdentifierChar (char c, int isFirstLetter) {
  return isUnderscore(c) || ((isFirstLetter) ? isLowerCase(c) : isAlphaNumeric(c));
}

int isCapKeywordChar (char c, int isFirstLetter) {
  return (isFirstLetter) ? isUpperCase(c) : (isAlphaNumeric(c) || isUnderscore(c));
}

int isArgumentNameChar (char c, int currLexemeChar) {
  return (currLexemeChar == 0) ? isColon(c) : isIdentifierChar(c, currLexemeChar == 1);
}

int isIntegerChar (char c, int isFirstLetter, int hasBase) {
  // /([0-9]+)|([0-9][0-9]?[rR][0-9a-zA-Z]+)/
  if (isFirstLetter) {
    return isDigit(c);
  }
  else if (hasBase){
    return isDigit(c) || isAlphabetic(c);
  }
  else {
    return isDigit(c) || c == 'r' || c == 'R';
  }
}


// adds token to token stream
int addToken (char *lexeme, tokenName tok) { 
  // allocate memory for a new token, put the data in there
  // and store it on the datastructure.
  token *temp = (token *)malloc(sizeof(token));
  strcpy(temp -> lexeme, lexeme);
  temp -> tok = tok;
  printf("%s: %i\n", lexeme, tok);
  return addTokenToTree(&tree, temp);
}

int addTokenToTree (tokenTree **tree, token *tok) {
  tokenTree *temp = (tokenTree *)malloc(sizeof(tokenTree));
  temp -> tok     = tok;
  temp -> left    = NULL;
  temp -> right   = NULL;
  if ((*tree) == NULL) {
    *tree = temp;
    return 0;
  }
  else {
    // strcmp(a, b) <  0 iff a <  b
    // strcmp(a, b) == 0 iff a == b
    // strcmp(a, b) >  0 iff a >  b
    if (strcmp(((*tree) -> tok) -> lexeme, tok -> lexeme) >= 0) {
      return addTokenToTree(&((*tree) -> left), tok);
    }
    else {
      return addTokenToTree(&((*tree) -> right), tok);
    }
  }
}

int isLeaf (tokenTree *tree) {
  return tree -> left == NULL && tree -> right == NULL;
}

int treeHeight (tokenTree *tree) {
  if (tree == NULL) {
    return 0;
  }
  else {
    return 1 + max(treeHeight(tree -> left), treeHeight(tree -> right));
  }
}

int isBalanced (tokenTree *tree) {
  return tree == NULL || abs(treeHeight(tree -> left) - treeHeight(tree -> right)) <= 1;
}

int balanceTree (tokenTree **tree) {
  // TODO
  return 0;
}

void displayTree (tokenTree *tree) {
  if (tree != NULL) {
    displayTree(tree -> left);
    printf("%s\n", tree -> tok -> lexeme);
    displayTree(tree -> right);
  }
}


int max (int a, int b) {
  return (a > b) ? a : b ;
}
