ldr r0,=0xD73
mov r2,#11
loop:
sub r0,r0,r2
add r1,r1,#1
cmp r0,r2
bge loop