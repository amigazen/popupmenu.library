# popupmenu.library

Intuition's built-in menus are powerful but rigid: fixed screen-bar layout, limited submenu nesting, little say over borders, shadows, icons, or multi-select behaviour. Applications that want context menus under the pointer, MagicMenu-like chrome, horizontal groups, or dynamic items built at runtime need something more flexible.

**popupmenu.library** - written by **Henrik Isaksson** from **1996** through **2002** - is a shared library for **popup and pulldown menus** with a user-configurable look and feel. Menus can open at the mouse, along a window title bar, or as traditional pulldowns; items support icons, BOOPSI images, checkmarks, mutual exclusion, bold/italic/shadow text, and virtually unlimited nesting. Preferences (and `ENV:PopupMenu.cfg`) control colours, borders, and shadows without recompiling clients.

This **10.11** release from the **amigazen project** merges the old Aminet **pmuser** (runtime) and **pmdev** (SDK) archives into one tree and modernises the build for **NDK 3.2**.

## [amigazen project](http://www.amigazen.com)

*A web, suddenly*

*Forty years meditation*

*Minds awaken, free*

**amigazen project** uses modern software development tools and methods to update and rerelease classic Amiga open source software. Projects include a new AWeb, a new Amiga Python 2, and the ToolKit project - a universal SDK for Amiga development. All *amigazen project* releases are guaranteed to build against the ToolKit standard so that anyone can download and begin contributing straightaway without having to tailor the toolchain for their own setup.

Henrik Isaksson is not associated with amigazen project. This updated released is redistributed under the BSD 2-Clause License (see [LICENSE.md](LICENSE.md)).

amigazen project philosophy is based on openness:

*Open* to anyone and everyone - *Open* source and free for all - *Open* your mind and create!

PRs for all projects are gratefully received at [GitHub](https://github.com/amigazen/). While the focus now is on classic 68k software, it is intended that all amigazen project releases can be ported to other Amiga-like systems including AROS and MorphOS where feasible.

## History

**popupmenu.library** grew from a small popup helper into a full shared library with prefs, shadows, pulldowns, keyboard control, and developer macros. 

| Era | Version | Maintainer | Notes |
|-----|---------|------------|-------|
| 1996 | 1.0-1.3 | Henrik Isaksson | First releases; submenus and check items |
| 1996 | 2.0 | Henrik Isaksson | Rewritten as a shared library |
| 1996-97 | 3.x-5.x | Henrik Isaksson | Prefs file, shadows, MagicMenu images, pulldowns, blitter windows |
| 1997 | 6.0-7.x | Henrik Isaksson | Multiselect, `PM_FilterIMsg`, sticky mode, submenu hooks |
| 1998-99 | 8.0-9.x | Henrik Isaksson | Insert/remove items, layout tags, locale hooks, tooltips |
| 2000-01 | 10.0-10.8.6 | Henrik Isaksson | Last public Aminet SDK/user notes (Enforcer fix in 10.8.6) |
| 2003 | 10.10 | Henrik Isaksson | Source tree `$VER` stamp `10.10.180` |
| 2026 | **10.11** | **amigazen project** | ToolKit / SFD modernisation; BSD 2-Clause; merged archive |

See [CHANGELOG.md](CHANGELOG.md) for the full guide history from 1.0 onward.

## Contact

- At GitHub https://github.com/amigazen/popupmenu.library/
- On the web at http://www.amigazen.com/toolkit/ (Amiga browser compatible)
- Or email toolkit@amigazen.com

## License

Original author Henrik Isakson has permitted relicensing when the sources were donated to AROS ("Any license is ok with me").
See here: https://github.com/aros-development-team/AROS/blob/master/workbench/libs/popupmenu/LICENSE

This ToolKit release is under the **BSD 2-Clause License** - see [LICENSE.md](LICENSE.md).

*Amiga* is a trademark of **Amiga Inc**.
