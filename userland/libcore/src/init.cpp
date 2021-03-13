/* SPDX-License-Identifier: MIT */

#include <libcore.h>

extern int main(const char *cmdline);

HFILE __console_handle;

extern "C" {
	void _start(const char* cmdline)
	{
		__console_handle = open("/dev/console", 0);
		if (is_error(__console_handle)) exit(1);
		
		int rc = main(cmdline);
		close(__console_handle);
		exit(rc);
	}
}

void exit(int exit_code)
{
	syscall(SYS_EXIT, (unsigned long)exit_code);
	__builtin_unreachable();
}

extern "C" {
	void __stack_chk_fail() {
		exit(1);
	}
}
