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
// Este programa exemplifica o funcionamento da família de intrínsecos _add(s)_
// e _sub(s)_ do AVX.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -o 3 3.c`.
// - Execute o arquivo `3`.
//
//
// DETALHES
//
// - As funções printv* e scanv* estão definidas em "comum.h".

#include "comum.h"

int main() {
    // Definição dos vetores iniciais
    __m256i veca = _mm256_setr_epi32(2000000000, -2000000000, 2000000000, -2000000000, 10, 12, 14, 16);
    __m256i vecb = _mm256_setr_epi32(2000000000, -2000000000, -2000000000, 2000000000, 2, 2, 2, 2);

    __m256i add = _mm256_add_epi8(veca, vecb);
    __m256i subs = _mm256_subs_epi16(veca, vecb);

    printv256i8("add", add);
    printv256i16("subs", subs);
    return 0;
}
