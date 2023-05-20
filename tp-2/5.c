// GCC123 - Arquitetura de Computadores II - 2021/2 - 10A
//
//
// COMPONENTES DO GRUPO
//
// - Érick de Castro Silva
// - Iago Manoel Brito de Sá Vaz da Silva
// - Isaque Pereira Alves
// - Katarina Lydia Friedrich
// - Luis Fernando Santos Ferreira
// - Vinícius Caputo de Castro
//
//
// SOBRE
//
// Este programa exemplifica o funcionamento da família de intrínsecos _permute_
// do AVX.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -o 5 5.c`.
// - Execute o arquivo `5`.
//
//
// DETALHES
//
// - As funções printv* e scanv* estão definidas em "comum.h".

#include "comum.h"

int main() {
    float __attribute__((aligned(32))) A[8] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    __m256 a = _mm256_load_ps(A);
    __m256 b = _mm256_permute_ps(a, 0b11000110);

    printv256("a", a);
    printv256("b", b);
    printf("%-8s= 0b11000110\n", "c");
    return 0;
}
