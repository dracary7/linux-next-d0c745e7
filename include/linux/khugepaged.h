/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KHUGEPAGED_H
#define _LINUX_KHUGEPAGED_H

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
extern struct attribute_group khugepaged_attr_group;

extern int khugepaged_init(void);
extern void khugepaged_destroy(void);
extern int start_stop_khugepaged(void);
extern void __khugepaged_enter(struct mm_struct *mm);
extern void __khugepaged_exit(struct mm_struct *mm);
extern void khugepaged_enter_vma(struct vm_area_struct *vma,
				 unsigned long vm_flags);
extern void khugepaged_fork(struct mm_struct *mm,
			    struct mm_struct *oldmm);
extern void khugepaged_exit(struct mm_struct *mm);
extern void khugepaged_enter(struct vm_area_struct *vma,
			     unsigned long vm_flags);

extern void khugepaged_min_free_kbytes_update(void);
#ifdef CONFIG_SHMEM
extern void collapse_pte_mapped_thp(struct mm_struct *mm, unsigned long addr);
#else
static inline void collapse_pte_mapped_thp(struct mm_struct *mm,
					   unsigned long addr)
{
}
#endif

#define khugepaged_enabled()					       \
	(transparent_hugepage_flags &				       \
	 ((1<<TRANSPARENT_HUGEPAGE_FLAG) |		       \
	  (1<<TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG)))
#define khugepaged_always()				\
	(transparent_hugepage_flags &			\
	 (1<<TRANSPARENT_HUGEPAGE_FLAG))
#define khugepaged_defrag()					\
	(transparent_hugepage_flags &				\
	 (1<<TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG))
#else /* CONFIG_TRANSPARENT_HUGEPAGE */
static inline void khugepaged_fork(struct mm_struct *mm, struct mm_struct *oldmm)
{
}
static inline void khugepaged_exit(struct mm_struct *mm)
{
}
static inline void khugepaged_enter(struct vm_area_struct *vma,
				    unsigned long vm_flags)
{
}
static inline void khugepaged_enter_vma(struct vm_area_struct *vma,
					unsigned long vm_flags)
{
}
static inline void collapse_pte_mapped_thp(struct mm_struct *mm,
					   unsigned long addr)
{
}

static inline void khugepaged_min_free_kbytes_update(void)
{
}
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

#endif /* _LINUX_KHUGEPAGED_H */
