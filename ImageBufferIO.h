/**
 * @file : ImageBufferIO.h
 * @author : Arnaud Mallen
 * @date : 29/05/2016
 * @version : 1.0
 *
 * @brief Image buffer reading and writing using GDAL
 *
 * @section DESCRIPTION
 *
 *
 *
 */


#ifndef IMAGE2BUFFER_IMAGEBUFFERIO_H
#define IMAGE2BUFFER_IMAGEBUFFERIO_H

#include <typeinfo>
#include <sstream>
#include <complex>
#include <vector>
#include <string.h>


#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_string.h"


/*
 * ImageBufferIO grants an easy access to image data using GDAL.
 * This class allows to :
 *  - Read a multi-band image into a vector of buffers
 *  - Write a vector of buffers into a multi-band image
 *
 * It works with all image formats supported by the GDAL you compile it with
 * It is templated over the datatype of the buffer. All conversions are taken care of by GDAL internally.
 *
 * Within a buffer, data is stored the usual way, i-e you can go through the data this way :
 *
 * for (int row = 0; row < nbRow; row++)
 *  for (int col = 0; col < nbcol; col++)
 *      buffer[row*nbRow+col] ....
 *
 * !! When reading and writing an image, the user is in charge of choosing the adequate
 * datatype to instantiate the class with. Reading and writing images with a low-definition datatype
 * can result in data loss.
 * The user should also be careful with the image format. Some image formats do not handle all datatypes
 * (for instance, JPEG does not accept Float32). !!
 */
template<typename T>
class ImageBufferIO{
public:

    /*
     * Loads an image into a vector of T * buffers. Data conversion is carried out by GDAL.
     * @param imagepath : path of the image to be loaded, can be relative of absolute, with extension
     * @param buffer : vector of data buffer, 1 element <-> 1 band
     * @param dimensions : 3 element vector containing [ nbCol, nbRow, nbBands ]
     * @param initGDAL : optional boolean allowing the user to init GDAL externally for better performances
     * @param driver : optional string allowing the user to specify which driver to use to open the image
     */
    static bool image2Buffer(const std::string & imageFilename,
                             std::vector<T*> & buffer,
                             std::vector<int> & dimensions,
                             bool initGDAL = false);

    /*
     * Writes an image from a vector of T * buffers.
     * @param buffer : vector of data buffer, 1 element <-> 1 band
     * @param dimensions : 3 element vector containing [ nbCol, nbRow, nbBands ]
     * @param imagepath : the output image filename
     * @param driver : optional string allowing the user to specify which driver to use to write the image
     * @param initGDAL : optional boolean allowing the user to init GDAL externally for better performances
     */
    static bool buffer2Image(std::vector<T*> & buffer,
                             std::vector<int> & dimensions,
                             const std::string imageFilename,
                             const std::string driver="GTiff",
                             bool initGDAL = false);

    /*
     * Cleans a buffer with the appropriate GDAL method
     * @param buffer : a vector of image buffer created by "image2Buffer"
     */
    static bool cleanBuffer(std::vector<T*> & buffer);

private:
    // Returns the GDALDataType associated to the primitive type
    static GDALDataType toGDALDataType();
};

template<typename T>
GDALDataType ImageBufferIO<T>::toGDALDataType()
{
    if (typeid(T) == typeid(unsigned char))
    {
        return GDT_Byte;
    }
    if (typeid(T) == typeid(short int))
    {
        return GDT_Int16;
    }
    if (typeid(T) == typeid(unsigned short int))
    {
        return GDT_UInt16;
    }
    if (typeid(T) == typeid(int))
    {
        return GDT_Int32;
    }
    if (typeid(T) == typeid(float))
    {
        return GDT_Float32;
    }
    if (typeid(T) == typeid(double))
    {
        return GDT_Float64;
    }
    if (typeid(T) == typeid(std::complex<unsigned short int>))
    {
        return GDT_CInt16;
    }
    if (typeid(T) == typeid(std::complex<int>))
    {
        return GDT_CInt32;
    }
    if (typeid(T) == typeid(std::complex<float>))
    {
        return GDT_CFloat32;
    }
    if (typeid(T) == typeid(std::complex<double>))
    {
        return GDT_CFloat64;
    }
    return GDT_Unknown;
}

template<typename T>
bool ImageBufferIO<T>::cleanBuffer(std::vector<T*> & buffer)
{
    for (int i = 0; i < buffer.size(); i++)
    {
        if (buffer[i])
            CPLFree(buffer[i]);
        buffer[i] = NULL;
    }
    // Nothing to do for now
    return true;
}

template<typename T>
bool ImageBufferIO<T>::buffer2Image(std::vector<T*> & buffer,
                                    std::vector<int> & dimensions,
                                    const std::string imagepath,
                                    const std::string driver,
                                    bool initGDAL)
{
    // 1 - Basic dimensions checking
    if (dimensions.size() < 3 || dimensions[0] < 1 || dimensions[1] < 1 || dimensions[2] < 1)
        return false;

    // 2 - Image creation and opening
    if (initGDAL) {
        GDALAllRegister();
    }

    char **papszOptions = NULL;
    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(driver.c_str());
    GDALDataset *image = poDriver->Create(imagepath.c_str(), dimensions[0], dimensions[1],
                                          dimensions[2], toGDALDataType(), papszOptions);

    if (!image)
    {
        return false;
    }

    // 3 - Image writing
    for (int band = 0; band < dimensions[2]; band++)
    {
        GDALRasterBand *poBand = image->GetRasterBand(band+1);
        // Row-by-row image writing
        for (int row = 0; row < dimensions[1]; row++)
        {
            poBand->RasterIO( GF_Write, 0, row, dimensions[0], 1,
                              &(buffer[band][row*dimensions[0]]), dimensions[0], 1, toGDALDataType(),
                              0, 0);
        }
    }

    // 4 - Closing and return
    GDALClose(image);
    return true;
}


template<typename T>
bool ImageBufferIO<T>::image2Buffer(const std::string & imagepath,
                                    std::vector<T*> & buffer,
                                    std::vector<int> & dimensions,
                                    bool initGDAL)
{
    // 1 - Image opening and reading
    if (initGDAL) {
        GDALAllRegister();
    }

    GDALDataset *poDataset;
    GDALRasterBand *poBand;
    poDataset = (GDALDataset *) GDALOpen(imagepath.c_str(), GA_ReadOnly);
    if (poDataset == NULL) {
        return false;
    }

    // 2 - Dimensions initialization
    buffer.clear();
    dimensions.clear();
    dimensions.push_back(poDataset->GetRasterXSize());
    dimensions.push_back(poDataset->GetRasterYSize());
    dimensions.push_back(poDataset->GetRasterCount());
    for (int i = 0; i < dimensions[2]; i++)
    {
        buffer.push_back(NULL);
    }

    // 3 - Data Loading band by band / row by row
    // Temporary Row Buffer, allocated once
    T * rowTmp = (T*) CPLMalloc(sizeof(T)*dimensions[0]);

    for (int band = 1; band < dimensions[2]+1; band++)
    {
        // Current band
        poBand = poDataset->GetRasterBand(band);
        // Memory allocation for current band buffer
        buffer[band-1] = (T*) CPLMalloc(sizeof(T)*dimensions[0]*dimensions[1]);

        // Line by line copy of the input
        for (int row = 0; row < dimensions[1]; row++)
        {
            // GDAL automatically converts input data to the appropriate data type
            poBand->RasterIO( GF_Read, 0, row, dimensions[0], 1,
                              rowTmp, dimensions[0], 1, toGDALDataType(),
                              0, 0);
            memcpy( &(buffer[band-1][row*dimensions[0]]), rowTmp, dimensions[0] * sizeof(T));
        }
    }

    // 4 - Closing and return
    CPLFree(rowTmp);
    GDALClose(poDataset);
    return true;
}

#endif //IMAGE2BUFFER_IMAGEBUFFERIO_H
