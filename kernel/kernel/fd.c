/* Copyright 2016 Pedro Falcato

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/fd.h>
#include <stdio.h>
ssize_t sys_write(int fd, const void *buf, size_t count)
{
	char *buffer = (char *) buf;
	if (fd == 1)		//stdout
	{
		tty_write(buffer, count);
	} else if (fd == 2)	// stderr
	{
		tty_set_color(0xFF0000);
		tty_write(buffer, count);
		tty_set_color(0xC0C0C0);
	}
	//Implement writing to files,when we support ATA/PATA/SATA/AHCI
	return count;
}

ssize_t sys_read(int fd, const void *buf, size_t count)
{
	if (fd == 0)		//stdin
	{

	}
	return NULL;
}

// Setup the file descriptor table
void fdt_setup(fd_t * fdt)
{
	for (int i = 0; i < MAX_FILDES; i++) {
		fdt[i].fd = i;
		if (i == 0) {
			memcpy((char *) fdt[i].path, "stdin",
			       strlen("stdin"));
		} else if (i == 1) {
			memcpy((char *) fdt[i].path, "stdout",
			       strlen("stdout"));
		} else if (i == 2) {
			memcpy((char *) fdt[i].path, "stderr",
			       strlen("stderr"));
		}
	}
}
