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

int pid;
module_param(pid, int, 0);

static int ModuleInit(void)
{
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

   //For loop traverses the VMA 
    for(vma = mm->mmap; vma; vma->vm_next)
    {
	unsigned long address;
	unsigned long start = vma->vm_start;
	unsigned long end = vma->vm_end;

	//For loop traverses each page in the VMA
        for(address = start; address < end; address += PAGE_SIZE)
	{
	    pgd = pgd_offset(mm, address);
	    if(pgd_none(*pgd) || pgd_bad(*pgd))
	    {
	        return 0;
	    }
	    
	    p4d = p4d_offset(pgd, address);
	    if (p4d_none(*p4d) || p4d_bad(*p4d))
	    {
	        return 0;
	    }
	    
	    pud = pud_offset(p4d, address);
	    if(pud_none(*pud) || pud_bad(*pud))
	    {
	        return 0;
	    }
	    
	    pmd = pmd_offset(pud, address);
	    if(pmd_none(*pmd) || pmd_bad(*pmd))
	    {
	        return 0;
	    }
	    
	    ptep = pte_offset_map(pmd, address);
	    if (!ptep)
	    {
	        return 0;
	    } 
	    pte = *ptep;
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
