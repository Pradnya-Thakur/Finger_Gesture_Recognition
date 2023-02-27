#ifndef _MY_PROJECT_INCLUDED_IMAGE_H_   //conditional compilation
#define _MY_PROJECT_INCLUDED_IMAGE_H_

typedef int status_t;

#define MY_PROJECT_STATUS_OK (0)
#define MY_PROJECT_ARG_INVALID (-1)
#define MY_PROJECT_NULL_ARG (MY_PROJECT_ARG_INVALID-1)
#define MY_PROJECT_NOT_ENOUGH_MEMORY (MY_PROJECT_NULL_ARG-1)
#define MY_PROJECT_NOT_SUPPORTED (MY_PROJECT_NOT_ENOUGH_MEMORY-1)

typedef enum
{
    enPixelFormat_None = -1,
    enPixelFormat_RGB888,    //0
    enPixelFormat_RGB565,    //1
    enPixelFormat_ARGB8888,  //2
    enPixelFormat_Mono       //3

}EnPixelFormat;

typedef struct image_t_
{
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    EnPixelFormat pixelFormat;
    unsigned char*data;

}Image_t;


Image_t* my_project_image_new(int w, int h, EnPixelFormat format, status_t* status);
Image_t* my_project_image_load(const char* path, status_t* status);
status_t my_project_image_delete(Image_t** image);

#endif // !_MY_PROJECT_INCLUDED_IMAGE_H_  (Closing directive for #ifndef)