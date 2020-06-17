mov r1,#1
mov r3,#20
loop:
cmp r3,#0
beq end
add r2,r1,r0
mov r0,r1
mov r1,r2
sub r3,r3,#1
b loop
end:
ldr r0,=0x100
str r2,[r0]