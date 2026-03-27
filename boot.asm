[bits 16]
[org 0x7C00]

start:
    ; 初始化段寄存器
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; 重置磁盘系统
    mov ah, 0x00
    mov dl, 0x80          ; 第一块硬盘
    int 0x13

    ; 读取第二扇区到内存 0x1000:0x0000
    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000
    mov ah, 0x02
    mov al, 1             ; 读取1个扇区
    mov ch, 0
    mov cl, 2             ; 第二扇区
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc disk_error

    ; 跳转到第二扇区
    jmp 0x1000:0x0000

disk_error:
    mov si, error_msg
    call print
    cli
    hlt

print:
    mov ah, 0x0E
.loop:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

error_msg db 'Disk error!', 0

times 510 - ($ - $$) db 0
dw 0xAA55