#pragma once   //to ensure the current source file to be included only once in a single compilation.
#include "../image.h"

#include <stdio.h>
typedef int(*ImageFormatIdentifier_fp)(FILE* fp); //returns 1->yes 0->no
typedef Image_t*(*ImageLoader_fp)(FILE *fd);

#define IMAGE_IS_SUPPORTED 1
typedef struct image_loader_t
{
    ImageFormatIdentifier_fp identifier;
    ImageLoader_fp loader;

}ImageLoader_t;

#define IMAGE_LOADER_COUNT 1
const ImageLoader_t loaders[IMAGE_LOADER_COUNT] = {NULL};

