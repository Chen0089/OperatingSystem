#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

// BPB (BIOS Parameter Block)
typedef struct {
    uint8_t  jmp[3];
    char     oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t root_entries;      // FAT32 中为 0
    uint16_t total_sectors_16;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat_16; // FAT32 中为 0
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t sectors_per_fat_32;     // FAT32 专用
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;           // 根目录起始簇号
    uint16_t fsinfo_sector;
    uint16_t backup_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved1;
    uint8_t  boot_signature;
    uint32_t volume_id;
    char     volume_label[11];
    char     fs_type[8];
} __attribute__((packed)) fat32_bpb_t;

// 文件系统主结构
typedef struct {
    fat32_bpb_t bpb;
    uint32_t* fat;              // FAT 表缓存（按簇号索引）
    uint32_t fat_size;          // FAT 表项数
    uint8_t* data_cache;        // 可选：数据缓存
} fat32_fs_t;

#endif