#pragma once

#include <stb_image.h>

#define TEXTURE_COUNT 20

static int current = 0;
unsigned int textureName[TEXTURE_COUNT];

#define TSIZE 64
extern unsigned char checkboard[TSIZE][TSIZE][4]; /* checkboard textures */
extern unsigned char brick[TSIZE][TSIZE][4];      /* brick wall textures */

void initTexture();