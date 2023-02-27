#include "image.h"
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>


Image_t* my_project_image_new(int w, int h, EnPixelFormat format, status_t* status)
{
    Image_t* image=NULL;
    status_t lStatus = MY_PROJECT_STATUS_OK;
    if(w<1 || h<1 || format=enPixelFormat_None)
    {
        lStatus = MY_PROJECT_ARG_INVALID;
    }

    else
    {
        int bytes_per_pixel = 3; //based on format
        int stride = 4; //find using w and next number divisible by 4
        size_t mem_size = h*stride*bytes_per_pixel*sizeof(unsigned char);
        void* memory = malloc(mem_size);

        if(memory != NULL)
        {
            void* imagePtr = malloc(sizeof(Image_t))
            if(imagePtr = NULL)
            {
                free(memory);
                lStatus = MY_PROJECT_NOT_ENOUGH_MEMORY;
            }
            else
            {
                image = (Image_t*)imagePtr;
                image->data = (unsigned char*)memory;
                image->height = h;
                image->width = w;
                image->stride = stride;
                image->pixelFormat=??;

            }
        }
        else
        {
            lStatus = MY_PROJECT_NOT_ENOUGH_MEMORY;
        }
    }

    if(status != NULL)
    {
        *status = lStatus;
    }
    return image;
}

Image_t* my_project_image_load(const char* path, status_t* status)
{

}

status_t my_project_image_delete(Image_t** image)
{
    if(image != NULL && *image!= NULL)
    {
        Image_t* imagePtr = *image;
        if(imagePtr->data != NULL)
        {
            free(imagePtr->data);
            free(imagePtr);
            *image = NULL;
            return MY_PROJECT_STATUS_OK;
        }
        return MY_PROJECT_ARG_INVALID;

    }
    return MY_PROJECT_NULL_ARG;
}