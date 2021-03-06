;----------------------------------------------------------------------
; * Copyright (C) 2016, 2017 Pedro Falcato
; *
; * This file is part of Onyx, and is made available under
; * the terms of the GNU General Public License version 2.
; *
; * You can redistribute it and/or modify it under the terms of the GNU
; * General Public License version 2 as published by the Free Software
; * Foundation.
; *----------------------------------------------------------------------
section .text
global tss_flush
tss_flush:
	mov ax, 0x3b
	ltr ax
	ret
global GDT_Flush
GDT_Flush:
	mov rax,rdi
	lgdt [rax]

	push 08h
	push .flush
	retf
.flush:
	mov ax, 10h      ; 0x10 is the offset in the GDT to our data segment
	mov ds, ax        ; Load all data segment selectors
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
global idt_flush
idt_flush:
	mov rax,rdi
	lidt [rax]
	ret
