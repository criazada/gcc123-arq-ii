li $v0, 5       # syscall 5 = read int
syscall         # l� inteiro em v0 (n)
sub $v0, $v0, 1 # para F_n, s�o necess�rias n-1 itera��es

li $a0, 1   # F_1 = 1
li $t0, 1   # F_2 = 1

loop:
  add $t1, $t0, $a0 # F_n = F_{n-1} + F_{n-2}
  move $a0, $t0     # para a pr�xima itera��o, F_{n-2} -> F_{n-1}
  move $t0, $t1     #                        , F_{n-1} -> F_n

  subi $v0, $v0, 1  # decrementa n
  bgtz $v0, loop    # itera enquanto n > 0

li $v0, 1   # syscall 1 = print int
syscall     # escreve o n�mero esperado na tela
