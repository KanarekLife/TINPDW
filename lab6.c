#include <stdio.h>
//#include <stdlib.h>

int suma(int start, int end){
  unsigned long int wynik=0;
  for (int i = 0; i < start; i++) {
    for (int j = 0; j < end; j++) {
     wynik += i + j; }
  }
  return wynik;
}

int main () {
  int i,j,l;
  static int x[25000][25000];
  for (l=0;l<20;l++){
    for (i = 0; i < 25000; i++) {
      for (j = 0; j < 25000; j++) {
        x[j][i] = i*l + j; }
    }
  }
  i/=20;
  j/=20;
  printf("Suma= %d\n",suma(i,j));

  return 0;
}

