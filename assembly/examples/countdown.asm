.text

li $t0, 10
loop:
  li $v0, 1 # print int
  move $a0, $t0
  syscall
  subi $t0, $t0, 1
  bgt $t0, $zero, loop
  
li $v0, 10 # exit
syscall