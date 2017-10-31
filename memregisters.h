/*
 * memregisters.h
 *
 *  Created on: Jul 28, 2015
 *      Author: nbarros
 */

#ifndef MEMREGISTERS_H_
#define MEMREGISTERS_H_

typedef struct gpio_dev {
	uint32_t base_addr;
	uint32_t high_addr;
	uint32_t dev_id;
	uint32_t has_intr;
	uint32_t is_dual;
} gpio_dev;

typedef struct mem_dev {
	uint32_t base_addr;
	uint32_t high_addr;
	uint32_t dev_id;
	uint32_t n_regs;
	uint32_t reg_offset;
} mem_dev;
// The IP memory ranges
//#define XPAR_MEMORY_MAPPER_0_S00_AXI_BASEADDR 0x43C00000
//#define XPAR_MEMORY_MAPPER_0_S00_AXI_HIGHADDR 0x43C0FFFF

//test axi address
#define XPAR_MEMORY_MAPPER_0_S00_AXI_BASEADDR 0x43c00000
#define XPAR_MEMORY_MAPPER_0_S00_AXI_HIGHADDR 0x43C0FFFF



// Register offset in bytes
#define REGISTER_OFFSET 4

#endif /* MEMREGISTERS_H_ */
