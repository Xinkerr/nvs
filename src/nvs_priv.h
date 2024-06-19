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
#ifndef __NVS_PRIV_H_
#define __NVS_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * MASKS AND SHIFT FOR ADDRESSES
 * an address in nvs is an uint32_t where:
 *   high 2 bytes represent the sector number
 *   low 2 bytes represent the offset in a sector
 */
#define ADDR_SECT_MASK 0xFFFF0000
#define ADDR_SECT_SHIFT 16
#define ADDR_OFFS_MASK 0x0000FFFF

/*
 * Status return values
 */
#define NVS_STATUS_NOSPACE 1

#define NVS_BLOCK_SIZE 32

#define NVS_LOOKUP_CACHE_NO_ADDR 0xFFFFFFFF

/* Allocation Table Entry */
__packed struct nvs_ate {
	uint16_t id;	/* data id */
	uint16_t offset;	/* data offset within sector */
	uint16_t len;	/* data len within sector */
	uint8_t part;	/* part of a multipart data - future extension */
	uint8_t crc8;	/* crc8 check of the entry */
};

#define COMPILE_ASSERT(expr, msg) \
    extern int compile_assert_##msg[(expr) ? 1 : -1]

COMPILE_ASSERT(offsetof(struct nvs_ate, crc8) == sizeof(struct nvs_ate) - sizeof(uint8_t), crc8_must_be_last);


#ifdef __cplusplus
}
#endif

#endif /* __NVS_PRIV_H_ */
