#include "./private/ImageLoader.h"
#include "./image.h"

// The bitmap file format: 1.Bitmap file header  2.Bitmap info header 3.Palette data  4.Bitmap data
ImageLoader_t bmp_loader;        //bitmap loader
bmp_loader.identifier = is_bmp;
bmp_loader.loader = load_bmp;

int is_bmp(FILE* fp) {
    char header[2];
    fread(header, 1, 2, fp);
    if (header[0] == 'B' && header[1] == 'M') {
        return 1;
    }
    return 0;
}

Image_t* load_bmp(const char* filename) {

    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Failed to open file %s\n", filename);
        return NULL;
    }

    //The size of the header is always 54 bytes for Windows BMP files, regardless of the image dimensions or pixel format
    unsigned char header[54];    //array of 54 bytes used to store the header of the BMP

    //fread() is used to read the first 54 bytes(as size of unsigned int is 1 byte) of a BMP file into the header array. It returns the number of elements that were read.
    if (fread(header, sizeof(unsigned char), 54, file) != 54) {
        fprintf(stderr, "Error: Invalid bitmap file format\n");
        fclose(file);
        return NULL;
    }
    // first two bytes of a BMP (bitmap) file header contain the file type identifier, which should be the characters "BM" in ASCII encoding
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Error: Invalid bitmap file format\n");
        fclose(file);
        return NULL;
    }

    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned int bpp = *(unsigned short*)&header[28];  

    if (bpp != 24 && bpp != 32) {
        printf("Error: Only 24-bit and 32-bit bitmap images are supported\n");
        fclose(file);
        return NULL;
    }
    unsigned int compression = *(unsigned int*)&header[30];
    if (compression != 0) {
        printf("Error: Compressed bitmap images are not supported\n");
        fclose(file);
        return NULL;
    }
    EnPixelFormat pixel_format;
    if (bpp == 24) {
        pixel_format = 0;  //RGB888
    }
    else {
        pixel_format = 2;  //RGB8888
    }

    unsigned int stride = ((width * (bpp / 8)) + 3) & (~3);
    unsigned int data_size = stride * height;
    unsigned char* data = (unsigned char*)malloc(data_size);
    if (!data) {
        printf("Error: Failed to allocate memory\n");
        fclose(file);
        return NULL;
    }
    if (fread(data, sizeof(unsigned char), data_size, file) != data_size) {
        printf("Error: Invalid bitmap file format\n");
        fclose(file);
        free(data);
        return NULL;
    }
    fclose(file);
    Image_t* image = (Image_t*)malloc(sizeof(Image_t));
    image->width = width;
    image->height = height;
    image->stride = stride;
    image->pixelFormat = pixel_format;
    image->data = data;
    return image;
}


// The information stored in header:
// Bytes 0-1: File type identifier. This is a two-byte field that should contain the characters "BM" in ASCII encoding to indicate that the file is a bitmap.
// Bytes 2-5: Total size of the file in bytes. This is a 32-bit unsigned integer that specifies the size of the BMP file, including the header and image data.
// Bytes 6-9: Reserved. These bytes are reserved for future use and should be set to 0.
// Bytes 10-13: Offset to the start of the image data. This is a 32-bit unsigned integer that specifies the offset from the beginning of the file to the first byte of the image data.
// Bytes 14-17: Size of the bitmap header. This is a 32-bit unsigned integer that specifies the size of the bitmap header in bytes.
// Bytes 18-21: width of image
// Bytes 22-25: height of image
// Bytes 28-29: bites per pixel
// Bytes 30-33: Compression type
// Bytes 34-37: Size of image data
// Bytes 38-41: Horizontal resolution
// Bytes 42-45: Vertical resolution
// Bytes 46-49: Number of colors in the color palette
