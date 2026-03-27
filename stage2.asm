[bits 16]
[org 0x1000]

start_stage2:
    ; 获取 VBE 模式信息
    mov ax, 0x4F00
    mov di, vbe_info
    int 0x10
    cmp ax, 0x004F
    jne no_vbe

    ; 设置 1024x768x32 模式
    mov ax, 0x4F02
    mov bx, 0x4118       ; 模式号
    or bx, 0x4000        ; 线性帧缓冲标志
    int 0x10
    cmp ax, 0x004F
    jne no_vbe

    ; 保存显存地址
    mov eax, [vbe_info + 0x28]
    mov [framebuffer_addr], eax

    ; 进入保护模式
    cli
    lgdt [gdtr]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

[bits 32]
protected_mode:
    ; 初始化段寄存器
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; 把显存地址传给内核
    push dword [framebuffer_addr]

    ; 加载内核（地址为0x1000）
    jmp 0x10000

no_vbe:
    ; 降级到文本模式
    mov ax, 0x0003
    int 0x10
    mov si, vbe_error
    call print32
    cli
    hlt

; 全局描述符表
gdtr:
    dw gdt_end - gdt - 1
    dd gdt

gdt:
    ; 空描述符
    dd 0, 0
    ; 代码段
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF
    ; 数据段
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF
gdt_end:

print32:
    pusha
    mov ebx, 0xB8000
.loop:
    mov al, [si]
    cmp al, 0
    je .done
    mov ah, 0x0F
    mov [ebx], ax
    add ebx, 2
    inc si
    jmp .loop
.done:
    popa
    ret

vbe_error db 'VBE failed!', 0

vbe_info:
    times 256 db 0
framebuffer_addr:
    dd 0

times 512 - ($ - $$) db 0   ; 确保第二扇区也是512字节，方便读取