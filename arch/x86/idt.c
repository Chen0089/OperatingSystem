#include "idt.h"
#include "io.h"
#include "keyboard.h"

extern void isr_stub(void);  // 告诉编译器这是个外部函数

// IDT表（256个中断门）
idt_entry_t idt_entries[256];
idtr_t idtr;

// 默认中断处理函数
void isr_handler(uint32_t int_no) {
    // 这里先简单打印错误号，以后可以改成更完整的处理
    const char* error_msg = "Interrupt: ";
    
    // 如果是除0错误，可以特殊处理
    if (int_no == 0) {
        // print("Divide by zero!\n");
    }
    
    // 死循环（或者返回，但一般中断处理完后会返回）
    while(1);  // 调试用，正式代码应该iretd
}

// IRQ处理函数（硬件中断）
void irq_handler(uint32_t int_no) {
    // 发EOI（End Of Interrupt）给PIC
    if (int_no >= 40) {
        // 从片
        outb(0xA0, 0x20);
    }
    // 主片
    outb(0x20, 0x20);
    
    // print("IRQ: ");
    // print_hex(int_no);
    // print("\n");
}

// 初始化IDT
void idt_init(void) {
    // 设置IDTR
    idtr.limit = sizeof(idt_entry_t) * 256 - 1;
    idtr.base = (uint32_t)&idt_entries;
    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);  // IRQ1 = 33
    // 清空IDT（所有门都指向默认处理）
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, (uint32_t)isr_stub, 0x08, 0x8E);
        // 0x08: 内核代码段选择子
        // 0x8E: 中断门属性（存在、ring0、中断门）
    }
    
    // 加载IDT
    idt_load(&idtr);
}

// 设置中断门
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}