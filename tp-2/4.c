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
// Este programa encontra o determinante de uma matriz 4x4 qualquer, utilizando
// um algoritmo que computa a fórmula descrita por Eric Zhang em [0], para uma
// matriz M, que é |M| = |A||D| + |B||C| - tr((adj(A)B)(adj(D)C)), sendo:
// - M a matriz 4x4, particionada em 4 blocos [1];
// - |X| o determinante da matriz X [2];
// - A, B, C e D os blocos 2x2 da matriz M [1];
// - tr(X) a função traço aplicada a uma matriz X [3];
// - adj(X) a matriz adjunta de X [4].
//
// A matriz é lida do terminal e armazenada numa matriz de doubles em ordem
// principal de linha.
//
// Cada linha da matriz é carregada para um vetor de double de 256 bits do AVX2,
// e são reorganizadas para facilitar multiplicação de elementos relevantes para
// encontrar os determinantes dos blocos.
//
// Os valores |A||D| e |B||C| são computados e armazenados em um vetor, e então
// somados para obter a primeira parte de |M|.
//
// As operações adj(X)Y são realizadas em um passo pela função adjXxY, que
// recebe as linhas da matriz que contêm os blocos X e Y e faz a multiplicação
// de matrizes já considerando que deve utilizar a matriz adjunta de X na
// operação.
//
// A função traço nada mais é que a soma da diagonal principal de uma matriz, e
// como estamos trabalhando com o traço de uma multiplicação de matrizes, seu
// resultado pode ser obtido rapidamente com uma multiplicação de vetores (ao
// invés de duas, que seria necessário caso fosse utilizar a outra diagonal) e
// algumas somas.
//
//
// UTILIZANDO O PROGRAMA
//
// - Compile o programa com `gcc -mavx2 -o 4 4.c`.
// - Execute o arquivo `4` e forneça os dados da matriz.
//
//
// DETALHES
//
// - Há um bug [5] no gcc do MinGW (Windows) no qual a stack não é alinhada
//   corretamente para suportar passagem de parâmetros de tipos de vetores do
//   AVX e AVX2 (é alinhada para 16 bytes, não 32). Evitamos esse bug marcando
//   as funções que recebem vetores como "inline". Somente marcar como inline
//   não resolve completamente o problema, já que -O0 (o modo padrão de
//   compilação do gcc) não faz inlining nenhum de funções. Isso é remediado
//   utilizando __attribute__((always_inline)) para sempre realizar inlining
//   independente do modo de compilação.
// - As funções printv* e scanv* estão definidas em "comum.h".
//
//
// REFERÊNCIAS
//
// - [0] https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
// - [1] https://pt.wikipedia.org/wiki/Matriz_em_bloco
// - [2] https://pt.wikipedia.org/wiki/Determinante
// - [3] https://pt.wikipedia.org/wiki/Traço_(álgebra_linear)
// - [4] https://pt.wikipedia.org/wiki/Matriz_adjunta
// - [5] https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54412


#include "comum.h"

// Realiza a multiplicação adj(X)Y, onde X e Y estão em L0 e L1 da seguinte forma:
// - L0 = [ Xa Ya Xb Yb ]
// - L1 = [ Xd Yd Xc Yc ]
//
// Retorna a matriz R = |  Ra  Rb  |
//                      |  Rc  Rd  |
// em um vetor de forma R = [ Ra Rc Rb Rd ]
static inline __attribute__((always_inline)) __m256d adjXxY(__m256d L0, __m256d L1) {
    // adj(X) x Y = |  Xd -Xb  | |  Ya  Yb  |
    //              | -Xc  Xa  | |  Yc  Yd  |
    // adj(X) x Y = | (Xd*Ya - Xb*Yc) (Xd*Yb - Xb*Yd) |
    //              | (Xa*Yc - Xc*Ya) (Xa*Yd - Xc*Yb) |
    // v1 = [ Xd Xa Xd Xa ] = [ L1_0 L0_0 L1_0 L0_0 ]
    // v2 = [ Ya Yc Yb Yd ] = [ L0_1 L1_3 L0_3 L1_1 ]
    //
    // v3 = [ Xb Xc Xb Xc ] = [ L0_2 L1_2 L0_2 L1_2 ]
    // v4 = [ Yc Ya Yd Yb ] = [ L1_3 L0_1 L1_1 L0_3 ]
    //
    // R = v1*v2-v3*v4 = [ Ra Rc Rb Rd ]

    __m256d v1 = _mm256_permute2f128_pd(L0, L1, 0b00100000); // v1 = [ L0_0 L0_1 L1_0 L1_1 ]
    __m256d v3 = _mm256_permute2f128_pd(L0, L1, 0b00110001); // v3 = [ L0_2 L0_3 L1_2 L1_3 ]

    v1 = _mm256_permute4x64_pd(v1, _MM_SHUFFLE(0, 2, 0, 2)); // v1 = [ L1_0 L0_0 L1_0 L1_0 ]
    v3 = _mm256_permute4x64_pd(v3, _MM_SHUFFLE(2, 0, 2, 0)); // v3 = [ L0_2 L1_2 L0_2 L1_2 ]

    __m256d v2 = _mm256_setr_pd(L0[1], L1[3], L0[3], L1[1]); // v2 = [ L0_1 L1_3 L0_3 L1_1 ]
    __m256d v4 = _mm256_permute4x64_pd(v2, _MM_SHUFFLE(2, 3, 0, 1));     // v4 = [ L1_3 L0_1 L1_1 L0_3 ]

    // v1*v2-v3*v4
    return _mm256_sub_pd(_mm256_mul_pd(v1, v2), _mm256_mul_pd(v3, v4));
}

int main() {
    double __attribute__((aligned(32))) a[4][4] = {
        { 0, 0, 0, 0 }, // Aa Ab Ba Bb
        { 0, 0, 0, 0 }, // Ac Ad Bc Bd
        { 0, 0, 0, 0 }, // Ca Cb Da Db
        { 0, 0, 0, 0 }  // Cc Cd Dc Dd
    };

    printf("Insira os elementos da matriz M\n");
    scanv256d("M[0]", a[0]);
    scanv256d("M[1]", a[1]);
    scanv256d("M[2]", a[2]);
    scanv256d("M[3]", a[3]);

    __m256d L0 = _mm256_load_pd(a[0]); // [ Aa Ab Ba Bb ]
    __m256d L1 = _mm256_load_pd(a[1]); // [ Ac Ad Bc Bd ]
    __m256d L2 = _mm256_load_pd(a[2]); // [ Ca Cb Da Db ]
    __m256d L3 = _mm256_load_pd(a[3]); // [ Cc Cd Dc Dd ]

    L0 = _mm256_permute4x64_pd(L0, _MM_SHUFFLE(3, 1, 2, 0)); // [ Aa Ba Ab Bb ]
    L1 = _mm256_permute4x64_pd(L1, _MM_SHUFFLE(2, 0, 3, 1)); // [ Ad Bd Ac Bc ]
    L2 = _mm256_permute4x64_pd(L2, _MM_SHUFFLE(1, 3, 0, 2)); // [ Da Ca Db Cb ]
    L3 = _mm256_permute4x64_pd(L3, _MM_SHUFFLE(0, 2, 1, 3)); // [ Dd Cd Dc Cc ]

    __m256d AB_axd_bxc = _mm256_mul_pd(L0, L1); // [ Aa*Ad Ba*Bd Ab*Ac Bb*Bc ]
    __m256d DC_axd_bxc = _mm256_mul_pd(L2, L3); // [ Da*Dd Ca*Cd Db*Dc Cb*Cc ] 
    __m256d ABDC_ad = _mm256_permute2f128_pd(AB_axd_bxc, DC_axd_bxc, 0b00100000); // [ Aa*Ad Ba*Bd Ca*Cd Da*Dd ]
    __m256d ABDC_bc = _mm256_permute2f128_pd(AB_axd_bxc, DC_axd_bxc, 0b00110001); // [ Ab*Ac Bb*Bc Cb*Cc Db*Dc ]
    // |X| = Xa*Xd - Xb*Xc
    __m256d detABDC = _mm256_sub_pd(ABDC_ad, ABDC_bc); // [ detA detB detD detC ]
    __m256d detDCAB = _mm256_permute4x64_pd(detABDC, _MM_SHUFFLE(1, 0, 3, 2)); // [ detD detC detA detB ]
    __m256d det_AxD_BxC = _mm256_mul_pd(detABDC, detDCAB);  // [ detAxdetD detBxdetC detDxdetA detCxdetB ]
    // primeira parte de |M|
    __m256d detM = _mm256_hadd_pd(det_AxD_BxC, det_AxD_BxC); // [ (detAxdetD + detBxdetC) (detAxdetD + detBxdetC) (detDxdetA + detCxdetB) (detDxdetA + detCxdetB) ]

    __m256d adjAxB = adjXxY(L0, L1);
    __m256d adjDxC = adjXxY(L2, L3);

    // adjAxB x adjDxC = | (Ea*Fa + Eb*Fc)       ---       |
    //   E        F      |       ---       (Ec*Fb + Ed*Fd) |
    // no nosso caso, a diagonal secundária não é relevante
    //
    //                            adjAxB = [ Ea Ec Eb Ed ]
    // permute4x64(adjDxC, (3, 1, 2, 0)) = [ Fa Fb Fc Fd ]

    __m256d tr = _mm256_mul_pd(adjAxB, _mm256_permute4x64_pd(adjDxC, _MM_SHUFFLE(3, 1, 2, 0)));
    __m256d tr2 = _mm256_permute2f128_pd(tr, tr, 0b00000001);
    // tr  = [ Aa*Ba Ac*Bb Ab*Bc Ad*Bd ]
    // tr2 = [ Ab*Bc Ad*Bd Aa*Ba Ac*Bb ]
    tr = _mm256_add_pd(tr, tr2); // [ (Aa*Ba+Ab*Bc) (Ac*Bb+Ad*Bd) (Ab*Bc+Aa*Ba) (Ad*Bd+Ac*Bb) ]
    tr = _mm256_hadd_pd(tr, tr); // [ (Aa*Ba+Ab*Bc + Ac*Bb+Ad*Bd) ... ]
    detM = _mm256_sub_pd(detM, tr); // subtração final
    // detM = [ (|A||D| + |B||C| - tr((adj(A)B)(adj(D)C))) ... ]
    
    printf("|M| = %lf\n", detM[0]);

    return 0;
}
