OUTPUT_FORMAT("binary")
ENTRY(kernel_main)

SECTIONS
{
    . = 0x10000;  /* 内核加载地址（和 boot.asm 里的跳转一致） */
    
    .text : {
        *(.text)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(.bss)
    }
    
    /* 如果有 .rodata 段 */
    .rodata : {
        *(.rodata)
    }
}