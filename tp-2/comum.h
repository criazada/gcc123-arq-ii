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
// Este arquivo é para utilidades de escrita e leitura de vetores, para
// padronizar a interface de entrada e saída dos vários programas.
//
//
// DETALHES
//
// - A utilização de __attribute__((always_inline)) é explicada em "4.c"

#include <immintrin.h>
#include <stdio.h>
#include <stdarg.h>

static inline __attribute__((always_inline)) void printv256d(const char *nome, __m256d v) {
    printf("%-8s= [ ", nome);
    for (int i = 0; i < 4; i++) {
        printf("%-10.3lf", v[i]);
    }
    printf("]\n");
}

static inline __attribute__((always_inline)) void printv256(const char *nome, __m256 v) {
    printf("%-8s= [ ", nome);
    for (int i = 0; i < 8; i++) {
        printf("%-10.3f", v[i]);
    }
    printf("]\n");
}

static inline __attribute__((always_inline)) void printv256i(const char *nome, __m256i v) {
    printf("%-8s= [ ", nome);
    for (int i = 0; i < 4; i++) {
        long long e = v[i];
        printf("%-10d", e);
        printf("%-10d", e >> 32);
    }
    printf(" ]\n");
}

static inline __attribute__((always_inline)) void printv256i8(const char *nome, __m256i v) {
    printf("%-8s= [ ", nome);
    for (int i = 0; i < 4; i++) {
        long long e = v[i];
        for (int i = 0; i < 8; i++) {
            printf("%-5hhd", (char) (e >> (i*8)));
        }
    }
    printf(" ]\n");
}

static inline __attribute__((always_inline)) void printv256i16(const char *nome, __m256i v) {
    printf("%-8s= [ ", nome);
    for (int i = 0; i < 4; i++) {
        long long e = v[i];
        for (int i = 0; i < 4; i++) {
            printf("%-10hd", (short) (e >> (i*16)));
        }
    }
    printf(" ]\n");
}

static void scanv256d(const char *nome, double *v) {
    for (int i = 0; i < 4; i++) {
        printf("%s[%d] = ", nome, i);
        scanf("%lf", &v[i]);
    }
}

static void scanv256(const char *nome, float *v) {
    for (int i = 0; i < 8; i++) {
        printf("%s[%d] = ", nome, i);
        scanf("%f", &v[i]);
    }
}

static void scanv256i(const char *nome, int *v) {
    for (int i = 0; i < 8; i++) {
        printf("%s[%d] = ", nome, i);
        scanf("%d", &v[i]);
    }
}
