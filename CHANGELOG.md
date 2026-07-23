# Changelog

All notable changes to **popupmenu.library** are documented in this file.

Format inspired by [Keep a Changelog](https://keepachangelog.com/).

Versions are listed **newest first**.

---

## [10.11] — 2026-07-23 (amigazen ToolKit)

**Maintainer:** amigazen project

### Added

- Canonical `SDK/SFD/popupmenu_lib.sfd` (NDK 3.2 / LibDescConverter)
- ToolKit `StartUp.c` / `LibInit.c` split (`NOSTARTUP` SAS/C link; no libent/libinit)
- `popupmenu_funcs.h` with `__ASM__` / `__REG__` / `__SAVE_DS__` from `<clib/compiler-specific.h>`
- `SDK/Include_h/` generated headers + `proto/pm.h` compatibility wrappers
- `SDK/Examples/smakefile` to rebuild all C demos with SAS/C
- BSD 2-Clause `LICENSE.md` (per Henrik Isaksson AROS grant)
- GadTools prefs editor `SDK/Examples/PopupMenuPrefs` — load/save modern IFF `PREF`/`PMNU` (`<prefs/popupmenu.h>`)
- Built-in image set selection (`PMP_IMAGES_SYSTEM` / `MM2_8` / `MM2_11` / `NONE` via `pmp_Reserved[0]`)

### Changed

- `Source/smakefile` / `SCOPTIONS` for ToolKit include paths and classic NDK build
- Gated NewGUI / OS4-only headers (`screens.h`, `gui.h`, `GetGUIAttrs`) off SAS/C
- CGFX shade hook uses registerised `h_Entry` (A0/A2/A1) under `params=register`
- Prefs path is now `ENV:sys/PopupMenu.prefs` / `ENVARC:sys/PopupMenu.prefs` (IFF format)
- Intuition-border menus use `SHINEPEN` background; text/emboss/ghosting avoids `SHINEPEN` in that mode
- Default chrome: Intuition border, New Look separators, no selected-item border, System images
- Static menu glyphs (submenu arrow / MM2 set) drawn with `BltTemplate` + JAM1 (no pen-0 box)
- Removed obsolete MUI prefs binary from `SDK/Prefs/` (replaced by GadTools editor)

### Notes

- **ABI unchanged** relative to 10.10: same LVO order and biases as the legacy FD
- Public `PM_FilterIMsgA` pragma offset corrected to **−102** (`0x66`); obsolete stub remains at **−90**
- Prefs editor button semantics: Test → ENV only (stay open); Use → ENV then exit; Save → ENVARC then ENV then exit; Cancel → exit

---

## [10.10] — 2003-04-24

- Source tree `$VER` stamp: `popupmenu.library 10.10.180 (24.04.03)` (Henrik Isaksson).

---

## [10.8.6] — ~2001-01 (Aminet SDK)

- SDK package `pmdev` readme version **10.8.6**; Enforcer hits on menu open fixed (per `pmdev.readme`).

---

## [10.7] — (Henrik Isaksson)

- Changed the version numbering to include a build number.
- Changed the license to use the "standard AFD copyright note".
- Fixed a bug in the shadow code.
- Text shadow/outline now uses `SHADOWPEN` in DrawInfo mode.
- Changed the position of submenus to 2/3 of the active item's width instead of 2/3 of the menu width, so it should work with horizontal groups now.
- Fixed a bug in the pen allocation.

---

## [10.6] — (Henrik Isaksson)

- Text styles are now preserved when an item is active.
- The InputHandler now uses maximum priority to prevent programs from stealing the input.

---

## [10.5] — (Henrik Isaksson)

- Added support for keyboard control:
  - Cursor down/up — select next/previous item
  - Cursor right — open sub menu
  - Cursor left — close sub menu
  - Enter/Return — select item and close menu OR open sub menu
  - Space — select an item OR open sub menu
  - ESC — close menu
- Selecting a checkmark item does no longer redraw the entire menu, unless the item affects other items (i.e. if the item is an MX item).
- Bug fix: MultiSelect did not highlight items immediately when they were selected.
- Implemented the `PM_Insert_Before` insert method for `PM_InsertMenuItem()`.

---

## [10.4] — (Henrik Isaksson)

- Bug fix: MultiSelect highlight should now work correctly with mutual exclude menus.
- Bug fix: The new input handler from v10.03 didn't work as expected.

---

## [10.3] — (Henrik Isaksson)

- Bug fix: A possible problem with MultiSelect menus has been removed.
- The library code has been rewritten so it can be recompiled with other compilers and possibly other OSes like AROS or Phoenix OS.
- The Input Handler has been rewritten and should now be a bit faster and hopefully the kind of dead locks that have been reported in conjunction with some commodities should be resolved quickly.

---

## [10.2] — (Henrik Isaksson)

- Bug fix: MultiSelect now works in "Sticky mode".
- Bug fix: A really ugly bug had found its way into the GetPopupMenuAttrs function; all boolean attributes were returned as TRUE all the time regardless of their actual value. This has been fixed now.
- Bug fix: Now it is possible to disable toggling of checkmark items, including mutual exclude items. Use the tag `PM_Toggle` to turn this feature off. This is recommended for all normal mutual exclude items!
- Introduced new shadow code to avoid overlapping of shadows, and also code for faster rendering of the shadows. This new code also fixes the problems in 16 bit RGB mode.
- Image drawing now uses CyberGraphics functions when available, which speeds up the drawing significantly on graphics board screens.
- An "intermediate space" is now inserted between AmigaKey icons and text.
- Recursive calls now checks the stack, and aborts if the stack is too small.
- New tag: `PM_ExcludeShared` enables sharing of the Exclude list between several items.
- New function: `PM_FreeIDList()` is used to free an Exclude list. When `PM_ExcludeShared` is used, it will not be freed automatically.

---

## [10.1] — (Henrik Isaksson)

- Same feature set as recorded under 10.2 in `PopupMenu.guide` (guide duplicates this block for 10.1 / 10.2).

---

## [9.7] — (Henrik Isaksson)

- Faster image rendering on machines with OS3.1 and gfx board through use of `WriteChunkyPixels()`.
- Bug fix: sometimes the widest item was incorrectly laid out when some other items in the menu had icons.
- Bug fix: a pulldown menu is now moved to the screen bar if its parent window is a backdrop window (or has a narrow title bar).

---

## [9.4] — (Henrik Isaksson)

- Text style settings does not override `PM_Bold`, `PM_Underlined` and `PM_Italic`.

---

## [9.3] — (Henrik Isaksson)

- Removed a bug which was causing Enforcer hits and which made the text settings useless.

---

## [9.2] — (Henrik Isaksson)

- `PM_Right` and `PM_Bottom` did not work correctly. Fixed.
- Icons are now centered horizontally in the icon column.
- If BOOPSI image objects are used as icons or images, they are now drawn in the correct state (`IDS_SELECTED` / `IDS_NORMAL`). This will make the transparency option in the Reaction image classes work with PopupMenu.

---

## [9.0] — (Henrik Isaksson)

- Added `PM_AbortHook()`, a function that should be called periodically from within submenu constructor/destructor hooks, to find out if the user does not want to wait for the menu to complete.
- Added `PM_LocaleHook` and `PM_StringID` which simplifies localization of the menus.
- Added `PM_GetVersion()`, which simply returns the version number of popupmenu.library. This function is intended for use by the prefs editor.
- Added `PM_ReloadPrefs()`, which forces the library to load the prefs file. This function is intended for use by the prefs editor.
- Added new tags for advanced layout (`PM_Members`, `PM_LayoutMode`).
- Submenus now 'walk' across the screen in a different way. When you reach the right side of the screen, they will start popping up to the left of the parent menu, until you reach the left side again.
- Added maximum depth for the shadows.
- Changed the PopupMenu structure. It is now sized dynamically; simple items (e.g. text-only items) require less than half as much memory as before.
- "Old Style" separator bars used to be drawn with the wrong pen, now they're drawn with shadowpen.
- "Old Style/Intuition" borders were incorrectly laid out as being 1 pixel wide, while their actual width is 2 pixels. This caused them to be overwritten sometimes.
- Fixed bug with ColourBoxes appearing under AmigaKey images in check items.
- Fixed keyboard shortcuts that didn't work with check items.
- Added `PM_HintBox`, which makes the menu disappear when the mouse is moved. Intended for showing short hints when the mouse pointer stops over a GUI element ("ToolTips").

---

## [8.20] — (Henrik Isaksson)

- Adjusted the MagicMenu style borders a little bit.
- Added "Raised" mode for selected items.
- Added E modules and examples by Daniel Kasmeroglu to the distribution.
- Added "Intermediate spacing" settings for the space between images and text.
- Added two new text modes, "Outlined" and "Embossed".
- Added colour settings for text outline and shadow.
- Submenus now behave a little better. They used to close at the same point as they opened on; now they close two pixels above this point.
- Added settings for shadow intensity.
- Added ability to adjust the vertical position of the item's text.
- Fixed a bug which caused the non-window mode to trash the screen sometimes.
- Fixed problems caused by omitting the `PM_Menu` tag.
- Rewrote the menu list scanning routines in assembler. This saved 72 bytes, and hopefully made things a little bit faster.
- Items with sub menus are now also selectable, if `PM_NoSelect` is set to FALSE (not reversible).
- Added support for `$POO` by Gareth Murfin.

---

## [8.0] — (Henrik Isaksson)

- Fixed a crash in the BigMenu demo.
- Reworked the layout code and added settings for spacing between borders and text.
- Fixed the 'dropbox' border style mode.
- Added text style settings.
- Added intuition style menu borders.
- Corrected the wide (title) separators in button border mode.
- Added an option to turn off 'real' shadows.
- Added pulldown menu 'windowbar' mode (menus show up at the top of the window instead of the screen bar).
- Replaced the icons with new NewIcons by Brian Gilbert.
- Added two new functions, `PM_InsertMenuItemA` and `PM_RemoveMenuItem`.
- Added a new tag, `PM_Hidden`.
- Ghosted items now has MagicMenu-like ghosting, when MM2 borders and images are selected. Currently only the text will be affected.
- Done a few minor visual adjustments.

---

## [7.50] — (Henrik Isaksson)

- Improved user input routines. The menus used to flicker sometimes when the mouse was moved.
- New remap routine. Optimized by Ralph Wermke.
- Added a submenu destructor hook, `PM_SubDestruct`.

---

## [7.40] — (Henrik Isaksson)

- Added a callback hook for dynamic allocation of menus: `PM_SubConstruct`.
- Translated the remapping code to assembler.

---

## [7.35] — (Henrik Isaksson)

- Bugfix. BYTE read from `$0` when a NULL pointer was passed to `PM_Title`.

---

## [7.3] — (Henrik Isaksson)

- Added a few new tags: `PM_Right`, `PM_Bottom`, `PM_MinWidth`, `PM_MinHeight`, `PM_CenterScreen`, `PM_UseLMB`.
- Removed a few Enforcer hits.
- Made a few changes to the non-window mode. It should work better with CGFX now.

---

## [7.2] — (Henrik Isaksson)

- Added "Sticky mouse button" mode.
- Removed the "Open menu when button is pressed/released" setting (it wasn't supported by any applications). Therefore the `PM_Code` tag is no longer needed.
- Fixed one possible enforcer hit.

---

## [7.1] — (Henrik Isaksson)

- Bugfix. Sometimes the menu could show up on the wrong place.

---

## [7.0] — (Henrik Isaksson)

- The `PM_FilterIMsg()` function has been moved to a new library offset. This was done because the argument passing used the wrong registers. To provide backwards compatibility, the old function is still left.
- The Image system is entirely rewritten. All used images are now stored in the configuration file. This saves some memory and will, in future releases, let the user load his/her own images. Image drawing should also be much faster, especially on chunky screens and on machines with chunky-to-planar converter. There may be some problems with prefs files from release 6.0.
- Fixed a bug in the separator bars (the MM2 style was used even without MagicMenu2 borders selected).
- Added the `PM_ColourBox` tag.

---

## [6.0] — (Henrik Isaksson)

- Made some changes to the API, for keyboard shortcuts and multiselect support. The library should still be 99% backwards compatible. Hacks like the previous version of the BigMenu demo will crash.
- Added keyboard shortcut support through `PM_FilterIMsgA`.
- `PM_ExLstA()` added for easier listing of mutually exclusive item IDs.
- Added support for multiselect.
- Added menu border settings.
- Fixed the "standard 2d images".
- Fixed a problem with pulldown menus when "compact" was not selected.
- Moved some of the images out of the library, and into the prefs editor. The needed images will then be stored in the prefs file (which means that only the needed images will be in memory).

---

## [5.4] — (Henrik Isaksson)

- Moved the submenus 5 pixels up.
- Improved the deadlock detection.
- Improved the look of separator bars.

---

## [5.35] — (Henrik Isaksson)

- Optimized the pen allocation a bit.

---

## [5.3] — (Henrik Isaksson)

- Moved the submenus a bit to the left. Now it looks more like the other menus, and it's easier to open the submenus.
- Fixed a bug in pen allocation/freeing.
- Made a few adjustments to the separator bars.
- Fixed bug in "Submenu Delay" code.
- Fixed an enforcer hit in window mode.
- Got rid of a few unnecessary bytes again (about 1k smaller).

---

## [5.2] — (Henrik Isaksson)

- Fixed delay bug in non-window mode.
- Improved the remapping of magic images. Right look and a few hundred percent faster.
- Pulldown menus should now work in non-window mode too.
- Fixed the shadows. They were 1 pixel too wide before.
- Added a new tag, `PM_Shadowed`, to make the text shadowed.
- Changed the `PMMenu` macro so the titles will use shadowed text.
- Fixed refresh problem in window-mode.
- Made the separator bars look more like MM2.
- And this time the library is actually 1272 bytes smaller!

---

## [5.1] — (Henrik Isaksson)

- Fixed the deadlock bug in non-window mode.
- Added MagicMenu2 Images, and its special remapping (relative color values).
- Added support for pulldown menus (works only in window-mode at the moment).

---

## [5.0] — (Henrik Isaksson)

- Opening the menu does no longer deactivate the active window.
- The menus can now use the blitter instead of windows, which is much faster (this has to be activated in the prefs-editor).
- The menu font is now fetched from the DrawInfo, and not the menu's parent window.
- There is now a space between the bottom of the text and the bottom of the select bar.
- Clicking the mouse button very fast should no longer result in a menu that doesn't disappear (as it should, when you release the button).
- Some of the tags sent to `OpenPopupMenu()` has become obsolete (the preferences has taken over).
- The input method has changed, so the library now requires `commodities.library` (which is a disk library).
- The menu shadows now look like MagicMenu2 shadows (the size increases for each submenu opened).
- Starting with this version, Kickstart 3.0 is required. At least for the non-window mode. In window mode it may still work with 2.0, but it's not tested.
- Fixed a bug. Submenus could appear at the bottom of the screen when the mouse was moved quickly over an item.
- Added a new function, `PM_AlterState()`.

---

## [4.3] — (Henrik Isaksson)

- Fixed a bug (read from address 0).

---

## [4.2] — (Henrik Isaksson)

- Now the menus can have shadows.

---

## [4.1] — (Henrik Isaksson)

- Images are now remapped correctly.
- Small changes for colour and image prefs.

---

## [4.0] — (Henrik Isaksson)

- More bugs fixed.
- A new demo, MenuVerify, shows how to use popupmenu.library with `IDCMP_MENUVERIFY`.

---

## [3.6] — (Henrik Isaksson)

- Bug fix (never released; more bug reports arrived).

---

## [3.5] — (Henrik Isaksson)

- Now checks for the file `ENV:PopupMenu.cfg`, and if it exists, loads it, and replaces the default settings.
- Bug in `PM_SubMenuTimer` fixed.
- `PM_Code` added.

---

## [3.0] — (Henrik Isaksson)

- Added `PM_GetItemAttrs()`, `PM_SetItemAttrs()`, `PM_IsChecked` and `PM_FindItem`.
- Added the tags `PM_Left` and `PM_Top`.
- Added a few more demos.
- Changed the naming of functions and macros to avoid interfering with other libraries.

---

## [2.0] — (Henrik Isaksson)

- Entirely rewritten and is now a shared library!

---

## [1.3] — (Henrik Isaksson)

- Added submenu support.

---

## [1.2] — (Henrik Isaksson)

- Lots of new flags, and checkable menu items.

---

## [1.1] — (Henrik Isaksson)

- `OpenPopupMenuPos()`.

---

## [1.0] — (Henrik Isaksson)

- First release.
