/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 1996-2002 Henrik Isaksson
 * Copyright (C) 2026 amigazen project
 *
 * pmimages.c - Built-in menu image sets (System / MM2 8 / MM2 11 / None)
 */
#include <exec/types.h>
#include <intuition/intuition.h>
#include <cybergraphx/cybergraphics.h>
#include <proto/cybergraphics.h>
#include "pmpriv.h"
#include "prefs/popupmenu.h"

extern BOOL V40Gfx;

/*****************************************************************************/
/* Shared submenu arrow (kept even for PMP_IMAGES_NONE). */

UBYTE chip arrowData[9 * 2] =
{
	0x80,0x00,0xC0,0x00,0xE0,0x00,0xF0,0x00,0xF8,0x00,0xF0,0x00,0xE0,0x00,0xC0,0x00,
	0x80,0x00
};

struct Image arrow =
{
	0, 0,
	5, 9, 1,
	(void *)arrowData,
	0x0001, 0x0000,
	NULL
};

/*****************************************************************************/
/* Compact MagicMenu-style set (~8px tall). Original planar artwork. */

/* Checkmark 9x8 */
static UBYTE chip mm8_checkData[8 * 2] =
{
	0x02,0x00,
	0x03,0x00,
	0x01,0x80,
	0x00,0xC0,
	0x80,0x60,
	0xC0,0x00,
	0x60,0x00,
	0x00,0x00
};

static struct Image mm8_check =
{
	0, 0, 9, 8, 1,
	(void *)mm8_checkData,
	0x0001, 0x0000,
	NULL
};

/* MX bullet 7x7 */
static UBYTE chip mm8_mxData[7 * 2] =
{
	0x38,0x00,
	0x7C,0x00,
	0xFE,0x00,
	0xFE,0x00,
	0xFE,0x00,
	0x7C,0x00,
	0x38,0x00
};

static struct Image mm8_mx =
{
	0, 0, 7, 7, 1,
	(void *)mm8_mxData,
	0x0001, 0x0000,
	NULL
};

/* Amiga-key box 11x8 */
static UBYTE chip mm8_amigaData[8 * 2] =
{
	0xFF,0xE0,
	0x80,0x20,
	0xB1,0xA0,
	0xA2,0xA0,
	0xA2,0xA0,
	0xB1,0xA0,
	0x80,0x20,
	0xFF,0xE0
};

static struct Image mm8_amiga =
{
	0, 0, 11, 8, 1,
	(void *)mm8_amigaData,
	0x0001, 0x0000,
	NULL
};

/* Submenu chevron 5x8 */
static UBYTE chip mm8_subData[8 * 2] =
{
	0x80,0x00,
	0xC0,0x00,
	0xE0,0x00,
	0xF0,0x00,
	0xE0,0x00,
	0xC0,0x00,
	0x80,0x00,
	0x00,0x00
};

static struct Image mm8_sub =
{
	0, 0, 5, 8, 1,
	(void *)mm8_subData,
	0x0001, 0x0000,
	NULL
};

/*****************************************************************************/
/* Larger MagicMenu-style set (~11px tall). */

static UBYTE chip mm11_checkData[11 * 2] =
{
	0x01,0x00,
	0x01,0x80,
	0x00,0xC0,
	0x00,0x60,
	0x00,0x30,
	0x80,0x18,
	0xC0,0x00,
	0x60,0x00,
	0x30,0x00,
	0x00,0x00,
	0x00,0x00
};

static struct Image mm11_check =
{
	0, 0, 11, 11, 1,
	(void *)mm11_checkData,
	0x0001, 0x0000,
	NULL
};

static UBYTE chip mm11_mxData[11 * 2] =
{
	0x1C,0x00,
	0x3E,0x00,
	0x7F,0x00,
	0x7F,0x00,
	0xFF,0x80,
	0xFF,0x80,
	0xFF,0x80,
	0x7F,0x00,
	0x7F,0x00,
	0x3E,0x00,
	0x1C,0x00
};

static struct Image mm11_mx =
{
	0, 0, 9, 11, 1,
	(void *)mm11_mxData,
	0x0001, 0x0000,
	NULL
};

static UBYTE chip mm11_amigaData[11 * 2] =
{
	0xFF,0xF0,
	0x80,0x10,
	0x80,0x10,
	0xB9,0xD0,
	0xA2,0x50,
	0xA2,0x50,
	0xA2,0x50,
	0xB9,0xD0,
	0x80,0x10,
	0x80,0x10,
	0xFF,0xF0
};

static struct Image mm11_amiga =
{
	0, 0, 12, 11, 1,
	(void *)mm11_amigaData,
	0x0001, 0x0000,
	NULL
};

static UBYTE chip mm11_subData[11 * 2] =
{
	0x80,0x00,
	0xC0,0x00,
	0xE0,0x00,
	0xF0,0x00,
	0xF8,0x00,
	0xFC,0x00,
	0xF8,0x00,
	0xF0,0x00,
	0xE0,0x00,
	0xC0,0x00,
	0x80,0x00
};

static struct Image mm11_sub =
{
	0, 0, 6, 11, 1,
	(void *)mm11_subData,
	0x0001, 0x0000,
	NULL
};

/*****************************************************************************/

struct Image *MakeSysImage(struct PM_Root *p, ULONG image, ULONG ist)
{
	struct Image *img;

	(void)ist;
	img = NewObject(NULL, SYSICLASS,
		SYSIA_DrawInfo, p->DrawInfo,
		SYSIA_Size, SYSISIZE_HIRES,
		SYSIA_Which, image,
		IA_Left, 0,
		IA_Top, 0,
		TAG_DONE);
	return img;
}

static void PM_Image_SetStatic(struct PM_Root *p,
	struct Image *check, struct Image *mx,
	struct Image *amiga, struct Image *sub)
{
	p->MenuImages[PMIMG_CHECKMARK] = check;
	p->MenuImages[PMIMG_EXCLUDE] = mx;
	p->MenuImages[PMIMG_AMIGAKEY] = amiga;
	p->MenuImages[PMIMG_SUBMENU] = sub;
	p->FreeMenuImages = FALSE;
}

static void PM_Image_SetSystem(struct PM_Root *p)
{
	p->MenuImages[PMIMG_AMIGAKEY] = MakeSysImage(p, AMIGAKEY, 0);
	p->MenuImages[PMIMG_CHECKMARK] = MakeSysImage(p, MENUCHECK, 0);
	p->MenuImages[PMIMG_EXCLUDE] = MakeSysImage(p, MENUCHECK, 0);
	p->MenuImages[PMIMG_SUBMENU] = &arrow;
	p->FreeMenuImages = TRUE;
}

void PM_Image_Allocate(struct PM_Root *p)
{
	int i;
	UBYTE set;

	for (i = 0; i < PMIMG_LAST; i++)
		p->MenuImages[i] = 0;
	p->FreeMenuImages = FALSE;

	set = PMP_IMAGES_SYSTEM;
	if (PM_Prefs)
		set = PM_Prefs->pmp_ImageSet;

	switch (set) {
	case PMP_IMAGES_NONE:
		/* Submenu affordance only. */
		p->MenuImages[PMIMG_SUBMENU] = &arrow;
		break;

	case PMP_IMAGES_MM2_8:
		PM_Image_SetStatic(p, &mm8_check, &mm8_mx, &mm8_amiga, &mm8_sub);
		break;

	case PMP_IMAGES_MM2_11:
		PM_Image_SetStatic(p, &mm11_check, &mm11_mx, &mm11_amiga, &mm11_sub);
		break;

	case PMP_IMAGES_SYSTEM:
	default:
		PM_Image_SetSystem(p);
		break;
	}
}

void PM_Image_Free(struct PM_Root *p)
{
	if (p->FreeMenuImages) {
		if (p->MenuImages[PMIMG_AMIGAKEY])
			DisposeObject(p->MenuImages[PMIMG_AMIGAKEY]);
		if (p->MenuImages[PMIMG_CHECKMARK])
			DisposeObject(p->MenuImages[PMIMG_CHECKMARK]);
		if (p->MenuImages[PMIMG_EXCLUDE])
			DisposeObject(p->MenuImages[PMIMG_EXCLUDE]);
	}
	p->MenuImages[PMIMG_AMIGAKEY] = NULL;
	p->MenuImages[PMIMG_CHECKMARK] = NULL;
	p->MenuImages[PMIMG_EXCLUDE] = NULL;
	p->MenuImages[PMIMG_SUBMENU] = NULL;
	p->FreeMenuImages = FALSE;
}

struct PrefsImage *PM_Image_Get(ULONG type, struct PopupMenu *item)
{
	(void)type;
	(void)item;
	return NULL;
}

UWORD PM_Image_Draw(struct PM_Window *w, ULONG type, WORD l, struct DrawInfo *dri, ULONG state, struct PopupMenu *item)
{
	struct PM_Root *p;
	struct Image *img;
	struct RastPort *rp;
	int y;
	WORD mod;
	UBYTE oldDrMd;
	UWORD pen;

	p = w->p;
	img = p->MenuImages[type];
	if (!img)
		return 0;

	rp = w->RPort;
	y = item->Top + item->Height / 2 - img->Height / 2;
	if (l < 0)
		l += item->Left + item->Width - img->Width;

	/*
	 * SysIClass BOOPSI images need DrawImageState.
	 * Static planar glyphs must use BltTemplate + JAM1: DrawImage /
	 * DrawImageState always paint "off" bits as pen 0, which shows as a
	 * grey/black box on white Intuition menus.
	 */
	if (p->FreeMenuImages && type != PMIMG_SUBMENU && type != PMIMG_SUBMENU_S) {
		DrawImageState(rp, img, l, y, state, dri);
	} else {
		if (state == IDS_NORMAL || state == IDS_SELECTED)
			pen = dri->dri_Pens[FILLTEXTPEN];
		else
			pen = dri->dri_Pens[TEXTPEN];

		mod = (WORD)(((img->Width + 15) >> 4) << 1);
		oldDrMd = rp->DrawMode;
		SetAPen(rp, pen);
		SetDrMd(rp, JAM1);
		BltTemplate((PLANEPTR)img->ImageData, 0, mod, rp, l, y,
			img->Width, img->Height);
		SetDrMd(rp, oldDrMd);
	}
	return img->Width;
}

UWORD PM_Image_Height(struct PM_Root *p, ULONG type, struct PopupMenu *item)
{
	(void)item;
	if (p->MenuImages[type])
		return p->MenuImages[type]->Height;
	return 0;
}

UWORD PM_Image_Width(struct PM_Root *p, ULONG type, struct PopupMenu *item)
{
	(void)item;
	if (p->MenuImages[type])
		return p->MenuImages[type]->Width;
	return 0;
}
