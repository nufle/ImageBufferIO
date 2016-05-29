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
    std::string imagePath = "PATH_TO_THE_IMAGE";
    std::vector<float *> imageBuffer;
    std::vector<int> imageDimensions;

    bool status = ImageBufferIO<float>::image2Buffer(imagePath,
                                                     imageBuffer,
                                                     imageDimensions,
                                                     true);
```

*Write a data buffer as an image* :

```c++

```


## Limitations
Your GDAL version may not have all the desired plug-ins you would like to use.

### Performances

## Dependencies

- [GDAL](https://github.com/OSGeo/gdal) (1.11 recommanded)


