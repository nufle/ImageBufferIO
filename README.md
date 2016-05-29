# ImageBufferIO
Read images into various data type buffers in **C++**.  
Based on [GDAL](http://www.gdal.org/).  
Image format available :  
	- Any that your GDAL software can handle  
Data type buffers available :  
*- Byte*  
*- Int*  

Tested with **GDAL 1.11.4**.  

## How to use

Simply include **ImageBufferIO.h** in your project.  
Compile it with **GDAL** include directories and library available.  

*Read an image*

```c++

```

*Write a data buffer as an image* :

```c++

```


## Limitations
Your GDAL version may not have all the desired plug-ins you would like to use.   

### Performances

## Dependencies

- [GDAL](https://github.com/OSGeo/gdal) (1.11 recommanded)


