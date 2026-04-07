#include "fat32.h"
#include "ata.h"      // 磁盘驱动
#include "print.h"

int fat32_mount(fat32_fs_t* fs, uint32_t start_lba) {
    uint8_t sector[512];
    
    // 读取引导扇区
    if (!ata_read_sector(start_lba, sector)) {
        print("Failed to read boot sector\n");
        return -1;
    }
    
    // 解析 BPB
    fat32_bpb_t* bpb = (fat32_bpb_t*)sector;
    
    // 验证 FAT32 签名
    if (bpb->boot_signature != 0x29) {
        print("Invalid boot signature\n");
        return -1;
    }
    
    // 保存 BPB
    fs->bpb = *bpb;
    
    // 验证是否为 FAT32（根目录入口应为 0）
    if (fs->bpb.root_entries != 0) {
        print("Not FAT32 (root_entries != 0)\n");
        return -1;
    }
    
    print("FAT32 mounted successfully\n");
    print("  Sectors per cluster: ");
    print_uint32(fs->bpb.sectors_per_cluster);
    print("\n  Root cluster: ");
    print_uint32(fs->bpb.root_cluster);
    print("\n");
    
    return 0;
}