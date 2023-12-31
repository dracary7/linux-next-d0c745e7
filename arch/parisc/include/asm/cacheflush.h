/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PARISC_CACHEFLUSH_H
#define _PARISC_CACHEFLUSH_H

#include <linux/mm.h>
#include <linux/uaccess.h>
#include <asm/tlbflush.h>

/* The usual comment is "Caches aren't brain-dead on the <architecture>".
 * Unfortunately, that doesn't apply to PA-RISC. */

#include <linux/jump_label.h>

DECLARE_STATIC_KEY_TRUE(parisc_has_cache);
DECLARE_STATIC_KEY_TRUE(parisc_has_dcache);
DECLARE_STATIC_KEY_TRUE(parisc_has_icache);

#define flush_cache_dup_mm(mm) flush_cache_mm(mm)

void flush_user_icache_range_asm(unsigned long, unsigned long);
void flush_kernel_icache_range_asm(unsigned long, unsigned long);
void flush_user_dcache_range_asm(unsigned long, unsigned long);
void flush_kernel_dcache_range_asm(unsigned long, unsigned long);
void purge_kernel_dcache_range_asm(unsigned long, unsigned long);
void flush_kernel_dcache_page_asm(void *);
void flush_kernel_icache_page(void *);

/* Cache flush operations */

void flush_cache_all_local(void);
void flush_cache_all(void);
void flush_cache_mm(struct mm_struct *mm);

void flush_kernel_dcache_page_addr(void *addr);

#define flush_kernel_dcache_range(start,size) \
	flush_kernel_dcache_range_asm((start), (start)+(size));

#define ARCH_IMPLEMENTS_FLUSH_KERNEL_VMAP_RANGE 1
void flush_kernel_vmap_range(void *vaddr, int size);
void invalidate_kernel_vmap_range(void *vaddr, int size);

void flush_cache_vmap_vunmap(unsigned long start, unsigned long end);
#define flush_cache_vmap(start, end)	flush_cache_vmap_vunmap(start, end)
#define flush_cache_vunmap(start, end)	flush_cache_vmap_vunmap(start, end)

#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
void flush_dcache_page(struct page *page);

#define flush_dcache_mmap_lock(mapping)		xa_lock_irq(&mapping->i_pages)
#define flush_dcache_mmap_unlock(mapping)	xa_unlock_irq(&mapping->i_pages)

#define flush_icache_page(vma,page)	do { 		\
	flush_kernel_dcache_page_addr(page_address(page)); \
	flush_kernel_icache_page(page_address(page)); 	\
} while (0)

#define flush_icache_range(s,e)		do { 		\
	flush_kernel_dcache_range_asm(s,e); 		\
	flush_kernel_icache_range_asm(s,e); 		\
} while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
do { \
	flush_cache_page(vma, vaddr, page_to_pfn(page)); \
	memcpy(dst, src, len); \
	flush_kernel_dcache_range_asm((unsigned long)dst, (unsigned long)dst + len); \
} while (0)

#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
do { \
	flush_cache_page(vma, vaddr, page_to_pfn(page)); \
	memcpy(dst, src, len); \
} while (0)

void flush_cache_page(struct vm_area_struct *vma, unsigned long vmaddr, unsigned long pfn);
void flush_cache_range(struct vm_area_struct *vma,
		unsigned long start, unsigned long end);

/* defined in pacache.S exported in cache.c used by flush_anon_page */
void flush_dcache_page_asm(unsigned long phys_addr, unsigned long vaddr);

#define ARCH_HAS_FLUSH_ANON_PAGE
static inline void
flush_anon_page(struct vm_area_struct *vma, struct page *page, unsigned long vmaddr)
{
	if (PageAnon(page)) {
		flush_tlb_page(vma, vmaddr);
		preempt_disable();
		flush_dcache_page_asm(page_to_phys(page), vmaddr);
		preempt_enable();
	}
}

#define ARCH_HAS_FLUSH_ON_KUNMAP
static inline void kunmap_flush_on_unmap(void *addr)
{
	flush_kernel_dcache_page_addr(addr);
}

#endif /* _PARISC_CACHEFLUSH_H */

