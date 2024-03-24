#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>

#define PIC_REMAP_OFFSET 0x20

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_Disable();
void i8259_Enable();
void i8259_Mask(int irq);
void i8259_Unmask(int irq);
void i8259_SendEndOfInterrupt(int irq);

uint16_t i8259_ReadIrqRequestRegister();
uint16_t i8259_ReadInServiceRegister();

#endif // __PIC_H__