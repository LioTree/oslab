INITSEG = 0x9000		! 将硬件参数放在0x90200处

entry _start
_start:
	mov	ah,#0x03		! 还是获取光标位置
	xor	bh,bh
	int	0x10
	
	mov	cx,#25
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#msg2
    mov ax,cs
    mov es,ax
	mov	ax,#0x1301		! 显示msg2
	int	0x10
    call read_param		! 读取硬件参数
	call output_param	! 将硬件参数输出
inf_loop:				! 没有system所以弄个死循环
        jmp inf_loop

! 读取硬件的参数，抄抄抄...
read_param:
	mov ax,#INITSEG		! 获取光标位置
	mov ds,ax
	mov ah,#0x03
	xor bh,bh
	int 0x10
	mov [0],dx

	mov ah,#0x88		! 获取内存大小
	int 0x15
	mov [2],ax

	mov ax,#0x0000		! 获取0号磁盘的数据
	mov ds,ax
	lds si,[4*0x41]
	mov ax,#INITSEG
	mov es,ax
	mov di,#0x0004
	mov cx,#0x10
	rep
	movsb
	ret

! 在屏幕上输出硬件的参数
output_param:
	mov ax,cs		! 设置好段寄存器
	mov es,ax
	mov ax,#INITSEG
	mov ds,ax

	mov ah,#0x03	! 光标位置
	xor bh,bh
	int 0x10
	mov cx,#18
	mov bx,#0x0007
	mov bp,#msg_cursor
	mov ax,#0x1301
	int 0x10
	mov dx,[0]
	call print_hex	

	mov ah,#0x03	! 内存大小
	xor bh,bh
	int 0x10
	mov cx,#14
	mov bx,#0x0007
	mov bp,#msg_memory
	mov ax,#0x1301
	int 0x10
	mov dx,[2]
	call print_hex

	mov ah,#0x03	! 显示KB
	xor bh,bh
	int 0x10
	mov cx,#2
	mov bx,#0x0007
	mov bp,#msg_kb
	mov ax,#0x1301
	int 0x10

	mov ah,#0x03	! 柱面数
	xor bh,bh
	int 0x10
	mov cx,#7
	mov bx,#0x0007
	mov bp,#msg_cyles
	mov ax,#0x1301
	int 0x10
	mov dx,[4]
	call print_hex
	
	mov ah,#0x03	! 磁头数
	xor bh,bh
	int 0x10
	mov cx,#8
	mov bx,#0x0007
	mov bp,#msg_heads
	mov ax,#0x1301
	int 0x10
	mov dx,[6]
	call print_hex
	
	mov ah,#0x03	! 扇区数
	xor bh,bh
	int 0x10
	mov cx,#10
	mov bx,#0x0007
	mov bp,#msg_sectors
	mov ax,#0x1301
	int 0x10
	mov dx,[12]
	call print_hex
	ret

print_hex:		! 以 16 进制方式打印栈顶的16位数，懒得看了直接抄的...具体的注释可以看实验楼上的代码
	mov cx,#4
print_digit:
	rol dx,#4
	mov ax,#0xe0f
	and al,dl
	add al,#0x30
	cmp al,#0x3a
	jl outp
	add al,#0x07
outp:
	int 0x10
	loop print_digit
	ret

msg2:
	.byte 13,10
        .ascii "Now we are in SETUP"
        .byte 13,10,13,10
! 硬件参数的提示信息
msg_cursor:
	.byte 13,10
	.ascii "Cursor position:"
msg_memory:
	.byte 13,10
	.ascii "Memory Size:"
msg_cyles:
	.byte 13,10
	.ascii "Cyls:"
msg_heads:
	.byte 13,10
	.ascii "Heads:"
msg_sectors:
	.byte 13,10
	.ascii "Sectors:"
msg_kb:
	.ascii "KB"

.org 510
boot_flag:
        .word 0xAA55
