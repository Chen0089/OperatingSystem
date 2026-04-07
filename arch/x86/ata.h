#ifndef ATA_H
#define ATA_H

#include <stdint.h>

// ATA 主通道寄存器端口（Primary Channel）
#define ATA_PRIMARY_IO      0x1F0   // 数据寄存器
#define ATA_PRIMARY_CONTROL 0x3F6   // 控制寄存器

// ATA 命令
#define ATA_CMD_READ        0x20    // 读取扇区
#define ATA_CMD_WRITE       0x30    // 写入扇区

/* 读取一个扇区（512字节）
 * 参数: lba - 逻辑块地址（0-based）
 *       buffer - 存放数据的缓冲区（至少512字节）
 * 返回: 0=成功，-1=失败 */
int ata_read_sector(uint32_t lba, uint8_t* buffer);

/* 写入一个扇区（512字节）
 * 参数: lba - 逻辑块地址（0-based）
 *       buffer - 要写入的数据（至少512字节）
 * 返回: 0=成功，-1=失败 */
int ata_write_sector(uint32_t lba, const uint8_t* buffer);

#endif