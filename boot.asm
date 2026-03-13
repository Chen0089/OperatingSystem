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

times 510 - ($ - $$) db 0
dw 0xAA55