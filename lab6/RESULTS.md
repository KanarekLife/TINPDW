# Wyniki optymalizacji - Laboratorium 6

## Specyfikacja środowiska
- Kompilator: `gcc (GCC) 15.2.1` / `clang version 21.1.8` / `icpx 2026.0`
- Procesor: AMD (AuthenticAMD)
- System operacyjny: Linux

## Zmierzone czasy wykonania

| Wersja programu | Flagi kompilacji | Czas (real) | Przyspieszenie | Uwagi |
| :--- | :--- | :--- | :--- | :--- |
| **Baseline** (`lab6.c`) | `-O0` | **35.275s** | 1x | Kod oryginalny, dostęp do pamięci `x[j][i]` |
| **Cache Optimized** | `-O0` | **12.475s** | ~2.8x | Zamiana kolejności pętli na `x[i][j]` |
| **OpenMP** | `-O0 -fopenmp` | **2.500s** | ~14.1x | Równoleglenie pętli (wielowątkowość) |
| **O3 Optimized** | `-O3` | **0.001s** | ~35000x | Dead Code Elimination (pętla main usunięta) |

## Benchmark rzeczywistych obliczeń

Wersja `lab6_optimized.c` zawiera pełny zestaw optymalizacji (cache, OpenMP, SIMD). Należy jednak pamiętać, że przy flagach `-O2` lub `-O3`, nowoczesne kompilatory wykrywają, iż operacje w pętli `main` nie wpływają na wynik funkcji `suma` i mogą je całkowicie usunąć (**Dead Code Elimination**), co skutkuje czasem wykonania rzędu **0.001s**. 

Aby zmierzyć realną wydajność pętli i uniknąć ich usuwania, przeprowadzono testy na wersji z wymuszonym odczytem pamięci. Wszystkie trzy kompilatory osiągnęły wtedy czas ok. **2.3s**, co stanowi realne **15-krotne przyspieszenie** względem wersji bazowej.

| Kompilator | Flagi Peak Performance | Czas (real) | Przepustowość |
| :--- | :--- | :--- | :--- |
| **GCC 15** | `-O3 -fopenmp -march=native` | **2.310s** | ~215 GB/s |
| **Clang 21** | `-O3 -fopenmp -march=native` | **2.282s** | ~218 GB/s |
| **ICPX 2026** | `-O3 -qopenmp -march=core-avx2` | **2.329s** | ~213 GB/s |

## Instrukcje kompilacji

Najlepszą wersję programu (`lab6_optimized.c`) można skompilować za pomocą poniższych komend:

### GCC
```bash
gcc lab6/lab6_optimized.c -O3 -fopenmp -march=native -o lab6_opt
```

### Clang
```bash
clang lab6/lab6_optimized.c -O3 -fopenmp -march=native -o lab6_opt
```

### Intel ICPX
```bash
icpx lab6/lab6_optimized.c -O3 -qopenmp -march=native -o lab6_opt
```
*Uwaga: W przypadku ICPX na systemach z procesorem AMD, należy użyć flagi `-march=core-avx2` zamiast `-native`.*

## Zastosowane techniki

1.  **Optymalizacja dostępu do pamięci (Cache Alignment):**
    Zmiana kolejności indeksowania na `x[i][j]` umożliwiła sekwencyjny odczyt/zapis i lepsze wykorzystanie cache L1/L2.
2.  **Równoleglenie (OpenMP):**
    Dodano dyrektywy `#pragma omp parallel for`, co pozwoliło rozdzielić iteracje pętli na wiele rdzeni procesora.
3.  **Wektoryzacja (SIMD):**
    Zastosowano `#pragma omp simd` oraz flagi sprzętowe, aby umożliwić wykorzystanie instrukcji wektorowych (AVX2).
4.  **Flagi kompilatora:**
    Użycie flagi `-O3` pozwala na najbardziej agresywne optymalizacje, w tym eliminację nieużywanego kodu.
