mov r0,#163
mov r2,#34
loop:
sub r0,r0,r2
add r1,r1,#1
cmp r0,r2
bge loop
ldr r4,=0x100
str r1,[r4]
str r0,[r4,#4]