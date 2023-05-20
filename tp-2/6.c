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
// Este programa realiza a multiplicação de 2 pares de números complexos com
// outros 2 pares de números complexos.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -mfma -o 6 6.c`.
// - Execute o arquivo `6`.
//
//
// DETALHES
//
// - As funções printv* e scanv* estão definidas em "comum.h".

#include "comum.h"

int main() {
  double __attribute__((aligned(32))) numeros[8];
  scanv256d("abxy", numeros);
  scanv256d("cdzw", &numeros[4]);

  __m256d vec1 = _mm256_load_pd(numeros);
  __m256d vec2 = _mm256_load_pd(&numeros[4]);
  __m256d neg = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

  // Multiplica os números reais com os reais e os imaginários com os imaginários
  // [ a b x y ] * [ c d z w ] = [ ac bd xz yw ]
  __m256d vec3 = _mm256_mul_pd(vec1, vec2);

  // Troca a posição dos números imaginários com os reais em vec2
  // [ c d z w ] -> [ d c w z ]
  vec2 = _mm256_permute_pd(vec2, 0x5);

  // Nega os atuais números reais em vec2
  // [ d c w z ] -> [ d -c w -z ]
  vec2 = _mm256_mul_pd(vec2, neg);  

  // Multiplica números reais com imaginários
  // [ a b x y ] * [ d -c w -z ] = [ ad -bc xw -yz ]
  __m256d vec4 = _mm256_mul_pd(vec1, vec2);

  // Subtrai horizontalmente os elementos de vec3 e vec4
  // hsub([ ac bd xz yw ], [ ad -bc xw -yz ]) = [ (ac-bd) (ad+bc) (xz-yw) (xw+yz) ]
  vec1 = _mm256_hsub_pd(vec3, vec4);

  // Mostra o resultado
  printv256d("v1 * v2", vec1);

  return 0;
}
