/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * $VER: PopupMenuPrefs.c 1.0 (23.7.2026)
 *
 * GadTools preferences editor for popupmenu.library.
 * Loads and saves the modern IFF PREF/PMNU format used by the library
 * (see <prefs/popupmenu.h>).
 *
 * Paths: ENV:sys/PopupMenu.prefs / ENVARC:sys/PopupMenu.prefs
 * Test  - write ENV:sys/, stay open (reload + sample menu)
 * Use   - write ENV:sys/, exit
 * Save  - write ENVARC:sys/, then ENV:sys/, exit
 * Cancel - exit without writing
 */

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/libraries.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <libraries/asl.h>
#include <libraries/iffparse.h>
#include <prefs/prefhdr.h>
#include <prefs/popupmenu.h>
#include <dos/dos.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/asl.h>
#include <proto/iffparse.h>
#include <proto/dos.h>
#include <proto/alib.h>

#include <libraries/pm.h>
#include <proto/pm.h>

#include <string.h>
#include <stdio.h>

/*****************************************************************************/

#define ENV_PATH	"ENV:sys/PopupMenu.prefs"
#define ARC_PATH	"ENVARC:sys/PopupMenu.prefs"

#define WIN_W		440
#define WIN_H		440

/* Gadget IDs */
enum {
	GID_MENUBORDER = 1,
	GID_SEPARATOR,
	GID_SELITEM,
	GID_PULLDOWN,
	GID_ANIMATION,
	GID_IMAGES,
	GID_STICKY,
	GID_SAMEHEIGHT,
	GID_SHADOWS,
	GID_SHADEINT,
	GID_SUBDELAY,
	GID_XOFFSET,
	GID_YOFFSET,
	GID_XSPACE,
	GID_YSPACE,
	GID_INTERMED,
	GID_TEXTDISP,
	GID_TITLE_BOLD,
	GID_TITLE_ITALIC,
	GID_TITLE_UNDER,
	GID_TITLE_SHADOW,
	GID_ITEM_BOLD,
	GID_ITEM_ITALIC,
	GID_ITEM_UNDER,
	GID_ITEM_SHADOW,
	GID_TEST,
	GID_LOAD,
	GID_SAVE,
	GID_USE,
	GID_CANCEL
};

/*****************************************************************************/

struct PopupMenuBase *PopupMenuBase;
struct Library *AslBase;
struct Library *IFFParseBase;
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
struct Library *GadToolsBase;

static struct Screen *scr;
static struct Window *win;
static APTR vi;
static struct Gadget *glist;
static struct Gadget *gad_border;
static struct Gadget *gad_sep;
static struct Gadget *gad_sel;
static struct Gadget *gad_pull;
static struct Gadget *gad_anim;
static struct Gadget *gad_images;
static struct Gadget *gad_sticky;
static struct Gadget *gad_sameh;
static struct Gadget *gad_shadows;
static struct Gadget *gad_shadeint;
static struct Gadget *gad_subdelay;
static struct Gadget *gad_xoff;
static struct Gadget *gad_yoff;
static struct Gadget *gad_xspace;
static struct Gadget *gad_yspace;
static struct Gadget *gad_inter;
static struct Gadget *gad_textdisp;
static struct Gadget *gad_title_bold;
static struct Gadget *gad_title_italic;
static struct Gadget *gad_title_under;
static struct Gadget *gad_title_shadow;
static struct Gadget *gad_item_bold;
static struct Gadget *gad_item_italic;
static struct Gadget *gad_item_under;
static struct Gadget *gad_item_shadow;

static struct PopupMenuPrefs prefs;
static struct PopupMenuPrefs defprefs;
static char status[80];

static STRPTR border_labels[] = {
	"Button Frame",
	"MagicMenu2 Frame",
	"Thick button Frame",
	"Double (Ridge)",
	"Double (DropBox)",
	"Intuition border",
	NULL
};

static STRPTR sep_labels[] = {
	"New Look",
	"Old Look",
	NULL
};

static STRPTR sel_labels[] = {
	"No border",
	"Raised",
	"Recessed",
	NULL
};

static STRPTR pull_labels[] = {
	"Screenbar",
	"Windowbar",
	"Always popup",
	NULL
};

static STRPTR anim_labels[] = {
	"None",
	"Zoom",
	"Fade",
	"Explode",
	NULL
};

static STRPTR images_labels[] = {
	"System",
	"MagicMenu2 8 pixels",
	"MagicMenu2 11 pixels",
	"No Images",
	NULL
};

/*****************************************************************************/

static void SetDefaults(struct PopupMenuPrefs *p)
{
	memset(p, 0, sizeof(*p));
	p->pmp_Flags = 1;
	p->pmp_Animation = PMP_ANIM_NONE;
	p->pmp_PulldownPos = PMP_PD_SCREENBAR;
	p->pmp_MenuBorder = PMP_MENUBORDER_OLDSTYLE;
	p->pmp_SelItemBorder = PMP_SELITEM_NO_BORDER;
	p->pmp_SeparatorBar = 0;
	p->pmp_XOffset = 2;
	p->pmp_YOffset = 2;
	p->pmp_XSpace = 2;
	p->pmp_YSpace = 2;
	p->pmp_Intermediate = 2;
	p->pmp_ShadowR = -30;
	p->pmp_ShadowG = -30;
	p->pmp_ShadowB = -30;
	p->pmp_ImageSet = PMP_IMAGES_SYSTEM;
}

static void SetStatus(STRPTR msg)
{
	strncpy(status, msg, sizeof(status) - 1);
	status[sizeof(status) - 1] = '\0';
	if (win) {
		SetAPen(win->RPort, 1);
		SetDrMd(win->RPort, JAM2);
		Move(win->RPort, 12, WIN_H - 14);
		Text(win->RPort, "                                        ", 40);
		Move(win->RPort, 12, WIN_H - 14);
		Text(win->RPort, status, (ULONG)strlen(status));
	}
}

/*****************************************************************************/
/* IFF PREF / PMNU load and save (matches library iff.c reader) */

static LONG LoadPrefsIFF(STRPTR name, struct PopupMenuPrefs *out)
{
	struct IFFHandle *iff;
	struct ContextNode *cn;
	LONG err;
	LONG got;

	got = 0;
	iff = AllocIFF();
	if (!iff)
		return IFFERR_NOMEM;

	iff->iff_Stream = (ULONG)Open(name, MODE_OLDFILE);
	if (!iff->iff_Stream) {
		FreeIFF(iff);
		return -1;
	}

	InitIFFasDOS(iff);
	err = OpenIFF(iff, IFFF_READ);
	if (err) {
		Close((BPTR)iff->iff_Stream);
		FreeIFF(iff);
		return err;
	}

	/* Walk raw chunks until PMNU (same approach as library iff.c). */
	while (!err) {
		err = ParseIFF(iff, IFFPARSE_RAWSTEP);
		if (err == IFFERR_EOC) {
			err = 0;
			continue;
		}
		if (err)
			break;

		cn = CurrentChunk(iff);
		if (!cn)
			continue;

		if (cn->cn_ID == ID_PMNU) {
			if (cn->cn_Size != (LONG)sizeof(struct PopupMenuPrefs)) {
				err = IFFERR_MANGLED;
				break;
			}
			if (ReadChunkBytes(iff, out, sizeof(*out)) != (LONG)sizeof(*out)) {
				err = IFFERR_READ;
				break;
			}
			got = 1;
			break;
		}
	}

	CloseIFF(iff);
	Close((BPTR)iff->iff_Stream);
	FreeIFF(iff);

	if (!got && !err)
		err = IFFERR_NOTIFF;
	return err;
}

static LONG SavePrefsIFF(STRPTR name, struct PopupMenuPrefs *in)
{
	struct IFFHandle *iff;
	struct PrefHeader ph;
	LONG err;

	memset(&ph, 0, sizeof(ph));
	ph.ph_Version = 0;
	ph.ph_Type = 0;
	ph.ph_Flags = 0;

	iff = AllocIFF();
	if (!iff)
		return IFFERR_NOMEM;

	iff->iff_Stream = (ULONG)Open(name, MODE_NEWFILE);
	if (!iff->iff_Stream) {
		FreeIFF(iff);
		return -1;
	}

	InitIFFasDOS(iff);
	err = OpenIFF(iff, IFFF_WRITE);
	if (err)
		goto fail;

	err = PushChunk(iff, ID_PREF, ID_FORM, IFFSIZE_UNKNOWN);
	if (err)
		goto fail_iff;

	err = PushChunk(iff, 0, ID_PRHD, sizeof(ph));
	if (!err)
		err = (WriteChunkBytes(iff, &ph, sizeof(ph)) != (LONG)sizeof(ph))
			? IFFERR_WRITE : 0;
	if (!err)
		err = PopChunk(iff);
	if (err)
		goto fail_iff;

	err = PushChunk(iff, 0, ID_PMNU, sizeof(*in));
	if (!err)
		err = (WriteChunkBytes(iff, in, sizeof(*in)) != (LONG)sizeof(*in))
			? IFFERR_WRITE : 0;
	if (!err)
		err = PopChunk(iff);
	if (err)
		goto fail_iff;

	err = PopChunk(iff);

fail_iff:
	CloseIFF(iff);
fail:
	Close((BPTR)iff->iff_Stream);
	FreeIFF(iff);
	return err;
}

static void LoadFromENV(void)
{
	LONG err;

	err = LoadPrefsIFF(ENV_PATH, &prefs);
	if (err) {
		prefs = defprefs;
		SetStatus("No ENV:sys prefs - using defaults");
	} else {
		SetStatus("Loaded ENV:sys/PopupMenu.prefs");
	}
}

/* Permanent first (ENVARC), then RAM copy (ENV) — classic prefs Save. */
static LONG SaveToARCThenENV(void)
{
	LONG err;

	err = SavePrefsIFF(ARC_PATH, &prefs);
	if (err)
		return err;
	err = SavePrefsIFF(ENV_PATH, &prefs);
	return err;
}

static LONG SaveToENV(void)
{
	return SavePrefsIFF(ENV_PATH, &prefs);
}

/*****************************************************************************/

static void GadgetsFromPrefs(void)
{
	LONG shade;

	if (!win)
		return;

	GT_SetGadgetAttrs(gad_border, win, NULL,
		GTCY_Active, (ULONG)prefs.pmp_MenuBorder, TAG_DONE);
	GT_SetGadgetAttrs(gad_sep, win, NULL,
		GTCY_Active, (ULONG)(prefs.pmp_SeparatorBar ? 1 : 0), TAG_DONE);
	GT_SetGadgetAttrs(gad_sel, win, NULL,
		GTCY_Active, (ULONG)prefs.pmp_SelItemBorder, TAG_DONE);
	GT_SetGadgetAttrs(gad_pull, win, NULL,
		GTCY_Active, (ULONG)prefs.pmp_PulldownPos, TAG_DONE);
	GT_SetGadgetAttrs(gad_anim, win, NULL,
		GTCY_Active, (ULONG)prefs.pmp_Animation, TAG_DONE);
	GT_SetGadgetAttrs(gad_images, win, NULL,
		GTCY_Active, (ULONG)prefs.pmp_ImageSet, TAG_DONE);
	GT_SetGadgetAttrs(gad_sticky, win, NULL,
		GTCB_Checked, prefs.pmp_Sticky ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_sameh, win, NULL,
		GTCB_Checked, prefs.pmp_SameHeight ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_shadows, win, NULL,
		GTCB_Checked, (prefs.pmp_Flags & 1) ? TRUE : FALSE, TAG_DONE);

	shade = (LONG)prefs.pmp_ShadowR;
	if (shade < -100)
		shade = -100;
	if (shade > 0)
		shade = 0;
	GT_SetGadgetAttrs(gad_shadeint, win, NULL,
		GTSL_Level, shade, TAG_DONE);

	GT_SetGadgetAttrs(gad_subdelay, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_SubMenuDelay, TAG_DONE);
	GT_SetGadgetAttrs(gad_xoff, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_XOffset, TAG_DONE);
	GT_SetGadgetAttrs(gad_yoff, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_YOffset, TAG_DONE);
	GT_SetGadgetAttrs(gad_xspace, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_XSpace, TAG_DONE);
	GT_SetGadgetAttrs(gad_yspace, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_YSpace, TAG_DONE);
	GT_SetGadgetAttrs(gad_inter, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_Intermediate, TAG_DONE);
	GT_SetGadgetAttrs(gad_textdisp, win, NULL,
		GTSL_Level, (LONG)prefs.pmp_TextDisplace + 8, TAG_DONE);

	GT_SetGadgetAttrs(gad_title_bold, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuTitles & PMP_TITLE_BOLD) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_title_italic, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuTitles & PMP_TITLE_ITALIC) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_title_under, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuTitles & PMP_TITLE_UNDERLINE) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_title_shadow, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuTitles & PMP_TITLE_SHADOW) ? TRUE : FALSE, TAG_DONE);

	GT_SetGadgetAttrs(gad_item_bold, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuItems & PMP_TEXT_BOLD) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_item_italic, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuItems & PMP_TEXT_ITALIC) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_item_under, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuItems & PMP_TEXT_UNDERLINE) ? TRUE : FALSE, TAG_DONE);
	GT_SetGadgetAttrs(gad_item_shadow, win, NULL,
		GTCB_Checked, (prefs.pmp_MenuItems & PMP_TEXT_SHADOW) ? TRUE : FALSE, TAG_DONE);
}

static void PrefsFromGadgets(void)
{
	ULONG v;
	LONG shade;
	UBYTE titles;
	UBYTE items;

	GT_GetGadgetAttrs(gad_border, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_MenuBorder = (UBYTE)v;
	GT_GetGadgetAttrs(gad_sep, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_SeparatorBar = (UBYTE)v;
	GT_GetGadgetAttrs(gad_sel, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_SelItemBorder = (UBYTE)v;
	GT_GetGadgetAttrs(gad_pull, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_PulldownPos = (UBYTE)v;
	GT_GetGadgetAttrs(gad_anim, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_Animation = (UBYTE)v;
	GT_GetGadgetAttrs(gad_images, win, NULL, GTCY_Active, &v, TAG_DONE);
	prefs.pmp_ImageSet = (UBYTE)v;

	GT_GetGadgetAttrs(gad_sticky, win, NULL, GTCB_Checked, &v, TAG_DONE);
	prefs.pmp_Sticky = v ? TRUE : FALSE;
	GT_GetGadgetAttrs(gad_sameh, win, NULL, GTCB_Checked, &v, TAG_DONE);
	prefs.pmp_SameHeight = v ? TRUE : FALSE;

	GT_GetGadgetAttrs(gad_shadows, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		prefs.pmp_Flags |= 1;
	else
		prefs.pmp_Flags &= (UBYTE)~1;

	GT_GetGadgetAttrs(gad_shadeint, win, NULL, GTSL_Level, &shade, TAG_DONE);
	prefs.pmp_ShadowR = (BYTE)shade;
	prefs.pmp_ShadowG = (BYTE)shade;
	prefs.pmp_ShadowB = (BYTE)shade;

	GT_GetGadgetAttrs(gad_subdelay, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_SubMenuDelay = (UBYTE)v;
	GT_GetGadgetAttrs(gad_xoff, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_XOffset = (UBYTE)v;
	GT_GetGadgetAttrs(gad_yoff, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_YOffset = (UBYTE)v;
	GT_GetGadgetAttrs(gad_xspace, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_XSpace = (UBYTE)v;
	GT_GetGadgetAttrs(gad_yspace, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_YSpace = (UBYTE)v;
	GT_GetGadgetAttrs(gad_inter, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_Intermediate = (UBYTE)v;
	GT_GetGadgetAttrs(gad_textdisp, win, NULL, GTSL_Level, &v, TAG_DONE);
	prefs.pmp_TextDisplace = (BYTE)((LONG)v - 8);

	titles = 0;
	GT_GetGadgetAttrs(gad_title_bold, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		titles |= PMP_TITLE_BOLD;
	GT_GetGadgetAttrs(gad_title_italic, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		titles |= PMP_TITLE_ITALIC;
	GT_GetGadgetAttrs(gad_title_under, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		titles |= PMP_TITLE_UNDERLINE;
	GT_GetGadgetAttrs(gad_title_shadow, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		titles |= PMP_TITLE_SHADOW;
	prefs.pmp_MenuTitles = titles;

	items = 0;
	GT_GetGadgetAttrs(gad_item_bold, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		items |= PMP_TEXT_BOLD;
	GT_GetGadgetAttrs(gad_item_italic, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		items |= PMP_TEXT_ITALIC;
	GT_GetGadgetAttrs(gad_item_under, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		items |= PMP_TEXT_UNDERLINE;
	GT_GetGadgetAttrs(gad_item_shadow, win, NULL, GTCB_Checked, &v, TAG_DONE);
	if (v)
		items |= PMP_TEXT_SHADOW;
	prefs.pmp_MenuItems = items;
}

static STRPTR ASLPickFile(BOOL save)
{
	struct FileRequester *fr;
	static char path[256];
	STRPTR result;

	result = NULL;
	fr = AllocAslRequestTags(ASL_FileRequest,
		ASLFR_TitleText, save ? (STRPTR)"Save PopupMenu prefs" : (STRPTR)"Load PopupMenu prefs",
		ASLFR_InitialFile, (STRPTR)"PopupMenu.prefs",
		ASLFR_DoSaveMode, save,
		ASLFR_RejectIcons, TRUE,
		TAG_DONE);
	if (fr) {
		if (AslRequest(fr, NULL)) {
			strcpy(path, fr->fr_Drawer);
			AddPart(path, fr->fr_File, sizeof(path));
			result = path;
		}
		FreeAslRequest(fr);
	}
	return result;
}

static void DoTestMenu(void)
{
	struct PopupMenu *menu;
	ULONG code;

	PrefsFromGadgets();
	if (SaveToENV()) {
		SetStatus("Test failed (could not write ENV:sys/)");
		return;
	}
	PM_ReloadPrefs();

	menu = PMMenu("Prefs Test"),
		PMItem("Border / separators"), PMEnd,
		PMItem("Check me"), PM_Checkit, TRUE, PM_Checked, FALSE, PMEnd,
		PMBar, PMEnd,
		PMItem("Sub menu"),
			PMSimpleSub,
				PMItem("Nested item"), PMEnd,
			PMEnd,
		PMEnd,
		PMItem("Done"), PM_UserData, 1, PMEnd,
	PMEnd;

	if (!menu) {
		SetStatus("Could not build test menu");
		return;
	}

	code = (ULONG)PM_OpenPopupMenu(win,
		PM_Menu, menu,
		TAG_DONE);
	PM_FreePopupMenu(menu);
	(void)code;
	SetStatus("Test menu closed (ENV:sys prefs active)");
}

/*****************************************************************************/

static struct Gadget *AddCycle(struct Gadget *prev, struct NewGadget *ng,
	STRPTR label, WORD top, UWORD gid, STRPTR *labels, ULONG active)
{
	ng->ng_LeftEdge = 140;
	ng->ng_TopEdge = top;
	ng->ng_Width = 200;
	ng->ng_Height = 14;
	ng->ng_GadgetText = label;
	ng->ng_GadgetID = gid;
	return CreateGadget(CYCLE_KIND, prev, ng,
		GTCY_Labels, labels,
		GTCY_Active, active,
		TAG_DONE);
}

static struct Gadget *AddCheck(struct Gadget *prev, struct NewGadget *ng,
	STRPTR label, WORD left, WORD top, UWORD gid, BOOL on)
{
	ng->ng_LeftEdge = left;
	ng->ng_TopEdge = top;
	ng->ng_Width = 26;
	ng->ng_Height = 11;
	ng->ng_GadgetText = label;
	ng->ng_GadgetID = gid;
	return CreateGadget(CHECKBOX_KIND, prev, ng,
		GTCB_Checked, on,
		TAG_DONE);
}

static struct Gadget *AddSlider(struct Gadget *prev, struct NewGadget *ng,
	STRPTR label, WORD top, UWORD gid, LONG level, LONG min, LONG max)
{
	ng->ng_LeftEdge = 140;
	ng->ng_TopEdge = top;
	ng->ng_Width = 200;
	ng->ng_Height = 12;
	ng->ng_GadgetText = label;
	ng->ng_GadgetID = gid;
	return CreateGadget(SLIDER_KIND, prev, ng,
		GTSL_Min, min,
		GTSL_Max, max,
		GTSL_Level, level,
		GTSL_LevelFormat, "%3ld",
		GTSL_MaxLevelLen, 4,
		TAG_DONE);
}

static struct Gadget *AddButton(struct Gadget *prev, struct NewGadget *ng,
	STRPTR label, WORD left, WORD top, WORD w, UWORD gid)
{
	ng->ng_LeftEdge = left;
	ng->ng_TopEdge = top;
	ng->ng_Width = w;
	ng->ng_Height = 16;
	ng->ng_GadgetText = label;
	ng->ng_GadgetID = gid;
	return CreateGadget(BUTTON_KIND, prev, ng, TAG_DONE);
}

static BOOL BuildWindow(void)
{
	struct NewGadget ng;
	struct Gadget *g;
	WORD y;
	LONG shade;

	scr = LockPubScreen(NULL);
	if (!scr)
		return FALSE;

	vi = GetVisualInfo(scr, TAG_DONE);
	if (!vi) {
		UnlockPubScreen(NULL, scr);
		return FALSE;
	}

	g = CreateContext(&glist);
	if (!g) {
		FreeVisualInfo(vi);
		UnlockPubScreen(NULL, scr);
		return FALSE;
	}

	memset(&ng, 0, sizeof(ng));
	ng.ng_VisualInfo = vi;
	ng.ng_TextAttr = scr->Font;
	ng.ng_Flags = PLACETEXT_LEFT;

	y = 16;
	gad_border = AddCycle(g, &ng, "Menu Border", y, GID_MENUBORDER,
		border_labels, prefs.pmp_MenuBorder);
	g = gad_border;
	y += 18;
	gad_sep = AddCycle(g, &ng, "Separators", y, GID_SEPARATOR,
		sep_labels, prefs.pmp_SeparatorBar ? 1 : 0);
	g = gad_sep;
	y += 18;
	gad_sel = AddCycle(g, &ng, "Selected item", y, GID_SELITEM,
		sel_labels, prefs.pmp_SelItemBorder);
	g = gad_sel;
	y += 18;
	gad_pull = AddCycle(g, &ng, "Pulldown", y, GID_PULLDOWN,
		pull_labels, prefs.pmp_PulldownPos);
	g = gad_pull;
	y += 18;
	gad_anim = AddCycle(g, &ng, "Animation", y, GID_ANIMATION,
		anim_labels, prefs.pmp_Animation);
	g = gad_anim;
	y += 18;
	gad_images = AddCycle(g, &ng, "Images", y, GID_IMAGES,
		images_labels, prefs.pmp_ImageSet);
	g = gad_images;
	y += 20;

	gad_sticky = AddCheck(g, &ng, "Sticky mouse button", 140, y, GID_STICKY,
		prefs.pmp_Sticky);
	g = gad_sticky;
	y += 16;
	gad_sameh = AddCheck(g, &ng, "Same height items", 140, y, GID_SAMEHEIGHT,
		prefs.pmp_SameHeight);
	g = gad_sameh;
	y += 16;
	gad_shadows = AddCheck(g, &ng, "Menu shadows", 140, y, GID_SHADOWS,
		(prefs.pmp_Flags & 1) ? TRUE : FALSE);
	g = gad_shadows;
	y += 18;

	shade = (LONG)prefs.pmp_ShadowR;
	if (shade < -100)
		shade = -100;
	if (shade > 0)
		shade = 0;
	gad_shadeint = AddSlider(g, &ng, "Shadow intensity", y, GID_SHADEINT,
		shade, -100, 0);
	g = gad_shadeint;
	y += 16;

	gad_subdelay = AddSlider(g, &ng, "Submenu delay", y, GID_SUBDELAY,
		prefs.pmp_SubMenuDelay, 0, 50);
	g = gad_subdelay;
	y += 16;
	gad_xoff = AddSlider(g, &ng, "X Offset", y, GID_XOFFSET,
		prefs.pmp_XOffset, 0, 16);
	g = gad_xoff;
	y += 16;
	gad_yoff = AddSlider(g, &ng, "Y Offset", y, GID_YOFFSET,
		prefs.pmp_YOffset, 0, 16);
	g = gad_yoff;
	y += 16;
	gad_xspace = AddSlider(g, &ng, "X Space", y, GID_XSPACE,
		prefs.pmp_XSpace, 0, 16);
	g = gad_xspace;
	y += 16;
	gad_yspace = AddSlider(g, &ng, "Y Space", y, GID_YSPACE,
		prefs.pmp_YSpace, 0, 16);
	g = gad_yspace;
	y += 16;
	gad_inter = AddSlider(g, &ng, "Intermediate", y, GID_INTERMED,
		prefs.pmp_Intermediate, 0, 16);
	g = gad_inter;
	y += 16;
	gad_textdisp = AddSlider(g, &ng, "Text displace", y, GID_TEXTDISP,
		(LONG)prefs.pmp_TextDisplace + 8, 0, 16);
	g = gad_textdisp;
	y += 20;

	/* Menu title text styles (left column) */
	ng.ng_Flags = PLACETEXT_RIGHT;
	gad_title_bold = AddCheck(g, &ng, "Title Bold", 12, y, GID_TITLE_BOLD,
		(prefs.pmp_MenuTitles & PMP_TITLE_BOLD) ? TRUE : FALSE);
	g = gad_title_bold;
	gad_item_bold = AddCheck(g, &ng, "Item Bold", 220, y, GID_ITEM_BOLD,
		(prefs.pmp_MenuItems & PMP_TEXT_BOLD) ? TRUE : FALSE);
	g = gad_item_bold;
	y += 16;
	gad_title_italic = AddCheck(g, &ng, "Title Italic", 12, y, GID_TITLE_ITALIC,
		(prefs.pmp_MenuTitles & PMP_TITLE_ITALIC) ? TRUE : FALSE);
	g = gad_title_italic;
	gad_item_italic = AddCheck(g, &ng, "Item Italic", 220, y, GID_ITEM_ITALIC,
		(prefs.pmp_MenuItems & PMP_TEXT_ITALIC) ? TRUE : FALSE);
	g = gad_item_italic;
	y += 16;
	gad_title_under = AddCheck(g, &ng, "Title Underlined", 12, y, GID_TITLE_UNDER,
		(prefs.pmp_MenuTitles & PMP_TITLE_UNDERLINE) ? TRUE : FALSE);
	g = gad_title_under;
	gad_item_under = AddCheck(g, &ng, "Item Underlined", 220, y, GID_ITEM_UNDER,
		(prefs.pmp_MenuItems & PMP_TEXT_UNDERLINE) ? TRUE : FALSE);
	g = gad_item_under;
	y += 16;
	gad_title_shadow = AddCheck(g, &ng, "Title Shadowed", 12, y, GID_TITLE_SHADOW,
		(prefs.pmp_MenuTitles & PMP_TITLE_SHADOW) ? TRUE : FALSE);
	g = gad_title_shadow;
	gad_item_shadow = AddCheck(g, &ng, "Item Shadowed", 220, y, GID_ITEM_SHADOW,
		(prefs.pmp_MenuItems & PMP_TEXT_SHADOW) ? TRUE : FALSE);
	g = gad_item_shadow;
	y += 22;

	ng.ng_Flags = 0;
	g = AddButton(g, &ng, "Test", 12, y, 70, GID_TEST);
	g = AddButton(g, &ng, "Load...", 90, y, 70, GID_LOAD);
	g = AddButton(g, &ng, "Save", 168, y, 70, GID_SAVE);
	g = AddButton(g, &ng, "Use", 246, y, 70, GID_USE);
	g = AddButton(g, &ng, "Cancel", 324, y, 70, GID_CANCEL);

	if (!g) {
		FreeGadgets(glist);
		FreeVisualInfo(vi);
		UnlockPubScreen(NULL, scr);
		return FALSE;
	}

	win = OpenWindowTags(NULL,
		WA_Title, (STRPTR)"popupmenu.library Prefs",
		WA_Width, WIN_W,
		WA_Height, WIN_H,
		WA_AutoAdjust, TRUE,
		WA_CloseGadget, TRUE,
		WA_DragBar, TRUE,
		WA_DepthGadget, TRUE,
		WA_Activate, TRUE,
		WA_Gadgets, glist,
		WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW |
			IDCMP_VANILLAKEY | BUTTONIDCMP | CYCLEIDCMP |
			CHECKBOXIDCMP | SLIDERIDCMP,
		WA_PubScreen, scr,
		WA_NewLookMenus, TRUE,
		TAG_DONE);

	if (!win) {
		FreeGadgets(glist);
		FreeVisualInfo(vi);
		UnlockPubScreen(NULL, scr);
		return FALSE;
	}

	GT_RefreshWindow(win, NULL);
	SetStatus("Edit settings, then Save or Use");
	return TRUE;
}

static void CloseUI(void)
{
	if (win) {
		CloseWindow(win);
		win = NULL;
	}
	if (glist) {
		FreeGadgets(glist);
		glist = NULL;
	}
	if (vi) {
		FreeVisualInfo(vi);
		vi = NULL;
	}
	if (scr) {
		UnlockPubScreen(NULL, scr);
		scr = NULL;
	}
}

static BOOL HandleMsg(struct IntuiMessage *im)
{
	struct Gadget *gad;
	UWORD code;
	STRPTR path;
	LONG err;
	BOOL running;

	running = TRUE;
	code = im->Code;

	switch (im->Class) {
	case IDCMP_CLOSEWINDOW:
		running = FALSE;
		break;

	case IDCMP_REFRESHWINDOW:
		GT_BeginRefresh(win);
		GT_EndRefresh(win, TRUE);
		SetStatus(status);
		break;

	case IDCMP_VANILLAKEY:
		if (code == 27)
			running = FALSE;
		break;

	case IDCMP_GADGETUP:
		gad = (struct Gadget *)im->IAddress;
		switch (gad->GadgetID) {
		case GID_TEST:
			DoTestMenu();
			break;

		case GID_LOAD:
			path = ASLPickFile(FALSE);
			if (path) {
				err = LoadPrefsIFF(path, &prefs);
				if (err) {
					SetStatus("Load failed (not modern IFF PREF/PMNU?)");
				} else {
					GadgetsFromPrefs();
					SetStatus("Loaded file");
				}
			}
			break;

		case GID_SAVE:
			PrefsFromGadgets();
			err = SaveToARCThenENV();
			if (err)
				SetStatus("Save failed (ENVARC:sys/ or ENV:sys/)");
			else {
				PM_ReloadPrefs();
				running = FALSE;
			}
			break;

		case GID_USE:
			PrefsFromGadgets();
			err = SaveToENV();
			if (err)
				SetStatus("Use failed (could not write ENV:sys/)");
			else {
				PM_ReloadPrefs();
				running = FALSE;
			}
			break;

		case GID_CANCEL:
			running = FALSE;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return running;
}

/*****************************************************************************/

int main(void)
{
	struct IntuiMessage *im;
	BOOL running;

	SetDefaults(&defprefs);
	prefs = defprefs;

	if (!(IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37)))
		return RETURN_FAIL;
	if (!(GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 37)))
		goto fail;
	if (!(GadToolsBase = OpenLibrary("gadtools.library", 37)))
		goto fail;
	if (!(IFFParseBase = OpenLibrary("iffparse.library", 37)))
		goto fail;
	AslBase = OpenLibrary("asl.library", 37);

	if (!OPEN_PM_LIB) {
		puts("Need popupmenu.library in LIBS:");
		goto fail;
	}

	LoadFromENV();

	if (!BuildWindow()) {
		puts("Could not open prefs window");
		CLOSE_PM_LIB;
		goto fail;
	}

	GadgetsFromPrefs();
	running = TRUE;
	while (running) {
		WaitPort(win->UserPort);
		while ((im = GT_GetIMsg(win->UserPort))) {
			running = HandleMsg(im);
			GT_ReplyIMsg(im);
			if (!running)
				break;
		}
	}

	CloseUI();
	CLOSE_PM_LIB;

fail:
	if (AslBase)
		CloseLibrary(AslBase);
	if (IFFParseBase)
		CloseLibrary(IFFParseBase);
	if (GadToolsBase)
		CloseLibrary(GadToolsBase);
	if (GfxBase)
		CloseLibrary((struct Library *)GfxBase);
	if (IntuitionBase)
		CloseLibrary((struct Library *)IntuitionBase);
	return RETURN_OK;
}
