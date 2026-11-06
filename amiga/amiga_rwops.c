/* SDL_RWFromFile for Holotz's Castle on AmigaOS (from the Gigalomania port;
 * there: the game's maps, prefs,
 * saves and pictures are read through RWops on files. OpenGPU's SDL 1.2
 * (sdl12-compat) hands SDL_RWFromFile to SDL2.module, and in the lab that
 * call never returned for the first picture (gfx/starfield.png), as
 * SDL_LoadWAV_RW never did in the OpenLieroX port. Only the game's own
 * code reads these RWops, so they are plain stdio here
 * (-Wl,--wrap=SDL_RWFromFile).
 * The AmiCradle Team. GPL 3 or later (Gigalomania is GPL 2 or later). */
#include <stdio.h>
#include <SDL.h>

static int SDLCALL rw_seek(SDL_RWops *rw, int offset, int whence)
{
	FILE *f = (FILE *)rw->hidden.unknown.data1;
	int w = whence == RW_SEEK_CUR ? SEEK_CUR : whence == RW_SEEK_END ? SEEK_END : SEEK_SET;
	if (fseek(f, offset, w) != 0) {
		SDL_SetError("fseek failed");
		return -1;
	}
	return (int)ftell(f);
}

static int SDLCALL rw_read(SDL_RWops *rw, void *ptr, int size, int maxnum)
{
	if (size <= 0 || maxnum <= 0)
		return 0;
	return (int)fread(ptr, (size_t)size, (size_t)maxnum, (FILE *)rw->hidden.unknown.data1);
}

static int SDLCALL rw_write(SDL_RWops *rw, const void *ptr, int size, int num)
{
	if (size <= 0 || num <= 0)
		return 0;
	return (int)fwrite(ptr, (size_t)size, (size_t)num, (FILE *)rw->hidden.unknown.data1);
}

static int SDLCALL rw_close(SDL_RWops *rw)
{
	int r = 0;
	if (rw) {
		r = fclose((FILE *)rw->hidden.unknown.data1) == 0 ? 0 : -1;
		SDL_FreeRW(rw);
	}
	return r;
}

SDL_RWops *__wrap_SDL_RWFromFile(const char *file, const char *mode)
{
	FILE *f;
	SDL_RWops *rw;
	if (!file || !mode) {
		SDL_SetError("SDL_RWFromFile: no file name or mode");
		return NULL;
	}
	f = fopen(file, mode);
	if (!f) {
		SDL_SetError("Couldn't open %s", file);
		return NULL;
	}
	rw = SDL_AllocRW();
	if (!rw) {
		fclose(f);
		return NULL;
	}
	rw->seek = rw_seek;
	rw->read = rw_read;
	rw->write = rw_write;
	rw->close = rw_close;
	rw->hidden.unknown.data1 = f;
	return rw;
}
