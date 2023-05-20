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
// Este programa realiza a operação de fmaddsub sobre diferentes tipos de dados,
// como double, float e inteiros. Para o último, a operação não é realizada com
// uma instrução só como com os outros.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -mfma -o 1 1.c`.
// - Execute o arquivo `1` e forneça os dados solicitados.
//
//
// DETALHES
//
// - As funções printv* e scanv* estão definidas em "comum.h".

#include "comum.h"

int main() {
    int tipo;
    printf("Selecione os tipos de dados\n");
    printf("1 - 4 doubles\n");
    printf("2 - 8 floats\n");
    printf("3 - 8 ints\n");
    printf("Escolha: ");
    scanf("%d", &tipo);

    printf("A operação a ser realizada será R = A x B +- C\n");

    if (tipo == 1) {
        // Os arrays devem estar alinhados em 32 bytes para que loads possam ser utilizados
        double __attribute__((aligned(32))) A[4], B[4], C[4];
        scanv256d("A", A);
        scanv256d("B", B);
        scanv256d("C", C);

        // Carrega os vetores e realiza o fma
        __m256d R = _mm256_fmaddsub_pd(_mm256_load_pd(A), _mm256_load_pd(B), _mm256_load_pd(C));
        printv256d("R", R);
    } else if (tipo == 2) {
        // O processo é o mesmo do que o para doubles, porém com 8 floats
        float __attribute__((aligned(32))) A[8], B[8], C[8];
        scanv256("A", A);
        scanv256("B", B);
        scanv256("C", C);

        __m256 R = _mm256_fmaddsub_ps(_mm256_load_ps(A), _mm256_load_ps(B), _mm256_load_ps(C));
        printv256("R", R);
    } else if (tipo == 3) {
        int __attribute__((aligned(32))) A[8], B[8], C[8];
        scanv256i("A", A);
        scanv256i("B", B);
        scanv256i("C", C);

        __m256i mascara = _mm256_setr_epi64x(-1, -1, -1, -1);
        __m256i a = _mm256_maskload_epi32(A, mascara);
        __m256i b = _mm256_maskload_epi32(B, mascara);
        // O processo diverge aqui, onde é necessário seletivamente negar alguns
        // números de C. Isso é feito com um vetor de multiplicadores
        __m256i multiplicador = _mm256_setr_epi32(-1, 1, -1, 1, -1, 1, -1, 1);
        __m256i c = _mm256_mullo_epi32(multiplicador, _mm256_maskload_epi32(C, mascara));

        // Como os elementos certos de C foram negados, basta somar à multiplicação entre A e B
        __m256i R = _mm256_add_epi32(_mm256_mullo_epi32(a, b), c);
        printv256i("R", R);
    }

    return 0;
}
