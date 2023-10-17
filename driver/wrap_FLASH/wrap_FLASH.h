#ifndef FLASH_WRAPPER_h
#define FLASH_WRAPPER_h

#include <stdio.h>
#include "pico/stdlib.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <hardware/sync.h>
#include <hardware/flash.h>

/*! \brief Write to flash
 *
 * \param addr Address in flash memory to start write. Must be a multiple of FLASH_SECTOR_SIZE
 * \param src Pointer to the buffer that contains bytes to write
 * \param len Number of bytes to write. Should be less than FLASH_SECTOR_SIZE
 * 
 * \return True if success, false if not
 */
bool flash_write(unsigned int addr, const uint8_t *src);

/*! \brief Read flash
 *
 * \param addr Address to start reading in flash. Must be a multiple of FLASH_PAGE_SIZE
 * \param dst Pointer to store read bytes
 * \param len Number of bytes to read. Should be less than FLASH_SECTOR_SIZE
 * 
 * \return Pointer to the data if success, NULL if failed
 */
bool flash_read(unsigned int addr, uint8_t* dst);

#endif
