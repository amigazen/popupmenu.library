/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * popupmenu_funcs.h - LVO declarations for popupmenu.library sources
 *
 * Register layout matches SDK/SFD/popupmenu_lib.sfd and FuncTab[] in StartUp.c.
 * Uses NDK <clib/compiler-specific.h> macros only - no project ASM/SAVEDS aliases.
 */

#ifndef POPUPMENU_FUNCS_H
#define POPUPMENU_FUNCS_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/execbase.h>
#include <utility/tagitem.h>
#include <intuition/intuition.h>
#include <clib/compiler-specific.h>
#include <libraries/pm.h>

struct PopupMenuBase *__ASM__ __SAVE_DS__ LibInit(
	__REG__(a6, struct ExecBase *sysbase),
	__REG__(a0, APTR seglist),
	__REG__(d0, struct PopupMenuBase *lib));
struct PopupMenuBase *__ASM__ __SAVE_DS__ LibOpen(
	__REG__(a6, struct PopupMenuBase *lib));
APTR __ASM__ __SAVE_DS__ LibClose(
	__REG__(a6, struct PopupMenuBase *lib));
APTR __ASM__ __SAVE_DS__ LibExpunge(
	__REG__(a6, struct PopupMenuBase *lib));
LONG __ASM__ LibReserved(void);

/* Public + private LVOs - same order as SDK/SFD/popupmenu_lib.sfd */
struct PopupMenu *__ASM__ __SAVE_DS__ PM_MakeMenuA(
	__REG__(a1, struct TagItem *tags));
struct PopupMenu *__ASM__ __SAVE_DS__ PM_MakeItemA(
	__REG__(a1, struct TagItem *tags));
VOID __ASM__ __SAVE_DS__ PM_FreePopupMenu(
	__REG__(a1, struct PopupMenu *menu));
APTR __ASM__ __SAVE_DS__ PM_OpenPopupMenuA(
	__REG__(a1, struct Window *wnd),
	__REG__(a2, struct TagItem *tags));
struct PM_IDLst *__ASM__ __SAVE_DS__ PM_MakeIDListA(
	__REG__(a1, struct TagItem *tags));
BOOL __ASM__ __SAVE_DS__ PM_ItemChecked(
	__REG__(a1, struct PopupMenu *pm),
	__REG__(d1, ULONG id));
LONG __ASM__ __SAVE_DS__ PM_GetItemAttrsA(
	__REG__(a2, struct PopupMenu *item),
	__REG__(a1, struct TagItem *tags));
LONG __ASM__ __SAVE_DS__ PM_SetItemAttrsA(
	__REG__(a2, struct PopupMenu *item),
	__REG__(a1, struct TagItem *tags));
struct PopupMenu *__ASM__ __SAVE_DS__ PM_FindItem(
	__REG__(a1, struct PopupMenu *menu),
	__REG__(d1, ULONG id));
VOID __ASM__ __SAVE_DS__ PM_AlterState(
	__REG__(a1, struct PopupMenu *menu),
	__REG__(a2, struct PM_IDLst *idlst),
	__REG__(d1, UWORD action));
APTR __ASM__ __SAVE_DS__ PM_OBSOLETEFilterIMsgA(void);
struct PM_IDLst *__ASM__ __SAVE_DS__ PM_ExLstA(
	__REG__(a1, ULONG *id));
APTR __ASM__ __SAVE_DS__ PM_FilterIMsgA(
	__REG__(a0, struct Window *window),
	__REG__(a1, struct PopupMenu *menu),
	__REG__(a2, struct IntuiMessage *imsg),
	__REG__(a3, struct TagItem *tags));
LONG __ASM__ __SAVE_DS__ PM_InsertMenuItemA(
	__REG__(a0, struct PopupMenu *menu),
	__REG__(a1, struct TagItem *tags));
struct PopupMenu *__ASM__ __SAVE_DS__ PM_RemoveMenuItem(
	__REG__(a0, struct PopupMenu *menu),
	__REG__(a1, struct PopupMenu *item));
BOOL __ASM__ __SAVE_DS__ PM_AbortHook(
	__REG__(a0, APTR handle));
STRPTR __ASM__ __SAVE_DS__ PM_GetVersion(void);
VOID __ASM__ __SAVE_DS__ PM_ReloadPrefs(void);
LONG __ASM__ __SAVE_DS__ PM_LayoutMenuA(
	__REG__(a0, struct Window *window),
	__REG__(a1, struct PopupMenu *menu),
	__REG__(a2, struct TagItem *tags));
ULONG __ASM__ __SAVE_DS__ PM_RexxHost(
	__REG__(a0, APTR rxmsg));
VOID __ASM__ __SAVE_DS__ PM_FreeIDList(
	__REG__(a0, struct PM_IDLst *list));

#endif /* POPUPMENU_FUNCS_H */
