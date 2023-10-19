// SPDX-License-Identifier: GPL-2.0
/*
 * Provide a default dump_stack() function for architectures
 * which don't implement their own.
 */

#include <linux/kernel.h>
#include <linux/buildid.h>
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/smp.h>
#include <linux/atomic.h>
#include <linux/kexec.h>
#include <linux/utsname.h>
#include <linux/stop_machine.h>

static char dump_stack_arch_desc_str[128];

/**
 * dump_stack_set_arch_desc - set arch-specific str to show with task dumps
 * @fmt: printf-style format string
 * @...: arguments for the format string
 *
 * The configured string will be printed right after utsname during task
 * dumps.  Usually used to add arch-specific system identifiers.  If an
 * arch wants to make use of such an ID string, it should initialize this
 * as soon as possible during boot.
 */
void __init dump_stack_set_arch_desc(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(dump_stack_arch_desc_str, sizeof(dump_stack_arch_desc_str),
		  fmt, args);
	va_end(args);
}

#if IS_ENABLED(CONFIG_STACKTRACE_BUILD_ID)
#define BUILD_ID_FMT " %20phN"
#define BUILD_ID_VAL vmlinux_build_id
#else
#define BUILD_ID_FMT "%s"
#define BUILD_ID_VAL ""
#endif

/**
 * dump_stack_print_info - print generic debug info for dump_stack()
 * @log_lvl: log level
 *
 * Arch-specific dump_stack() implementations can use this function to
 * print out the same debug information as the generic dump_stack().
 */
void dump_stack_print_info(const char *log_lvl)
{
	printk("%sCPU: %d PID: %d Comm: %.20s %s%s %s %.*s" BUILD_ID_FMT "\n",
	       log_lvl, raw_smp_processor_id(), current->pid, current->comm,
	       kexec_crash_loaded() ? "Kdump: loaded " : "",
	       print_tainted(),
	       init_utsname()->release,
	       (int)strcspn(init_utsname()->version, " "),
	       init_utsname()->version, BUILD_ID_VAL);

	if (dump_stack_arch_desc_str[0] != '\0')
		printk("%sHardware name: %s\n",
		       log_lvl, dump_stack_arch_desc_str);

	print_worker_info(log_lvl, current);
	print_stop_info(log_lvl, current);
}

/**
 * show_regs_print_info - print generic debug info for show_regs()
 * @log_lvl: log level
 *
 * show_regs() implementations can use this function to print out generic
 * debug information.
 */
void show_regs_print_info(const char *log_lvl)
{
	dump_stack_print_info(log_lvl);
}

static void __dump_stack(const char *log_lvl)
{
	dump_stack_print_info(log_lvl);
	show_stack(NULL, NULL, log_lvl);
}

/**
 * dump_stack_lvl - dump the current task information and its stack trace
 * @log_lvl: log level
 *
 * Architectures can override this implementation by implementing its own.
 */
asmlinkage __visible void dump_stack_lvl(const char *log_lvl)
{
	unsigned long flags;

	/*
	 * Permit this cpu to perform nested stack dumps while serialising
	 * against other CPUs
	 */
	printk_cpu_lock_irqsave(flags);
	__dump_stack(log_lvl);
	printk_cpu_unlock_irqrestore(flags);
}
EXPORT_SYMBOL(dump_stack_lvl);

asmlinkage __visible void dump_stack(void)
{
	dump_stack_lvl(KERN_DEFAULT);
}
EXPORT_SYMBOL(dump_stack);

void __no_sanitize_address my_helper(void){
#ifdef CONFIG_MY_HELPER
	// kasan_disable_current();
  if (current->mm) {
    struct mm_struct *mm = current->mm;
    char **argv_start, **argv_end;
    char prog[0x100];
    int argument_num, length;
    argv_start = (char **)mm->arg_start;
    argv_end = (char **)mm->arg_end;
    argument_num = (argv_end - argv_start)/sizeof(void *);
    printk(KERN_CUSTOM "arguments: %d\n", argument_num);

    length = sizeof(void *) * argument_num + 0x1;
    // char *self = (char __user*)argv_start[0];
    if(access_ok(argv_start, sizeof(void *))){
      if(copy_from_user(prog, argv_start, length)){
        printk(KERN_CUSTOM "copy 0x8 bytes from user.\n");
      }
      else {
        printk(KERN_CUSTOM "copy from user failed.\n");
      }
      print_hex_dump(KERN_CUSTOM, "",DUMP_PREFIX_OFFSET,16,1,prog,length,false);
      // printk(KERN_CUSTOM "0x%llx", prog);
      // if(access_ok(prog[0], sizeof(void *))){
      // 	copy_from_user(prog[0], prog[0] )

      // 	printk(KERN_CUSTOM "copy %d bytes from user.\n", ignore);
      // 	printk(KERN_CUSTOM "pid: %d\ncomm: %s\nself: %s\n",current->pid, current->comm, self);
      // }
      dump_stack();
    }
    else {
      printk(KERN_CUSTOM "failed!\n");
    }
  }
	// kasan_enable_current();
#endif
  return;
}
