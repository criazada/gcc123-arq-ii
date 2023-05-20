.data
snao: .ascii "nao "
spertence: .asciiz "pertence a H\n"

.text
li $v0, 5       # syscall 5 = read int
syscall         # l� inteiro em v0 (n)
move $t0, $v0   # salva v0 em t0 (t0 = n)

li $t1, 1           # t1 = 1 
mtc1 $t1, $f10      # move t1 para f10, sem converter
cvt.d.w $f10, $f10  # converte f10 para double (f10 = 1)

loop:
  li $v0, 7         # syscall 7 = read double
  syscall           # l� inteiro em f0
  mov.d $f2, $f0    # salva em f2 o valor de f0 (f2 = x)
  syscall           # l� inteiro em f0 (f0 = y)

  la $a0, spertence # salva o endere�o da string de pertencente em a0

  mul.d $f2, $f2, $f2   # f2 = f2�
  mul.d $f0, $f0, $f0   # f0 = f0�
  add.d $f0, $f0, $f2   # f0 += f2
  c.le.d $f0, $f10      # verifica se f2� + f0� <= 1
  bc1t menor_que_um     # pula para menor_que_um se for
  maior_que_um:
    la $a0, snao        # sen�o, salva o endere�o da string de n�o pertencente em a0
  menor_que_um:
    li $v0, 4           # syscall 4 = print string
    syscall             # escreve string apontada por a0 na sa�da

  subi $t0, $t0, 1  # decrementa n
  bgtz $t0, loop    # itera enquanto n > 0
