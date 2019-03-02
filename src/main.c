/* Somador de Floats
 *
 * Este programa recebera uma serie de caracteres contendo floats em seu conteúd
 * Ao receber um caractere fim de linha ('\n'), deve imprimir na tela a soma dos
 * floats válidos encontrados na entrada.
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 300

typedef struct {
  int currentState;
} fsm;

const char dlts[] = {' ','\0'};

int isDelimiter(char c, const char *dlts);
int isDigit(char c);
int isDot(char c);
int changeState(void *m, char input);


int main() {

  char *token;
  char input[MAX_LENGTH];
  float sum;
  int  pos;
  fsm  m;

  // Recieve data
  scanf("%[^\n]", input);

  // Initial conditions
  m.currentState = 1; // starts in 1 because the input doesn't need to start with a delimiter.
  token = input;
  pos = 0;
  sum = 0;

  // Process digit search using state machine
  do {
    changeState((fsm *)&m, input[pos]);
    // Delimiter found - set token begin
    if(m.currentState==1) token = input+pos+1;
    // Token found - set token end and process sum
    if(m.currentState==5) {
      input[pos] = '\0';
      //printf("Found Token: '%s'\n", token);      
      sum += atof(token);
      input[pos] = ' ';
      token = input+pos+1;
    }
  } while(input[pos++] != '\0');
  // Print result
  printf("%g\n", sum);
  return 0;
}

int isDigit(char c) {
  if(c >= 48 && c <= 57)
    return 1;
  return 0;
}

int isDelimiter(char c, const char *dlts) {
  do {
    if(c == *dlts)
      return 1;
  } while(*dlts++ != '\0');
  return 0;
}

int isDot(char c) {
  return c=='.';
}

int changeState(void *m, char input) {
  fsm *this_m = (fsm *)m;

  switch(this_m->currentState) {
  // Search for delimiter
  case 0:
    if(isDelimiter(input, dlts))
      this_m->currentState = 1;
    break;
  // Found delimiter, search for number
  case 1:
    if(isDigit(input)) {
      this_m->currentState = 2;
    }
    else if(!isDelimiter(input,dlts)) {
      this_m->currentState = 0;
    }
    break;
  // Found number(before dot), delimiter->'valid float', dot->'search number'
  case 2:
    if(isDelimiter(input, dlts)) {
      this_m->currentState = 5;
    }
    else if(isDot(input)) {
      this_m->currentState = 3;
    }
    else if(!isDigit(input)) {
      this_m->currentState = 0;
    }
    break;
  // Found dot, search for number
  case 3:
    if(isDelimiter(input,dlts)) {
      this_m->currentState = 1;
    }
    else if(isDigit(input)) {
      this_m->currentState = 4;
    }
    else {
      this_m->currentState = 0;
    }
    break;
  // Found number(after dot), search for delimiter
  case 4:
    if(isDelimiter(input, dlts)) {
      this_m->currentState = 5;      
    }
    else if(!isDigit(input)) {
      this_m->currentState = 0;
    }
    break;
  // Found delimiter, valid float. Test next state
  case 5:
    if(isDelimiter(input,dlts)) {
      this_m->currentState = 1;
    }
    else if(isDigit(input)) {
      this_m->currentState = 2;
    }
    else {
      this_m->currentState = 0;
    }
    break;
  }
  
  return this_m->currentState;
}
