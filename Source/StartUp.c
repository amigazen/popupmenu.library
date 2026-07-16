/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * StartUp.c - LVO trap, function table, and LibInit / LibOpen / LibClose /
 *             LibExpunge / LibReserved for popupmenu.library
 *
 * FuncTab[] order MUST match SDK/SFD/popupmenu_lib.sfd.
 * Link with Sc NOSTARTUP (AsyncIO / ToolKit style) - not SAS/C libent/libinit.
 */

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <dos/dos.h>

#include <proto/exec.h>

#include <libraries/pm.h>
#include "private/pm_build.h"
#include "popupmenu_funcs.h"

extern const char AS_LibName[];
extern const char AS_LibId[];

extern BOOL __asm __saveds OpenLibs(register __a6 struct PopupMenuBase *l);
extern void __asm __saveds __UserLibCleanup(register __a6 struct PopupMenuBase *l);

struct ExecBase *SysBase;
struct PopupMenuBase *PopupMenuBase;
static APTR SegList;

struct InitTable {
	ULONG it_LibSize;
	APTR *it_FuncTable;
	APTR it_DataTable;
	APTR it_InitFunc;
};

static VOID FreeLib(struct PopupMenuBase *lib);

APTR FuncTab[];

struct InitTable InitTab = {
	(ULONG)sizeof(struct PopupMenuBase),
	(APTR *)FuncTab,
	(APTR)NULL,
	(APTR)LibInit
};

/*
 * Standard library vectors, then private/public LVOs from popupmenu_lib.sfd.
 */
APTR FuncTab[] = {
	(APTR)LibOpen,
	(APTR)LibClose,
	(APTR)LibExpunge,
	(APTR)LibReserved,
	/* ==public V2 */
	(APTR)PM_MakeMenuA,
	(APTR)PM_MakeItemA,
	(APTR)PM_FreePopupMenu,
	(APTR)PM_OpenPopupMenuA,
	(APTR)PM_MakeIDListA,
	/* ==public V3 */
	(APTR)PM_ItemChecked,
	(APTR)PM_GetItemAttrsA,
	(APTR)PM_SetItemAttrsA,
	(APTR)PM_FindItem,
	/* ==public V5 */
	(APTR)PM_AlterState,
	/* ==private obsolete Filter */
	(APTR)PM_OBSOLETEFilterIMsgA,
	/* ==public V6 */
	(APTR)PM_ExLstA,
	(APTR)PM_FilterIMsgA,
	/* ==public V8 */
	(APTR)PM_InsertMenuItemA,
	(APTR)PM_RemoveMenuItem,
	/* ==public V9 */
	(APTR)PM_AbortHook,
	(APTR)PM_GetVersion,
	(APTR)PM_ReloadPrefs,
	(APTR)PM_LayoutMenuA,
	/* ==private ARexx */
	(APTR)PM_RexxHost,
	/* ==public V10 */
	(APTR)PM_FreeIDList,
	(APTR)((LONG)-1)
};

LONG
__ASM__ LibReserved(void)
{
	return 0;
}

struct PopupMenuBase *
__ASM__ __SAVE_DS__ LibInit(
	__REG__(a6, struct ExecBase *sysbase),
	__REG__(a0, APTR seglist),
	__REG__(d0, struct PopupMenuBase *lib))
{
	SysBase = sysbase;
	SegList = seglist;
	PopupMenuBase = lib;

	lib->pmb_Library.lib_Node.ln_Type = NT_LIBRARY;
	lib->pmb_Library.lib_Node.ln_Pri = 0;
	lib->pmb_Library.lib_Node.ln_Name = (STRPTR)AS_LibName;
	lib->pmb_Library.lib_Flags = LIBF_CHANGED | LIBF_SUMUSED;
	lib->pmb_Library.lib_Version = PM_LIB_VERSION;
	lib->pmb_Library.lib_Revision = PM_LIB_REVISION;
	lib->pmb_Library.lib_IdString = (STRPTR)AS_LibId;
	lib->pmb_SegList = (ULONG)seglist;
	lib->pmb_ExecBase = (struct Library *)sysbase;

	if (!OpenLibs(lib))
	{
		FreeLib(lib);
		lib = NULL;
		PopupMenuBase = NULL;
	}

	return lib;
}

struct PopupMenuBase *
__ASM__ __SAVE_DS__ LibOpen(__REG__(a6, struct PopupMenuBase *lib))
{
	++lib->pmb_Library.lib_OpenCnt;
	lib->pmb_Library.lib_Flags &= ~LIBF_DELEXP;
	return lib;
}

APTR
__ASM__ __SAVE_DS__ LibClose(__REG__(a6, struct PopupMenuBase *lib))
{
	if (lib->pmb_Library.lib_OpenCnt && --lib->pmb_Library.lib_OpenCnt)
	{
		return NULL;
	}

	if (lib->pmb_Library.lib_Flags & LIBF_DELEXP)
	{
		return LibExpunge(lib);
	}

	return NULL;
}

APTR
__ASM__ __SAVE_DS__ LibExpunge(__REG__(a6, struct PopupMenuBase *lib))
{
	if (lib->pmb_Library.lib_OpenCnt)
	{
		lib->pmb_Library.lib_Flags |= LIBF_DELEXP;
		return NULL;
	}

	Remove(&lib->pmb_Library.lib_Node);
	FreeLib(lib);

	return SegList;
}

static VOID
FreeLib(struct PopupMenuBase *lib)
{
	__UserLibCleanup(lib);
	FreeMem((UBYTE *)lib - lib->pmb_Library.lib_NegSize,
		lib->pmb_Library.lib_NegSize + lib->pmb_Library.lib_PosSize);
	PopupMenuBase = NULL;
}
