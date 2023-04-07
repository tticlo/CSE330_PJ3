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
#include <linux/mm_types.h>
#include <linux/pgtable.h>
#include <linux/hrtimer.h>

int pid;
module_param(pid, int, 0);

//Timer Variables
static struct hrtimer timer;
ktime_t timeInterval;
unsigned long timer_interval_ns = 10e0;

int counter = 0;

//enum hrtimer_restart no_restart_callback(struct hrtimer *timer)
//{
//    ktime_t currentTime, interval;
//    currentTime = ktime_get();
//    interval = ktime_set(0, timer_interval_ns);
//    hrtimer_forward(timer, currentTime, interval);
//
//    return HRTIMER_RESTART;
//}

static int ModuleInit(void)
{
    printk("Entered ModuleInit");

    //Current Executing process = task 
    struct task_struct *task;

    //Memory Management of task 
    struct mm_struct *mm = task->mm;

   //Virtual Memory Area
    struct vm_area_struct *vma;

    //Five-Level Page Table
    pgd_t *pgd;
    p4d_t *p4d;
    pmd_t *pmd;
    pud_t *pud;
    pte_t *ptep, pte;

    printk("Read in all variables");
 
//    ktime_t currentTime, interval;
//    currentTime = ktime_get();
//    interval = ktime_set(0, timer_interval_ns);
//
//    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
//    timer.function = &no_restart_callback;
//    hrtimer_start(&timer, timeInterval, HRTIMER_MODE_REL);
//
//
//
    //For loop traverses the VMA 
    for(vma = mm->mmap; vma; vma->vm_next)
    {
	unsigned long address;
	unsigned long start = vma->vm_start;
	unsigned long end = vma->vm_end;

	//For loop traverses each page in the VMA
        for(address = start; address < end; address += PAGE_SIZE)
	{
	    //Gets pgd and checks if its bad or exists
	    pgd = pgd_offset(mm, address);
	    if(pgd_none(*pgd) || pgd_bad(*pgd))
	    {
	        return 0;
	    }
	    
	    //Gets p4d and checks if its bad or exists
	    p4d = p4d_offset(pgd, address);
	    if (p4d_none(*p4d) || p4d_bad(*p4d))
	    {
	        return 0;
	    }
	    
	    //Gets pud and checks if its bad or exists
	    pud = pud_offset(p4d, address);
	    if(pud_none(*pud) || pud_bad(*pud))
	    {
	        return 0;
	    }
	    
            //Get pte and check if its bad or exists
	    pmd = pmd_offset(pud, address);
	    if(pmd_none(*pmd) || pmd_bad(*pmd))
	    {
	        return 0;
	    }
//	    
//	    //Gets and stores the pte
//	    ptep = pte_offset_map(pmd, address);
//	    pte = *ptep;
//
//	    //Increase teh counter if the pte has been acessed
//	   // if(ptep && ptep_test_and_clear_young(vma, address, ptep))
//	   //{
//          //     counter += 1;
//	   // }
//	   // else
//	   // {
//	   //     return 0;
//	   // }
//
//	    //Unamp virtual memoory
//	    pte_unmap(ptep);
	}
    }
    return 0;
}

static void ModuleExit(void)
{

}

module_init(ModuleInit);
module_exit(ModuleExit);
MODULE_LICENSE("GPL");
