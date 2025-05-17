; ===== 中断处理模块 =====
[bits 32]
global load_idt

load_idt:
    mov eax, [esp+4]
    lidt [eax]
    ret

%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common
%endmacro

section .text
irq_common:
    pusha
    call irq_handler
    popa
    add esp, 8
    iret
