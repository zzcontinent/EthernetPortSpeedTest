/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "memregisters.h"
#include "memutils.h"


int main()
{
    printf("Starting the system to read the AXI slave registers.\n");

    // Map the whole memory bunch
    printf("Mapping the memory needed...\n");

    mem_dev regs;
    regs.base_addr = XPAR_MEMORY_MAPPER_0_S00_AXI_BASEADDR;
    regs.high_addr = XPAR_MEMORY_MAPPER_0_S00_AXI_HIGHADDR;
    regs.dev_id = 0;
    regs.n_regs = 20;
    regs.reg_offset = 4;

    void *mappedCmd = MemoryMapping(regs.base_addr,regs.high_addr);

    printf("Mapping completed...\n");

    //OLED initialization
    mWriteReg((u32)mappedCmd,64,1);

    //OLED Write test
    char dispStr[16] = "hello world!";
    u32 val = 0;
    val |= dispStr[0];
    val |= dispStr[1]<<8;
    val |= dispStr[2]<<16;
    val |= dispStr[3]<<24;
    mWriteReg((u32)mappedCmd,0,val);

    val = 0;
    val |= dispStr[4];
    val |= dispStr[5]<<8;
    val |= dispStr[6]<<16;
    val |= dispStr[7]<<24;
    mWriteReg((u32)mappedCmd,4,val);
    
    val = 0;
    val |= dispStr[8];
    val |= dispStr[9]<<8;
    val |= dispStr[10]<<16;
    val |= dispStr[11]<<24;
    mWriteReg((u32)mappedCmd,8,val);
    
    val = 0;
    val |= dispStr[12];
    val |= dispStr[13]<<8;
    val |= dispStr[14]<<16;
    val |= dispStr[15]<<24;
    mWriteReg((u32)mappedCmd,12,val);

    sleep(1);
    printf("WriteReg finished...\n");
    
    //oled read test
    uint32_t ret[5];
    ret[0] = mReadReg((u32)mappedCmd, 0);
    ret[1] = mReadReg((u32)mappedCmd, 4);
    ret[2] = mReadReg((u32)mappedCmd, 8);
    ret[3] = mReadReg((u32)mappedCmd, 12);
    ret[4]=0;
    printf("%x %x %x %x\n",ret[0],ret[1],ret[2],ret[3]);

    printf("Unmapping the registers...\n");
      munmap((void*)mappedCmd, regs.high_addr-regs.base_addr);
    printf("All done!\n");

    return 0;
}
