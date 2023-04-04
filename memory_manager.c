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

//Two Thread Pointers
struct task_sreuct *producer_thread;

void Display(void)
{
	//printk("Test: buffSize = %d", buffSize);
	//printk("Test: prod = %d", prod);
	//printk("Test: cons = %d", cons);
	//printk("Test: uuid = %d", uuid);
}


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
