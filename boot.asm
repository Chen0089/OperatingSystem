BITS 16
ORG 0x7C00

start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg
    call print_string

    cli
    hlt

print_string:
    mov ah, 0x0E
.next:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .next
.done:
    ret

msg db 'Hello from MyOS v0.0.1-dev!', 0


; 加载内核（这里假设内核已经在 0x1000）
; 实际需要从磁盘读取
; 跳转到内核
jmp 0x1000:0x0000

times 510 - ($ - $$) db 0
dw 0xAA55