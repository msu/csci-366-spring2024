.text

main:
  li $v0, 4 # print string
  la $a0, enter
  syscall

  li $v0, 5 # read int
  syscall

  move $t0, $v0 # save to temporary
  
  addi $t0, $t0, 1 # add one

  li $v0, 4 # print string
  la $a0, add
  syscall

  li $v0, 1 # print int
  move $a0, $t0
  syscall

  li $v0, 10  # exit
  syscall

.data
enter:	.asciiz	"Enter a number: "
add:	.asciiz	"Added 1: "

