SETUPLEN=2
SETUPSEG=0x07e0		! linux 0.11将setup放在0x90200，但我们放在0x07e00
entry _start
_start:
	mov ah,#0x03	! 读取光标位置
	xor bh,bh
 	int 0x10
	mov cx,#20		! cx为msg1的长度
	mov bx,#0x0007
	mov bp,#msg1
	mov ax,#0x07c0
	mov es,ax
	mov ax,#0x1301
	int 0x10		! 显示msg1
load_setup:			! 装载setup
	mov dx,#0x0000
	mov cx,#0x0002
	mov bx,#0x0200
	mov ax,#0x0200+SETUPLEN
	int 0x13
	jnc ok_load_setup
	mov dx,#0x0000
	mov ax,#0x0000
	int 0x13
	jmp load_setup
ok_load_setup:		! 跳到setup
	jmpi 0,SETUPSEG
msg1:
	.byte 13,10
	.ascii "I love *** ..."
	.byte 13,10,13,10
.org 510	! 用来保证bootsect一定是512字节
boot_flag:
	.word 0xAA55
