/* IMG_Load for Xye on AmigaOS (see include/SDL/SDL_image.h), on libpng,
 * from the Team's TecnoballZ and Biniax-2 ports: there is no SDL_image 1.2 for
 * OpenGPU's SDL 1.2. As SDL_image makes them: palette PNGs become 8-bit
 * surfaces with the PNG's palette, others 32-bit (RGBA when they have
 * alpha), and a tRNS colour is the surface's colour key; interlaced ones
 * too. The AmiCradle Team. zlib licence. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "SDL_image.h"

SDL_Surface *IMG_Load(const char *file)
{
	FILE *f = fopen(file, "rb");
	png_structp png = NULL;
	png_infop info = NULL;
	SDL_Surface *volatile surf = NULL;
	png_bytep *volatile rows = NULL;
	png_uint_32 w, h, y;
	int depth, ctype;

	if (!f) {
		SDL_SetError("IMG_Load: can't open %s", file);
		return NULL;
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png)
		info = png_create_info_struct(png);
	if (!png || !info) {
		SDL_SetError("IMG_Load: out of memory");
		goto out;
	}
	if (setjmp(png_jmpbuf(png))) {
		SDL_SetError("IMG_Load: bad PNG %s", file);
		if (surf)
			SDL_FreeSurface(surf);
		surf = NULL;
		goto out;
	}
	png_init_io(png, f);
	png_read_info(png, info);
	png_get_IHDR(png, info, &w, &h, &depth, &ctype, NULL, NULL, NULL);
	png_set_interlace_handling(png);
	if (ctype == PNG_COLOR_TYPE_PALETTE) {
		png_colorp pal;
		int n, i;
		SDL_Color colors[256];

		if (depth < 8)
			png_set_packing(png);
		png_read_update_info(png, info);
		surf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);
		if (!surf)
			goto out;
		if (png_get_PLTE(png, info, &pal, &n)) {
			if (n > 256)
				n = 256;
			for (i = 0; i < n; i++) {
				colors[i].r = pal[i].red;
				colors[i].g = pal[i].green;
				colors[i].b = pal[i].blue;
				colors[i].unused = 0;
			}
			SDL_SetColors(surf, colors, 0, n);
		}
		/* tRNS: one fully transparent colour is the colour key, as
		 * SDL_image makes it */
		if (png_get_valid(png, info, PNG_INFO_tRNS)) {
			png_bytep trans;
			int ntrans, key = -1;
			png_get_tRNS(png, info, &trans, &ntrans, NULL);
			for (i = 0; i < ntrans; i++) {
				if (trans[i] == 0) {
					if (key >= 0) {
						key = -1;
						break;
					}
					key = i;
				} else if (trans[i] != 255) {
					key = -1;
					break;
				}
			}
			if (key >= 0)
				SDL_SetColorKey(surf, SDL_SRCCOLORKEY, (Uint32)key);
		}
	} else if (ctype & PNG_COLOR_MASK_ALPHA) {
		/* 32-bit RGBA */
		if (depth == 16)
			png_set_strip_16(png);
		if (ctype == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);
		png_read_update_info(png, info);
		surf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
			0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		if (!surf)
			goto out;
	} else {
		/* RGB (or grey) in 32 bits, its tRNS colour the colour key, as
		 * SDL_image makes it */
		png_color_16p tc = NULL;
		int has_key = 0;
		Uint8 kr = 0, kg = 0, kb = 0;

		if (png_get_valid(png, info, PNG_INFO_tRNS) &&
		    png_get_tRNS(png, info, NULL, NULL, &tc) && tc) {
			int sh = depth == 16 ? 8 : 0;
			has_key = 1;
			if (ctype == PNG_COLOR_TYPE_GRAY) {
				int g = tc->gray >> sh;
				if (depth < 8)
					g = g * 255 / ((1 << depth) - 1);
				kr = kg = kb = (Uint8)g;
			} else {
				kr = (Uint8)(tc->red >> sh);
				kg = (Uint8)(tc->green >> sh);
				kb = (Uint8)(tc->blue >> sh);
			}
		}
		if (depth == 16)
			png_set_strip_16(png);
		if (ctype == PNG_COLOR_TYPE_GRAY) {
			if (depth < 8)
				png_set_expand_gray_1_2_4_to_8(png);
			png_set_gray_to_rgb(png);
		}
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		png_read_update_info(png, info);
		surf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
			0xFF000000, 0x00FF0000, 0x0000FF00, 0);
		if (!surf)
			goto out;
		if (has_key)
			SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, kr, kg, kb));
	}
	rows = malloc(sizeof(png_bytep) * h);
	if (!rows) {
		SDL_FreeSurface(surf);
		surf = NULL;
		goto out;
	}
	for (y = 0; y < h; y++)
		rows[y] = (png_bytep) surf->pixels + y * surf->pitch;
	png_read_image(png, rows);
	png_read_end(png, NULL);
out:
	free(rows);
	if (png)
		png_destroy_read_struct(&png, info ? &info : NULL, NULL);
	fclose(f);
	return surf;
}
