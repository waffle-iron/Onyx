/*----------------------------------------------------------------------
 * Copyright (C) 2016, 2017 Pedro Falcato
 *
 * This file is part of Onyx, and is made available under
 * the terms of the GNU General Public License version 2.
 *
 * You can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *----------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <signal.h>

#include <kernel/process.h>
#include <kernel/signal.h>
#include <kernel/task_switching.h>
#include <kernel/vmm.h>
#include <kernel/panic.h>
static uint64_t faulting_address;
const char* exception_msg[] = {
    "Division by zero exception",
    "Debug Trap",
    "Non-maskable interrupt",
    "Hit a breakpoint",
    "Overflow trap",
    "Overflow trap",
    "Fault: Bound range exceeded",
    "Invalid Instruction",
    "FPU not detected",
    "Critical error: DOUBLE FAULT",
    "Invalid TSS",
    "Segment not present",
    "Stack segment fault",
    "General Protection Fault",
    "Page fault at ",
    "",
    "x87 floating point exception",
    "Alignment check exception",
    "Machine check exception",
    "SIMD floating point exception",
    "Virtualization exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "Security exception"
};
static _Bool faulting = false;
static inline void exit_isr_handler()
{
	faulting = false;
}
static inline void enter_isr_handler()
{
	faulting = true;
}
static inline bool is_recursive_fault()
{
	return faulting;
}
void isr_handler(intctx_t *ctx)
{
	int int_no = ctx->int_no;
	uint64_t err_code = ctx->err_code;
	if(is_recursive_fault())
	{
		for(;;);
	}
	if(ctx->rip > VM_HIGHER_HALF)
	{
		printk("Kernel exception %u at %p\n", int_no, ctx->rip);
		printk("Registers: rax: %x\nrbx: %x\nrcx: %x\nrdx: %x\nrdi: %x\nrsi: %x\nrbp: %x\nr8:  %x\nr9:  %x\n\
r10: %x\nr11: %x\nr12: %x\nr13: %x\nr14: %x\nr15: %x\nrsp: %x\nrflags: %x\nds: %x\ncs: %x\n", 
			ctx->rax, ctx->rbx, ctx->rcx, ctx->rdx, ctx->rdi, ctx->rsi, ctx->rbp, ctx->r8, ctx->r9, 
		ctx->r10, ctx->r11, ctx->r12, ctx->r13, ctx->r14, ctx->r15, ctx->rsp, ctx->rflags, ctx->ds, ctx->cs);
		printk("Halting!\n");
		halt();

	}
	printk("Exception %u at %p\n", int_no, ctx->rip);
	printk("Registers: rax: %x\nrbx: %x\nrcx: %x\nrdx: %x\nrdi: %x\nrsi: %x\nrbp: %x\nr8:  %x\nr9:  %x\n\
r10: %x\nr11: %x\nr12: %x\nr13: %x\nr14: %x\nr15: %x\nrsp: %x\nrflags: %x\nds: %x\ncs: %x\n", 
			ctx->rax, ctx->rbx, ctx->rcx, ctx->rdx, ctx->rdi, ctx->rsi, ctx->rbp, ctx->r8, ctx->r9, 
		ctx->r10, ctx->r11, ctx->r12, ctx->r13, ctx->r14, ctx->r15, ctx->rsp, ctx->rflags, ctx->ds, ctx->cs);
	// Enter the isr handler
	enter_isr_handler();
	switch (ctx->int_no) {
	case 0:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 1:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 2:{
			break;
		}
	case 3:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 4:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 5:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 6:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 7:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 8:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 9:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 10:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 11:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 12:{
			panic(exception_msg[int_no]);
			break;
		}
	case 13:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 14:{
			/* A page fault has occurred. */
			/* The faulting address is stored in the CR2 register. */
			__asm__ __volatile__ ("mov %%cr2, %0":"=r"
      				(faulting_address));
		vmm_entry_t *entr = vmm_is_mapped((void*)faulting_address);
		if(!entr)
		{
		pf:
			printf("\n");
			printf("%s0x%X\n",exception_msg[int_no],faulting_address);
			if(err_code & 0x2)
				printf(" caused by a write\n");
			if(err_code & 0x4)
			{
				printf("user-mode\n");
			}
			if(err_code & 0x10)
				printf("Instruction fetch\n");
			if(err_code & (1 << 3))
				printf("Reserved bit was set!\n");
                       sys_kill(current_process->pid, SIGSEGV);
		       break;
		}
		goto pf;
		if(err_code & 0x2 && ~entr->rwx & VMM_WRITE)
			goto pf;
		if(err_code & 0x10 && entr->rwx & VMM_NOEXEC)
			goto pf;
		if(err_code & 0x4 && faulting_address > 0xFFFF800000000000)
			goto pf;
		vmm_map_range((void*)faulting_address, 1, entr->rwx);
		}
	case 15:{
			break;	/*Reserved exception */
		}
	case 16:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 17:{
			break;
		}
	case 18:{
			break;
		}
	case 19:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 20:{
			sys_kill(current_process->pid, SIGSEGV);
			break;
		}
	case 21:		/*Handle the intel reserved exceptions to do nothing */
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		break;
	}

	exit_isr_handler();
}
