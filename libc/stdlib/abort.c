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
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#if defined(__is_spartix_kernel)
#include <kernel/panic.h>
#endif				// __is__spartix_kernel
__attribute__ ((__noreturn__))
void abort(void)
{
#ifdef __is_spartix_kernel
	panic("abort()");
#else
	SYSCALL(ABORT_SYSCALL, 0, 0, 0, 0, 0);
#endif
	__builtin_unreachable();
}