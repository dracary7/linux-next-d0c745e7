/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/kern_levels.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/mm_types.h>

void helper();

void helper(){
  if (current->mm) {
    struct mm_struct *mm = current->mm;
    char **argv_start, **argv_end;
    int argument_num;
    argv_start = (char **)mm->arg_start;
    argv_end = (char **)mm->arg_end;
    argument_num = (argv_end- argv_start)/sizeof(size_t);
    printk(KERN_CUSTOM "arguments: %d\n", argument_num);

    int length = sizeof(size_t)*argument_num+0x1;
    char prog[length];
    memcpy(prog,0,length);
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
}
