#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include <linux/semaphore.h>
#include <linux/time.h>
#include <linux/slab.h>



int pid;

module_param(pid, int, 0);


//void Display(void)
//{
//	printk("Test: prod = %d", prod);
//}

static int ModuleInit(void)
{

	return 0;
}

static void ModuleExit(void)
{

}

module_init(ModuleInit);
module_exit(ModuleExit);
MODULE_LICENSE("GPL");
