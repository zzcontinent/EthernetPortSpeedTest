/*
 * memutils.h
 *
 *  Created on: Jul 28, 2015
 *      Author: nbarros
 */

#ifndef MEMUTILS_H_
#define MEMUTILS_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

//typedef unsigned char u8;
//typedef unsigned short u16;
typedef uint32_t u32;

// Number of bytes offset between registers
#define REG_OFFSET 4

#define REG0_OFFSET 0
#define REG1_OFFSET 4
#define REG2_OFFSET 8
#define REG3_OFFSET 12

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define QUEUE_SIZE          5

/** Write a value to a register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the CLOCKLOGICdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.                                                     **/
#define mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/** Read a value from a register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the CLOCKLOGIC device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.                       **/
#define mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/// Register offsets
//u32 RegOffset0= REG0_OFFSET;
//u32 RegOffset1= REG1_OFFSET;
//u32 RegOffset2= REG2_OFFSET;
//u32 RegOffset3= REG3_OFFSET;

/// Read to and from addresses
u32 Xil_In32(u32 Addr)
{
  return *(volatile u32 *) Addr;
}

void Xil_Out32(u32 OutAddress, u32 Value)
{
  *(volatile u32 *) OutAddress = Value;
}

/// Memory Mapping Method
/// Takes a physical memory range and returns a virtual address pointing to that range
void* MemoryMapping(u32 BaseAddress, u32 HighAddress)
{
  //Begin memory mapping
  int memfd;
  void *MappedBaseAddress;
  off_t dev_base = BaseAddress;

  // Open Memory location
  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  if (memfd == -1){
   	printf("Can't open /dev/mem.\n");
   	exit(0);
  }

  // Map into user space the area of memory containing the device
  MappedBaseAddress = mmap(0, (HighAddress-BaseAddress), PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~(HighAddress-BaseAddress-1));
  if (MappedBaseAddress == (void *) -1){
   	printf("Can't map the memory to user space.\n");
    exit(0);
  }

  return MappedBaseAddress;
}

//void InitialiseRegs(void* MappedBaseAddress)
//{
//  // Set registers to zero...
//  mWriteReg(MappedBaseAddress, RegOffset0, 0);
//  mWriteReg(MappedBaseAddress, RegOffset1, 0);
//  mWriteReg(MappedBaseAddress, RegOffset2, 0);
//  mWriteReg(MappedBaseAddress, RegOffset3, 0);
//}

#endif /* MEMUTILS_H_ */
