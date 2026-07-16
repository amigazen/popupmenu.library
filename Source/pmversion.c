/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * pmversion.c - VER string for PM_GetVersion / version queries
 */

#include "Rev.h"

#ifdef _M68020
#ifdef _M68030
#ifdef _M68040
#ifdef _M68060
#define __CPU__ "060"
#else
#define __CPU__ "040"
#endif
#else
#define __CPU__ "030"
#endif
#else
#define __CPU__ "020"
#endif
#else
#define __CPU__ "000"
#endif

char _LibID[] = "$VER: popupmenu.library " VERSION " (" DATE ")";
