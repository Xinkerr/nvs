/*
 * Copyright 2024 Xinkerr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __FS_NVS_H__
#define __FS_NVS_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Non-volatile Storage
 * @defgroup nvs Non-volatile Storage
 * @ingroup file_system_storage
 * @{
 * @}
 */

/**
 * @brief Non-volatile Storage Data Structures
 * @defgroup nvs_data_structures Non-volatile Storage Data Structures
 * @ingroup nvs
 * @{
 */

struct nvs_ops
{
	int (*write)(uint32_t addr, const void *data, uint32_t len);
	int (*read)(uint32_t addr, void *data, uint32_t len);
	int (*erase)(uint32_t addr, uint32_t size);

	int (*mutex_init)(void);
	int (*mutex_lock)(void);
	int (*mutex_unlock)(void);
};

struct nvs_fs_property
{
	uint32_t address;
	uint16_t sector_size;
	uint16_t sector_count;
	uint8_t write_block_size;
	uint8_t erase_value;
};

/**
 * @brief Non-volatile Storage File system structure
 *
 * @param offset File system offset in flash
 * @param ate_wra Allocation table entry write address. Addresses are stored as uint32_t:
 * high 2 bytes correspond to the sector, low 2 bytes are the offset in the sector
 * @param data_wra Data write address
 * @param sector_size File system is split into sectors, each sector must be multiple of pagesize
 * @param sector_count Number of sectors in the file systems
 * @param ready Flag indicating if the filesystem is initialized
 * @param nvs_lock Mutex
 * @param flash_device Flash Device runtime structure
 * @param flash_parameters Flash memory parameters structure
 */
struct nvs_fs {
	uint32_t offset;
	uint32_t ate_wra;
	uint32_t data_wra;
	uint16_t sector_size;
	uint16_t sector_count;
	uint8_t write_block_size;
	uint8_t erase_value;
	bool ready;
	// struct k_mutex nvs_lock;
	// const struct device *flash_device;
	// const struct flash_parameters *flash_parameters;
	struct nvs_ops ops;
};

/**
 * @}
 */

/**
 * @brief Non-volatile Storage APIs
 * @defgroup nvs_high_level_api Non-volatile Storage APIs
 * @ingroup nvs
 * @{
 */

/**
 * @brief nvs_mount
 *
 * Mount a NVS file system onto the flash device specified in @p fs.
 *
 * @param fs Pointer to file system
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_mount(struct nvs_fs *fs);

/**
 * @brief nvs_clear
 *
 * Clears the NVS file system from flash.
 * @param fs Pointer to file system
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_clear(struct nvs_fs *fs);

/**
 * @brief nvs_write
 *
 * Write an entry to the file system.
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be written
 * @param data Pointer to the data to be written
 * @param len Number of bytes to be written
 *
 * @return Number of bytes written. On success, it will be equal to the number of bytes requested
 * to be written. When a rewrite of the same data already stored is attempted, nothing is written
 * to flash, thus 0 is returned. On error, returns negative value of errno.h defined error codes.
 */
int nvs_write(struct nvs_fs *fs, uint16_t id, const void *data, size_t len);

/**
 * @brief nvs_delete
 *
 * Delete an entry from the file system
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be deleted
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_delete(struct nvs_fs *fs, uint16_t id);

/**
 * @brief nvs_read
 *
 * Read an entry from the file system.
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be read
 * @param data Pointer to data buffer
 * @param len Number of bytes to be read
 *
 * @return Number of bytes read. On success, it will be equal to the number of bytes requested
 * to be read. When the return value is larger than the number of bytes requested to read this
 * indicates not all bytes were read, and more data is available. On error, returns negative
 * value of errno.h defined error codes.
 */
int nvs_read(struct nvs_fs *fs, uint16_t id, void *data, size_t len);

/**
 * @brief nvs_read_hist
 *
 * Read a history entry from the file system.
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be read
 * @param data Pointer to data buffer
 * @param len Number of bytes to be read
 * @param cnt History counter: 0: latest entry, 1: one before latest ...
 *
 * @return Number of bytes read. On success, it will be equal to the number of bytes requested
 * to be read. When the return value is larger than the number of bytes requested to read this
 * indicates not all bytes were read, and more data is available. On error, returns negative
 * value of errno.h defined error codes.
 */
int nvs_read_hist(struct nvs_fs *fs, uint16_t id, void *data, size_t len, uint16_t cnt);

/**
 * @brief nvs_calc_free_space
 *
 * Calculate the available free space in the file system.
 *
 * @param fs Pointer to file system
 *
 * @return Number of bytes free. On success, it will be equal to the number of bytes that can
 * still be written to the file system. Calculating the free space is a time consuming operation,
 * especially on spi flash. On error, returns negative value of errno.h defined error codes.
 */
int nvs_calc_free_space(struct nvs_fs *fs);

/**
 * @brief nvs_init
 *
 * Initializes a NVS file system in flash.
 *
 *
 * @param fs Pointer to file system
 * @param property Pointer to flash property
 * @param ops Pointer to nvs ops, flash write/read/erase apis
 * @retval 0 Success
 */
int nvs_init(struct nvs_fs *fs, struct nvs_fs_property* property, struct nvs_ops* ops);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __FS_NVS_H__ */
