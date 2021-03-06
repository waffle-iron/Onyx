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
#ifndef _STDIO_IMPL_H
#define _STDIO_IMPL_H
#include <stddef.h>
#include <sys/types.h>

struct _IO_FILE
{
	int fd;
	void *buf;
	size_t buf_size;
	off_t off;
	unsigned int mode;
	volatile long lock;
};

struct _IO_FILE *__stdio_open(const char *path, const char *attrb);
int __stdio_close(struct _IO_FILE *file);
size_t __stdio_write(const void *ptr, size_t size, size_t nmemb, struct _IO_FILE *stream);
size_t __stdio_read(const void *ptr, size_t size, size_t nmemb, struct _IO_FILE *stream);
int __stdio_fseek(struct _IO_FILE *stream, long offset, int whence);
long __stdio_ftell(struct _IO_FILE *stream);
void __stdio_rewind(struct _IO_FILE *stream);
char *__stdio_gets(char *buf);


#define STDIO_DEFAULT_GETS_SIZE 1024
#endif
