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

//Timer Variables
static struct hrtimer hr_timer;
static struct hrtimer no_restart_hr_timer;
unsigned long timer_interval_ns = 3e9;
static int timer_count = 0;

//Declare variables as readable and rritasble
int pid = 0;
module_param(pid, int, S_IRUSR | S_IWUSR);

//Unsure
//struct task_struct *task = get_current();

//ktime_t timeInterval;

//int counter = 0;


enum hrtimer_restart no_restart_callback(struct hrtimer *timer)
{
    return HRTIMER_NORESTART;
}

enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
    timer_count += 1;
    printk(KERN_INFO "The timer was fired\n");
    if(timer_count < 3)
    {
        ktime_t ktime = ktime_set(0, timer_interval_ns);
        hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL);
        return HRTIMER_RESTART;
    }
    else
    {
        return HRTIMER_NORESTART;
    }
}

void timer_init(void)
{
    //for first two test cases
    ktime_t ktime = ktime_set(0, timer_interval_ns);
    hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    hr_timer.function = &timer_callback;
    hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL);

    //for the third case
    ktime_t ktime_no_restart = ktime_set( 0,timer_interval_ns);
    //Init & Start the hrtimer for NO_RESTART (That isOnly WSS)
    hrtimer_init(&no_restart_hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    no_restart_hr_timer.function = &no_restart_callback;
    hrtimer_start(&no_restart_hr_timer, ktime_no_restart, HRTIMER_MODE_REL);
}


void test(void)
{
    
    //Current Executing process = task 
    struct task_struct *task = get_current();

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

    //For loop traverses the VMA 
    for(vma = mm->mmap; vma; vma = vma->vm_next)
    {
	unsigned long address;
	unsigned long start = vma->vm_start;
	unsigned long end = vma->vm_end;

	//For loop traverses each page in the VMA
        for(address = start; address < end; address += PAGE_SIZE)
	{
	    //Gets pgd and checks if its bad or exists
	    pgd = pgd_offset(mm, address);
	    if(pgd_none(*pgd) || pgd_bad(*pgd)){return;}
	    
	    //Gets p4d and checks if its bad or exists
	    //p4d = p4d_offset(pgd, address);
	    //if (p4d_none(*p4d) || p4d_bad(*p4d)){return;}

	    //Gets pud and checks if its bad or exists
	    //pud = pud_offset(p4d, address);
	    //if(pud_none(*pud) || pud_bad(*pud)){return;}

	    //Get pmd and check if its bad or exists
	    //pmd = pmd_offset(pud, address);
	    //if(pmd_none(*pmd) || pmd_bad(*pmd)){return;}

	    //Gets and stores the pte
	    //ptep = pte_offset_map(pmd, address);
	    //pte = *ptep;

	    //Increase teh counter if the pte has been acessed
	    //if(ptep && ptep_test_and_clear_young(vma, address, ptep))
	    //{
            //    counter += 1;
	    //}
	    //else
	    //{
	    //    return 0;
	    //}

	    //Unamp virtual memoory
	    //pte_unmap(ptep);
	}
    }
}

static int ModuleInit(void)
{    
    printk(KERN_INFO "CSE330 POroject-2 Kernel Module Inserted\n");

    //Call Timer Method
    timer_init();

    //Four Arguments
    printk(KERN_INFO "PID %d: RSS=%d KB, SWAP=%d KB, WSS=%d KB\n", pid, pid, pid, pid);

    test();

//    ktime_t currentTime, interval;
//    currentTime = ktime_get();
//    interval = ktime_set(0, timer_interval_ns);
//
//    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
//    timer.function = &no_restart_callback;
//    hrtimer_start(&timer, timeInterval, HRTIMER_MODE_REL);
//

    printk(KERN_INFO "Got to the end\n");

    return 0;
}

static void ModuleExit(void)
{

    printk(KERN_INFO "CSE330 POroject-2 Kernel Module Removed\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
MODULE_LICENSE("GPL");
