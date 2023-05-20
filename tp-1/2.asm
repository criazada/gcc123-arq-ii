li $v0, 5       # syscall 5 = read int
syscall         # lê inteiro em v0 (n)
move $t0, $v0   # salva v0 em t0 (t0 = n)

li $v0, 5       # syscall 5 = read int
syscall         # lê inteiro em v0 (i)
move $t1, $v0   # salva v0 em t1 (t1 = i)

li $v0, 5       # syscall 5 = read int
syscall         # lê inteiro em v0 (j)
move $t2, $v0   # lê inteiro em t2 (t2 = j)

ble $t1, $t2, i_menor   # se i > j, troca i por j
xor $t1, $t1, $t2       # t1 = t1 ^ t2
xor $t2, $t1, $t2       # t2 = t1 ^ t2 = t1
xor $t1, $t1, $t2       # t1 = t1 ^ t2 = t2
i_menor:

# t3 = múltiplos de i (a*i)
# t4 = múltiplos de j (b*j)

loop:
  blt $t3, $t4, ai_menor    # se a*i < b*j, a*i deve ser impresso
  bj_menor:
    move $a0, $t4       # salva b*j em a0 para ser impresso
    add $t4, $t4, $t2   # b*j += j
    bne $t3, $a0, print # se a*i == b*(j-1), ambos devem ser incrementados e a*i impresso
  ai_menor:
    move $a0, $t3       # salva a*i em a0 para ser impresso
    add $t3, $t3, $t1   # a*i += i

  print:
  li $v0, 1     # syscall 1 = print int
  syscall       # escreve na tela o menor múltiplo

  li $v0, 11    # syscall 11 = print char
  li $a0, ' '   # a0 = caractere a ser impresso
  syscall       # escreve um espaço na saída

  subi $t0, $t0, 1  # decrementa n
  bgtz $t0, loop    # itera enquanto n > 0
