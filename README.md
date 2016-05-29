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
Since it's a pythonic tool, it is not focused on performances, even though Numpy and OpenCV both have low level core... So if you do not want to spend all night waiting for your Collada file, I advise you not to use input files bigger than 1500px by 1500px. (meaning you may have to crop your SRTM tiles...) 

### Algorithmic


## Dependencies

- [GDAL](https://github.com/OSGeo/gdal) (1.11 recommanded)


