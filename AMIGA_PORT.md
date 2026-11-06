Xye 0.12.2 for AmigaOS 3.x
==========================

Xye (Victor Hugo Soliz Kuncar, http://xye.sourceforge.net), the puzzle game
after Kye: guide the green circle around the room, collect every gem and
get past the blocks, arrows, magnets, monsters and timers. Hundreds of
levels in packs (a tutorial, Kye's own and others), skins and a level
editor. For AmigaOS 3.x on a 68040 or 68060 with an FPU and an RTG screen,
on OpenGPU's SDL 1.2 (sdl12-compat).

Base: xye_0.12.2+dfsg.orig.tar.gz from Debian (sha256
3ddd66375219bbfbeabdcd5bf6105810795d26e1d1fc582c7f038f42392e6cfd):
upstream's 0.12.2 without its copy of the DejaVu fonts. Debian's patches
(0.12.2+dfsg-12) are the second commit. Branch team2/xye-amiga.

Status: plays in the lab (Instance-45 on GFOpenUp 0.8.19, 6 November
2026): the level-pack browser with its welcome dialog, the theme
selector with its preview, the tutorial's first level (the player moved
with the cursor keys, the level's solution played to the end with S),
Quit with its question, and back to the Shell; xyeconf.xml, the saved
progress and the last level written in data/.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- GFOpenUp 0.8.18 or later (LIBS:OpenGPU/SDL2.module). Everything else is
  in the program.


How to play
-----------

Open the Xye icon, or run Xye from a Shell. Pick a level pack and Play
(the tutorials first); the cursor keys move, Escape or the Quit button
leaves, S plays a level's solution, F1 is the editor and F3 the themes.
The level packs and skins are in the drawer's data/, and so are the
settings (xyeconf.xml) and your progress (savedgames.conf).


Building
--------

    make -f amiga/Makefile.amigaos -j2 dist

with GFOpenAmigaGCC 16.2.0-open4, the OpenNDK headers, the OpenGPU kit and
the Team's static FreeType, libpng, zlib and bzip2 (see the Makefile's
head). The drawer is dist/Xye (and dist/Xye.info).


What was changed
----------------

- src/vxsdl.cpp: the skin's recolouring unpacks its pixels itself on
  AmigaOS (SDL 1.2's SDL_GetRGBA, inline): each SDL call goes to
  SDL2.module, and once a pixel of the 400x400 skin for each colour the
  game sat for minutes at "Initializing Recolor cache".
- DATA_PATH is PROGDIR:data/ (the levels and res/); Xye keeps its own
  files there too, as it does with no $HOME.
- No SDL_image or SDL_ttf 1.2 for OpenGPU's SDL 1.2: the Team's
  amiga/sdl_image_lite.c (PNG on libpng, from the TecnoballZ and Biniax-2
  ports) and SDL_ttf 2.0.11 (amiga/sdl_ttf, as in the Hex-a-hop port), with
  SDL_RWFromFile on stdio (amiga/amiga_rwops.c, -Wl,--wrap): through
  sdl12-compat SDL_ttf's font file never opened. TinyXML is the Team's copy
  (amiga/tinyxml); Debian builds with the system's.
- The fonts are DejaVu Sans and Sans Bold (amiga/fonts), which Debian's
  source leaves out and Debian's package links from fonts-dejavu.
- amiga/amiga_prefix.hpp: the C++ streams first in every file (the
  toolchain's pthread.h wraps read() and write() in macros once
  <unistd.h> is in), and <ctime> and <cstdlib>. amiga/stack.c: 512 KB.


Licences
--------

Xye: its own zlib-style licence (COPYING.txt). The "detailed" skins: the
Ancestral licence (data/res/detailed_COPYING). Colin Phipps's levels:
free with credit (the licence is in data/levels/Phipps.xye). DejaVu fonts: COPYING-DejaVu.txt.
SDL_ttf 2.0.11 and TinyXML: zlib licence. The Amiga files (amiga/): the
AmiCradle Team, zlib licence, except amiga_rwops.c, which comes from the
Team's Gigalomania port and is GPL 3 or later, so the program as built
here is GPL 3 or later.
