/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 * But use these as seldom as possible since they are much more slower
 * than regular operations.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 97, 99, 2000, 03, 04, 06 by Ralf Baechle
 */
#ifndef _ASM_MIPS_ATOMIC_H
#define _ASM_MIPS_ATOMIC_H

#ifdef CONFIG_SMP
#error SMP not supported
#endif

#include <linux/types.h>

typedef struct { volatile int counter; } atomic_t;

#define ATOMIC_INIT(i)	  { (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v, i)		((v)->counter = (i))

//TODO is this safe for U-Boot? (IRQs are disabled, right?)

static inline void atomic_add(int i, volatile atomic_t *v)
{
// 	unsigned long flags;

// 	local_irq_save(flags);
	v->counter += i;
// 	local_irq_restore(flags);
}

static inline void atomic_sub(int i, volatile atomic_t *v)
{
// 	unsigned long flags;

// 	local_irq_save(flags);
	v->counter -= i;
// 	local_irq_restore(flags);
}

static inline void atomic_inc(volatile atomic_t *v)
{
// 	unsigned long flags;

// 	local_irq_save(flags);
	v->counter += 1;
// 	local_irq_restore(flags);
}

static inline void atomic_dec(volatile atomic_t *v)
{
// 	unsigned long flags;

// 	local_irq_save(flags);
	v->counter -= 1;
// 	local_irq_restore(flags);
}

#endif /* _ASM_MIPS_ATOMIC_H */
