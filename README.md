# ImageBufferIO
Read image files into various data type buffers in **C++**.  
Based on [GDAL](http://www.gdal.org/).  
The main goal of this tool was to quickly generate intermediate image 
logs in the context of scientific image processing. In such a context,
images are often floating point valued.  
Image format available :  
- Any that your GDAL software can handle  

Data type available for the output buffers / images :  
- unsigned char / Byte
- short int / Int16
- unsigned short int / UInt16
- int / Int32
- float / Float32
- double / Float64
- complex unsigned short int / CInt16
- complex int / CInt32
- complex float / CFloat32
- complex double / CFloat64

Tested with **GDAL 1.11.4**, on Ubuntu 16.04, 64 bit 

## How to use

Simply include **ImageBufferIO.h** in your project.  
Compile it with **GDAL** include directories and library available.  

*Read an image into a float-valued buffer*

```c++
    #include "ImageBufferIO.h"
    
    ...
    
    std::string imagePath = "PATH_TO_THE_INPUT_IMAGE";
    // No need to allocate memory
    std::vector<float *> imageBuffer;
    std::vector<int> imageDimensions;

    bool status = ImageBufferIO<float>::image2Buffer(imagePath,
                                                     imageBuffer,
                                                     imageDimensions,
                                                     true);
    ...  
    
    // Do not forget to free the buffer
    ImageBufferIO<unsigned char>::cleanBuffer(imageBuffer);
```
You now can play with the values of the image contained  

*Write an empty 512x512x1 data buffer in an image* :

```c++
    #include "ImageBufferIO.h"

    ... 
    
    std::string imagePath = "PATH_TO_THE_OUTPUT_IMAGE";
    std::string driver = "GTIFF";
    int nbRow = 512, nbCol = 512;
    float * imageBuffer = new float[nbRow*nbCol];
    for (int i = 0; i < nbRow*nbCol; i++)
        imageBuffer[i]=0.0;
    
    std::vector<float*> image;
    image.push_back(imageBuffer)
    
    std::vector<int> dimensions;
    dimensions.push_back(nbCol);
    dimensions.push_back(nbRow);
    dimensions.push_back(1);
    
    bool status = ImageBufferIO<float>::buffer2Image(image,
                                                     dimensions,
                                                     imagePath,
                                                     driver,
                                                     true);
    ... 
    
    // Do not forget to free the buffer
    delete [] imageBuffer;

```


## Limitations
Your GDAL version may not have all the desired plug-ins you would like to use.
This has not been tested on many archirectures. This could result in inconsistencies between the size of the images' pixel datatype and the size of the buffer pixel.

### Performances
The performances of this tool relies fully on the performances of GDAL.

## Dependencies

- [GDAL](https://github.com/OSGeo/gdal) (1.11 recommanded)


