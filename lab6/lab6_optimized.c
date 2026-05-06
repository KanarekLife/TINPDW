#include <stdio.h>
#include <omp.h>

// Stanisłáw Nieradko 193044


/*
  kanareklife@workstation:~/Projects/TINPDW/lab6$ clang lab6_optimized.c -O3 -fopenmp -march=native -o lab6 && time ./lab6 && rm lab6
  Suma= 1951562500

  real    0m2.836s
  user    0m2.051s
  sys     0m0.766s
*/

int suma(int start, int end){
  unsigned long int wynik = 0;
  
  // zrównoleglamy pętlę
  #pragma omp parallel for reduction(+:wynik)
  for (int i = 0; i < start; i++) {
    unsigned long int row_sum = 0;
    
    // wymuszamy simd
    #pragma omp simd reduction(+:row_sum)
    for (int j = 0; j < end; j++) {
      row_sum += (unsigned long int)i + j;
    }
    wynik += row_sum;
  }
  return (int)wynik;
}

int main () {
  int i, j, l;
  static int x[25000][25000];

  for (l = 0; l < 20; l++){
    
    // zrównoleglamy pętlę
    #pragma omp parallel for private(j)
    for (i = 0; i < 25000; i++) {
      
      // wymuszamy simd
      #pragma omp simd
      for (j = 0; j < 25000; j++) {
        
        // było x[j][i] (skakało po kolumnach) -> x[i][j] zapewnia odczyt i zapis liniowy.
        x[i][j] = i * l + j; 
      }
    }
  }

  // przez zrównoleglenie i oraz j miały złe wartości -> przywracamy
  i = 25000;
  j = 25000;
  i /= 20;
  j /= 20;

  printf("Suma= %d\n", suma(i, j));

  return 0;
}