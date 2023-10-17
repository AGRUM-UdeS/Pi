#include "wrap_FLASH.h"

bool flash_write(unsigned int addr, const uint8_t *src) {
    if (addr % FLASH_SECTOR_SIZE) {
        return false;
    }

    taskENTER_CRITICAL();
    uint32_t ints = save_and_disable_interrupts();
    
    flash_range_erase(addr, FLASH_SECTOR_SIZE);
    flash_range_program(addr, src, FLASH_PAGE_SIZE);
    
    restore_interrupts (ints);
    taskEXIT_CRITICAL();
    return true;
}

bool flash_read(unsigned int addr, uint8_t* dst) {
    if (addr % FLASH_PAGE_SIZE) {
        return false;
    }

    taskENTER_CRITICAL();
    uint32_t ints = save_and_disable_interrupts();

    // Compute the memory-mapped address, remembering to include the offset for RAM
    unsigned int addr_offset = XIP_BASE +  addr;
    uint8_t* p = (uint8_t *)addr_offset;  // Place an int pointer at our memory-mapped address

    restore_interrupts (ints);
    taskEXIT_CRITICAL();
    *dst = *p;
    return true;
}
