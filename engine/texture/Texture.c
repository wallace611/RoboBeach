#include "Texture.h"

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char checkboard[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char brick[TSIZE][TSIZE][4];      /* brick wall textures */

void make_check() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkboard[i][j][0] = c;
            checkboard[i][j][1] = c;
            checkboard[i][j][2] = c;
            checkboard[i][j][3] = 255;
        }
}

void brick_pattern() {
    int y, x, i;

    for (y = 0; y < 12; y++) { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                brick[y][x][i] = brick[y - 32][x][i];
}

void loadTexture(const char *filename, unsigned char** data, int* width, int* height, int* channels) {
    *data = stbi_load(filename, width, height, channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load image: %s\n", filename);
        exit(1);
    }
}


void initSandTexture() {
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(5, textureName);
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    brick_pattern();
    glBindTexture(GL_TEXTURE_2D, textureName[1]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick);

    unsigned char* data = NULL;
    int width, height, channels;
    loadTexture("Pictures/sand.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/water.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/metal.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/rock.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/fabric.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    // 7
    loadTexture("Pictures/water_splash.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/water_splash2.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/tree1.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    loadTexture("Pictures/skydome.png", &data, &width, &height, &channels);
    glBindTexture(GL_TEXTURE_2D, textureName[current]);
    current += 1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

void initTexture() {
    glGenTextures(TEXTURE_COUNT, textureName);
    initSandTexture();
}
