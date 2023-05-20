.data
snao: .ascii "nao e "
sperfeito: .asciiz "perfeito\n"

.text
li $v0, 5   # syscall 5 = read int
syscall     # lê inteiro em v0 (n)

la $a0, sperfeito   # salva o endereço da string de número perfeito em a0

beq $v0, 6, perfeito        # verifica se v0 é perfeito
beq $v0, 28, perfeito       #
beq $v0, 496, perfeito      #
beq $v0, 8128, perfeito     #
beq $v0, 33550336, perfeito #	

la $a0, snao    # $v0 não é perfeito, a0 então deve apontar para a string de não perfeito

perfeito:
li $v0, 4   # syscall 4 = print string
syscall     # escreve string apontada por a0 na saída
