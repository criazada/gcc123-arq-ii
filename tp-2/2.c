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
// Este programa realiza a operação DAXPY, ou a*X + Y, onde a é um escalar
// fornecido pelo usuário.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -mfma -o 2 2.c`.
// - Execute o arquivo `2` e forneça os dados solicitados.
//
//
// DETALHES
//
// - As funções printv* e scanv* estão definidas em "comum.h".

#include "comum.h"

int main() {
    float __attribute__((aligned(32))) X[8] = {  1.0, 2.0,  3.0, 4.0,  6.0, 8.0, 10.0, 12.0 };
    float __attribute__((aligned(32))) Y[8] = { -1.0, 3.0, -5.0, 7.0, 12.0, 1.5, 3.33,  2.0 };

    float A;
    printf("a = ");
    scanf("%f", &A);

    // Preenche o vetor a deixando todas as posições com o valor do escalar A
    __m256 a = _mm256_broadcast_ss(&A);
    __m256 x = _mm256_load_ps(X);
    __m256 y = _mm256_load_ps(Y);

    // Realiza a*X + Y utilizando FMA
    __m256 r = _mm256_fmadd_ps(a, x, y);
    printv256("R", r);

    return 0;
}
