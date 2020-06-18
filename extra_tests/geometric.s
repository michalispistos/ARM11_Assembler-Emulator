mov r1,#1
mov r2,#2
mov r3,#5
mov r4,#1
loop:
cmp r3,#0
beq end
mul r4,r4,r2
add r1,r1,r4
sub r3,r3,#1
b loop
end:
ldr r0,=0x100
str r1,[r0]