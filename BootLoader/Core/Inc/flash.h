//
// Created by 58257 on 2024/2/17.
//

#ifndef BOOTLOADER_FLASH_H
#define BOOTLOADER_FLASH_H

#include <stdint.h>

void flash_erase_range(uint32_t start_address, uint32_t end_address);

#endif //BOOTLOADER_FLASH_H
