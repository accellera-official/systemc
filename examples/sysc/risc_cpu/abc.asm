ldpid	0
movi	R6, 10
movi	R5, 8
mov	R5, R5
addi	R5, R5, 1
bne     R5, R6, -5 
add	R1, R2, R3
add	R2, R3, R4
ldpid	0
add 	R1, R1, R3
addi 	R1, R3, 14
fadd 	R3, R2, R2
fmul	R10, R11, R11
sub 	R4, R5, R6
movi	R5, 2
movi	R6, 0
subi 	R4, R5, 18
mul 	R1, R1, R4
addi 	R4, R6, 14
nand 	R10, R13, R15
addi	R6, R6, 1
movi	R11, 10
beq     R5, R6, 5 
lw  	R11, R11, 1
and 	R10, R13, R15
mmxadds R13, R11, R11
mmxsub  R9, R10, R3
mov 	R13, R13
sub 	R4, R5, R6
beq     R4, R4, 2 
not	R4, R6
nand 	R10, R13, R15
mmxadds R13, R11, R11
and 	R11, R14, R15
mmxsub  R9, R10, R3
mul	R1,  R10, R1
fadd 	R3, R2, R2
movi	R11, 10
fmul	R10, R11, R11
beq     R4, R4, 2 
xor 	R13, R14, R1
div	R1, R1, R3
pmadd	R3,  R11, R14
mmxck 	R1,  R1, R4
movi	R11, 8
movi	R12, 4
fdiv	R13, R11, R12
sw 	R13, R0, 10
sub 	R3,  R3, R11
mod	R4, R5, R5
pack 	R11, R13, R15
lw  	R11, R0, 5
and 	R10, R13, R15
mmxadds R13, R11, R11
mmxsub  R9, R10, R3
mov 	R13, R13
sub 	R4, R5, R6
nand 	R10, R13, R15
mmxadds R13, R11, R11
and 	R11, R14, R15
mov 	R1, R3
