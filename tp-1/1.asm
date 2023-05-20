.data
snao: .ascii "nao e "
sperfeito: .asciiz "perfeito\n"

.text
li $v0, 5   # syscall 5 = read int
syscall     # l� inteiro em v0 (n)

la $a0, sperfeito   # salva o endere�o da string de n�mero perfeito em a0

beq $v0, 6, perfeito        # verifica se v0 � perfeito
beq $v0, 28, perfeito       #
beq $v0, 496, perfeito      #
beq $v0, 8128, perfeito     #
beq $v0, 33550336, perfeito #	

la $a0, snao    # $v0 n�o � perfeito, a0 ent�o deve apontar para a string de n�o perfeito

perfeito:
li $v0, 4   # syscall 4 = print string
syscall     # escreve string apontada por a0 na sa�da
