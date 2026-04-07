#ifndef ATA_H
#define ATA_H

#include <stdint.h>

// 主通道寄存器端口
#define ATA_PRIMARY_IO      0x1F0
#define ATA_PRIMARY_CONTROL 0x3F6

// 命令
#define ATA_CMD_READ        0x20
#define ATA_CMD_WRITE       0x30

// 读取一个扇区（512字节）
int ata_read_sector(uint32_t lba, uint8_t* buffer);

// 写入一个扇区（512字节）
int ata_write_sector(uint32_t lba, const uint8_t* buffer);

#endif