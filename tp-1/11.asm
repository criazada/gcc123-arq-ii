li $v0, 5   # syscall 5 = read int
syscall     # l� inteiro em v0 (n)

li $t0, 1           # t0 = 1
mtc1 $t0, $f10      # move t0 para f10, sem converter
cvt.d.w $f10, $f10  # converte f10 para double (f10 = 1)

loop:
  add.d $f0, $f0, $f10  # f0 += 1
  div.d $f2, $f10, $f0  # f2 = 1/f0
  add.d $f12, $f12, $f2 # f12 += 1/f0

  subi $v0, $v0, 1  # decrementa n
  bgtz $v0, loop    # itera enquanto n > 0

li $v0, 3   # syscall 3 = print double
syscall     # escreve f12 na sa�da
