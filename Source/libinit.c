/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * LibInit.c - ROMTag and version strings for popupmenu.library
 */


#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/resident.h>
#include <clib/compiler-specific.h>

#include "private/pm_build.h"
#include "Rev.h"

const char AS_LibName[] = "popupmenu.library";
const char AS_LibId[] = "popupmenu.library " VERSION " (" DATE ")\r\n";

struct InitTable;
extern struct InitTable InitTab;
extern APTR __ASM__ __SAVE_DS__ LibExpunge(__REG__(a6, struct Library *base));

struct Resident RomTag = {
	RTC_MATCHWORD,
	&RomTag,
	LibExpunge,
	RTF_AUTOINIT,
	VERNUM,
	NT_LIBRARY,
	0,
	(APTR)AS_LibName,
	(APTR)AS_LibId,
	(APTR)&InitTab
};

#ifdef __SASC
/*
 * sc.lib may reference these; shared libraries must not call exit().
 * Do not stub __chkabort - amiga.lib/sc already provide it (multiply defined).
 */
ULONG XCEXIT  = NULL;
ULONG _XCEXIT = NULL;
#endif
