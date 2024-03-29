#include "idt.h"
#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include <x86_64/cpu/cpu.h>
#include <x86_64/cpu/panic.h>
#include <system/pic/pic.h>

#define IDT_ENTRIES 256

idt_entry_t idt[IDT_ENTRIES];
idt_pointer_t idt_p;
void *irq_handlers[16];

extern uint64_t isr_tbl[];

static const char *exception_strings[32] = {"Division By Zero",
                                            "Debug",
                                            "Nonmaskable Interrupt",
                                            "Breakpoint",
                                            "Overflow",
                                            "Bound Range Exceeded",
                                            "Invalid Opcode",
                                            "Device Not Available",
                                            "Double Fault",
                                            "Coprocessor Segment Overrun",
                                            "Invalid TSS",
                                            "Segment Not Present",
                                            "Stack Segment Fault",
                                            "General Protection Fault",
                                            "Page Fault",
                                            "Reserved",
                                            "x87 FPU Error"
                                            "Alignment Check",
                                            "Machine Check",
                                            "Simd Exception",
                                            "Virtualization Exception",
                                            "Control Protection Exception",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Reserved",
                                            "Hypervisor Injection Exception",
                                            "VMM Communication Exception",
                                            "Security Exception",
                                            "Reserved"};

extern void load_idt(uint64_t);

void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags)
{
  dprintf("[\e[0;32mIDT\e[0m] Setting IDT gate for interrupt %d. Base: "
          "0x%016llX, Sel: 0x%08llX, Flags: 0x%04llX\n",
          num, base, sel, flags);
  idt[num].offset_low = (base & 0xFFFF);
  idt[num].offset_middle = (base >> 16) & 0xFFFF;
  idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
  idt[num].selector = sel;
  idt[num].ist = 0;
  idt[num].flags = flags;
  idt[num].zero = 0;
}

void init_idt()
{
  dprintf("[\e[0;32mIDT\e[0m] Initializing IDT\n");
  idt_p.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
  idt_p.base = (uint64_t)&idt;

  for (size_t i = 0; i < 16; i++)
  {
    irq_handlers[i] = NULL;
  }

  // Remap the PIC
  asm("sti");
  i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, true);

  for (int i = 0; i < IDT_ENTRIES; ++i)
  {
    set_idt_gate(i, isr_tbl[i], 0x28, 0x8E);
  }

  // Enable interrupts on the PIC
  i8259_Disable(); // Mask all interrupts during initialization
  load_idt((uint64_t)&idt_p);
  i8259_Enable(); // Unmask interrupts after IDT is loaded
  asm("cli");
  dprintf("[\e[0;32mIDT\e[0m] IDT Initialization complete\n");
}

void excp_handler(int_frame_t frame)
{
  // dprintf("[\e[0;32mIDT\e[0m] Handling exception with vector %d\n",
  // frame.vector);

  if (frame.vector < 0x20)
  {
    dprintf("[\e[0;31mIDT\e[0m] Exception: %s\n",
            exception_strings[frame.vector]);
    panic(exception_strings[frame.vector], frame);
    hcf();
  }
  else if (frame.vector >= 0x20 && frame.vector <= 0x2f)
  {
    // dprintf("[\e[0;32mIDT\e[0m] Handling IRQ: %d\n", frame.vector - 0x20);

    int irq = frame.vector - 0x20;
    typedef void (*handler_func_t)(int_frame_t *);

    handler_func_t handler = irq_handlers[irq];

    if (handler != NULL)
    {
      // dprintf("[\e[0;32mIDT\e[0m] Found handler for IRQ %d\n", irq);
      handler(&frame);
    }

    i8259_SendEndOfInterrupt(irq);
  }
  else if (frame.vector == 0x80)
  {
    dprintf("[\e[0;32mIDT\e[0m] Handling system call\n");
  }
}

void irq_register(uint8_t irq, void *handler)
{
  dprintf("[\e[0;32mIDT\e[0m] Registering IRQ handler for IRQ %d\n", irq);
  irq_handlers[irq] = handler;
}

void irq_deregister(uint8_t irq)
{
  dprintf("[\e[0;32mIDT\e[0m] Deregistering IRQ handler for IRQ %d\n", irq);
  irq_handlers[irq] = NULL;
}
