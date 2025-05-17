// 内存管理
#define PAGE_SIZE 4096
#define PHYS_MEM_SIZE 0x1000000 // 16MB

class PhysicalMemoryManager {
private:
    uint32_t* bitmap;
    uint32_t max_frames;
public:
    void init() {
        bitmap = (uint32_t*)0x5000; // 内存位图地址
        max_frames = PHYS_MEM_SIZE / PAGE_SIZE;
        memset(bitmap, 0, max_frames/8);
    }
    
    void* alloc_page() {
        for (uint32_t i=0; i<max_frames/32; i++) {
            if (bitmap[i] != 0xFFFFFFFF) {
                for (int j=0; j<32; j++) {
                    if (!(bitmap[i] & (1 << j))) {
                        bitmap[i] |= (1 << j);
                        return (void*)(i*32*PAGE_SIZE + j*PAGE_SIZE);
                    }
                }
            }
        }
        return nullptr; // 内存耗尽
    }
};

// 虚拟内存管理（分页初始化）
void init_paging() {
    asm volatile("mov %%cr3, %0" : : "r"(page_dir));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // 启用分页
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
