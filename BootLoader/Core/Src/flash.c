//
// Created by 58257 on 2024/2/17.
//

#include "flash.h"
#include "stm32f1xx_hal.h"

void flash_erase_range(uint32_t start_address, uint32_t end_address) {
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error;
    // ����ҳ��СΪ1KB
    uint32_t page_size = 1024;

    // �������ʼ��ַ��������ַ֮�����ҳ��
    uint32_t total_pages = (end_address - start_address) / page_size;

    // ���ò�����ʼ���ṹ��
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = start_address;
    erase_init.NbPages = total_pages;

    // ���� Flash д�����
    HAL_FLASH_Unlock();

    // ִ�� Flash ��������
    HAL_FLASHEx_Erase(&erase_init, &page_error);

    // ���� Flash д�����
    HAL_FLASH_Lock();
}