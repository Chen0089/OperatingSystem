[bits 32]
[GLOBAL idt_load]
[GLOBAL isr_stub]
[GLOBAL irq_stub]
[EXTERN isr_handler]
[EXTERN irq_handler]

; 加载IDT
idt_load:
    mov eax, [esp+4]      ; 参数：idtr指针
    lidt [eax]            ; 加载IDTR
    ret

; 定义ISR宏（软件中断/异常）
%macro ISR_NOERR 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte 0           ; 错误码占位（没有错误码）
    push byte %1          ; 中断号
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte %1          ; 错误码已在栈中
    jmp isr_common_stub
%endmacro

; 定义IRQ宏（硬件中断）
%macro IRQ 2
[GLOBAL irq%1]
irq%1:
    cli
    push byte 0           ; 错误码占位
    push byte %2          ; 中断号（32+IRQ号）
    jmp irq_common_stub
%endmacro

; 常见的ISR（0-31是CPU异常）
ISR_NOERR 0   ; 除0错误
ISR_NOERR 1   ; 调试异常
ISR_NOERR 2   ; 非屏蔽中断
ISR_NOERR 3   ; 断点
ISR_NOERR 4   ; 溢出
ISR_NOERR 5   ; 边界检查
ISR_NOERR 6   ; 非法指令
ISR_NOERR 7   ; 设备不可用
ISR_ERR   8   ; 双重错误（有错误码）
ISR_NOERR 9   ; 协处理器段越界
ISR_ERR   10  ; 无效TSS
ISR_ERR   11  ; 段不存在
ISR_ERR   12  ; 堆栈段错误
ISR_ERR   13  ; 通用保护错误
ISR_ERR   14  ; 页错误
ISR_NOERR 15  ; 保留
ISR_NOERR 16  ; FPU错误
ISR_ERR   17  ; 对齐检查
ISR_NOERR 18  ; 机器检查
ISR_NOERR 19  ; SIMD浮点异常

; IRQ（0-15对应IRQ0-IRQ15，映射到32-47）
IRQ 0, 32   ; 系统时钟
IRQ 1, 33   ; 键盘
IRQ 2, 34   ; 级联
IRQ 3, 35   ; 串口2
IRQ 4, 36   ; 串口1
IRQ 5, 37   ; 声卡
IRQ 6, 38   ; 软盘
IRQ 7, 39   ; 并口
IRQ 8, 40   ; RTC
IRQ 9, 41   ; 通用
IRQ 10, 42  ; 通用
IRQ 11, 43  ; 通用
IRQ 12, 44  ; 鼠标
IRQ 13, 45  ; FPU
IRQ 14, 46  ; 主IDE
IRQ 15, 47  ; 从IDE

; ISR公共处理
isr_common_stub:
    pusha                ; 保存所有通用寄存器
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10         ; 加载内核数据段
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp             ; 传递寄存器指针
    call isr_handler
    pop esp
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8           ; 清理错误码和中断号
    iret

; IRQ公共处理
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    call irq_handler
    pop esp
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret