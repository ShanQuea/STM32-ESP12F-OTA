//
// Created by 58257 on 2024/2/17.
//

#include "flash.h"
#include "stm32f1xx_hal.h"

void flash_erase_range(uint32_t start_address, uint32_t end_address) {
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error;
    // 设置页大小为1KB
    uint32_t page_size = 1024;

    // 计算从起始地址到结束地址之间的总页数
    uint32_t total_pages = (end_address - start_address) / page_size;

    // 配置擦除初始化结构体
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = start_address;
    erase_init.NbPages = total_pages;

    // 解锁 Flash 写入操作
    HAL_FLASH_Unlock();

    // 执行 Flash 擦除操作
    HAL_FLASHEx_Erase(&erase_init, &page_error);

    // 锁定 Flash 写入操作
    HAL_FLASH_Lock();
}