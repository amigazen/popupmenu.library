/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * compiler.h - thin shim over NDK clib/compiler-specific.h
 *
 * Prefer __ASM__ / __REG__ / __SAVE_DS__ directly in new code (popupmenu_funcs.h,
 * StartUp.c). ASM / SAVEDS / GNUCREG remain only for unconverted LVO bodies.
 */

#ifndef POPUPMENU_COMPILER_H
#define POPUPMENU_COMPILER_H

#include <exec/types.h>
#include <clib/compiler-specific.h>

#ifndef PM_INITTABLE_DEFINED
#define PM_INITTABLE_DEFINED 1
struct InitTable
{
	ULONG it_LibSize;
	APTR *it_FuncTable;
	APTR  it_DataTable;
	APTR  it_InitFunc;
};
#endif

/* Temporary aliases for legacy LVO sources (retire in Phase C). */
#ifndef ASM
#define ASM __ASM__
#endif
#ifndef SAVEDS
#define SAVEDS __SAVE_DS__
#endif
#ifndef GNUCREG
#define GNUCREG(r)
#endif

#endif /* POPUPMENU_COMPILER_H */
