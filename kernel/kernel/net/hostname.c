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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/utsname.h>

#include <kernel/vmm.h>
#include <kernel/dns.h>
#include <kernel/network.h>

int sys_sethostname(const void *name, size_t len)
{
	if(len > _UTSNAME_LENGTH)
		return errno =-EINVAL;
	if(vmm_check_pointer((void *) name, len) < 0)
		return errno =-EFAULT;
	if((ssize_t) len < 0)
		return errno =-EINVAL;
	/* We need to copy the name, since the user pointer isn't safe */
	char *hostname = malloc(len+1);
	if(!name)
		return errno =-ENOMEM;
	memset(hostname, 0, len+1);
	memcpy(hostname, name, len);
	network_sethostname(hostname);
	
	return 0;
}
int sys_gethostname(char *name, size_t len)
{
	if(vmm_check_pointer(name, len) < 0)
		return errno =-EFAULT;
	if((ssize_t) len < 0)
		return errno =-EINVAL;
	
	size_t str_len = strlen(network_gethostname());
	if(len < str_len)
		return errno =-EINVAL;
	strcpy(name, network_gethostname());
	
	return 0;
}