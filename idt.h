#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// 中断门描述符结构（8字节）
typedef struct {
        uint16_t base_low;      // 中断处理函数地址低16位
            uint16_t sel;           // 段选择子（内核代码段）
                uint8_t  always0;       // 必须为0
                    uint8_t  flags;         // 属性标志
                        uint16_t base_high;     // 中断处理函数地址高16位
} __attribute__((packed)) idt_entry_t;

// IDTR寄存器结构（6字节）
typedef struct {
        uint16_t limit;         // IDT表大小 -1
            uint32_t base;          // IDT表基地址
} __attribute__((packed)) idtr_t;

// 初始化IDT
void idt_init(void);

// 设置中断门
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// 加载IDTR（汇编实现）
void idt_load(idtr_t *idtr);

// 中断处理函数（C部分）
void isr_handler(uint32_t int_no);
void irq_handler(uint32_t int_no);

#endif
