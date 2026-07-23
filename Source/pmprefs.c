/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * pmprefs.c - PopupMenu preferences defaults and loaders
 */

#include <dos/dos.h>
#include <proto/dos.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <string.h>
#include "pmprefs.h"

void PM_LoadPrefsFile(STRPTR filename, ULONG flags, struct PopupMenuPrefs *prefs, struct PopupMenuPrefs *defprefs);

/*
 * Default chrome: Intuition NewLook menus (prefs editor labels).
 * Menu Border 5 = Intuition border, Separators 0 = New Look,
 * Selected item 0 = No border. Images default to System (SysIClass).
 */
struct PopupMenuPrefs DefaultPrefs = {
	1,			/* pmp_Flags		*/
	0,			/* pmp_SubMenuDelay	*/
	PMP_ANIM_NONE,		/* pmp_Animation	*/
	PMP_PD_SCREENBAR,	/* pmp_PulldownPos	*/
	FALSE,			/* pmp_Sticky		*/
	FALSE,			/* pmp_SameHeight	*/
	PMP_MENUBORDER_OLDSTYLE, /* pmp_MenuBorder (Intuition border) */
	PMP_SELITEM_NO_BORDER,	/* pmp_SelItemBorder	*/
	0,			/* pmp_SeparatorBar (New Look) */
	0,			/* pmp_MenuTitles	*/
	0,			/* pmp_MenuItems	*/
	2,			/* pmp_XOffset		*/
	2,			/* pmp_YOffset		*/
	2,			/* pmp_XSpace		*/
	2,			/* pmp_YSpace		*/
	2,			/* pmp_Intermediate	*/
	0,			/* pmp_TextDisplace	*/
	-30,			/* pmp_ShadowR		*/
	-30,			/* pmp_ShadowG		*/
	-30,			/* pmp_ShadowB		*/
	0,			/* pmp_TransparencyR	*/
	0,			/* pmp_TransparencyG	*/
	0,			/* pmp_TransparencyB	*/
	0,			/* pmp_TransparencyBlur	*/
	0,			/* pmp_AnimationSpeed	*/

	{ PMP_IMAGES_SYSTEM },	/* pmp_Reserved[0]=images, rest 0 */
};

struct PopupMenuPrefs LoadedPrefs;

struct PopupMenuPrefs *PM_Prefs = &DefaultPrefs;

void PM_Prefs_Free()
{
}

void PM_Prefs_Load(STRPTR file)
{
	PM_LoadPrefsFile(file, 0, &LoadedPrefs, &DefaultPrefs);
	PM_Prefs = &LoadedPrefs;
}

void __saveds __asm PM_ReloadPrefs(void)
{
	PM_Prefs_Load(PMP_PATH);
}
