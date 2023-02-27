#include "image.h"
#include "./private/ImageLoader.h"
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>


Image_t* my_project_image_new(int w, int h, EnPixelFormat format, status_t* status)
{
    Image_t* image=NULL;
    status_t lStatus = MY_PROJECT_STATUS_OK;
    if(w<1 || h<1 || format==enPixelFormat_None)
    {
        lStatus = MY_PROJECT_ARG_INVALID;
    }

    else
    {
        int bytes_per_pixel;
        switch(format) 
        {
            case 0:
                bytes_per_pixel = 3;   //RGB888
                break;
            case 1:
                bytes_per_pixel = 2;   //RGB565
                break;
            case 2:
                bytes_per_pixel = 4;   //ARGB8888
                break;
            case 3:
                bytes_per_pixel = 1;   //Mono
                break;
            default:
                lStatus = MY_PROJECT_ARG_INVALID;
                return;
        }


        // int bytes_per_pixel = 3; //based on format
        // int stride = 4; //find using w and next number divisible by 4
        unsigned int stride = w*bytes_per_pixel;
        while(stride%4 !=0){
            stride++;
        }

        size_t mem_size = h*stride*bytes_per_pixel*sizeof(unsigned char);
        void* memory = malloc(mem_size);

        if(memory != NULL)
        {
            void *imagePtr = malloc(sizeof(Image_t));
            if(imagePtr == NULL)
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
                image->pixelFormat=format;

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
        //check whether file exists or not
        //FILE* file; // write code to open file

        status_t Status = MY_PROJECT_STATUS_OK;

        FILE *file = fopen(path, "rb");
        if (file == NULL)
        {
            //fclose(file);
            return (NULL);
        }
        
        if(IMAGE_LOADER_COUNT>0)
        {
            for(int i=0; i<IMAGE_LOADER_COUNT; i++)
            {
                const ImageLoader_t* loader = loaders[i];
                if(loader == NULL || loader->identifier == NULL)
                {
                    fprintf(stderr, "Loader is null");
                    continue;
                }
                if(loader->identifier(file)== IMAGE_IS_SUPPORTED)
                {
                    return loader->loader(file);
                }
            }
        }
        else{
            Status = MY_PROJECT_NOT_SUPPORTED;
        }
        //Identify the format of image
        //check whether you support the format or not
        //select the supported loader and load
    return NULL;
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