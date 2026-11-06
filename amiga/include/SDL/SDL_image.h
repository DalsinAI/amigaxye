/* Xye for AmigaOS (from the TecnoballZ and Biniax-2 ports): the SDL_image
 * 1.2 call it makes, IMG_Load, for its PNGs (amiga/sdl_image_lite.c, on
 * libpng). The OpenGPU kit's SDL 1.2 (sdl12-compat) comes without an
 * SDL_image 1.2. The AmiCradle Team. zlib licence. */
#ifndef AMIGA_SDL_IMAGE_LITE_H
#define AMIGA_SDL_IMAGE_LITE_H
#include <SDL.h>
#ifdef __cplusplus
extern "C" {
#endif
SDL_Surface *IMG_Load(const char *file);
#define IMG_GetError SDL_GetError
#define IMG_SetError SDL_SetError
#ifdef __cplusplus
}
#endif
#endif
