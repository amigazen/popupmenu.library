/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * pmtypes.h - OS independent exec/types.h equivalent
 */


#ifndef PM_TYPES_H
#define PM_TYPES_H

#define AMIGA 1

#ifdef AMIGA

#include <exec/types.h>

#else

typedef unsigned long ULONG;
typedef long LONG;

typedef unsigned short UWORD;
typedef short WORD;

typedef unsigned char UBYTE;
typedef char BYTE;

typedef char BOOL;

typedef char * STRPTR;

#define TRUE	1
#define FALSE	0

#endif

#endif /* PM_TYPES_H */
