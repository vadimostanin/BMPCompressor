#include "types_internal.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

std::vector<unsigned char> encodeImage(const RawImageData& image)
{
    std::vector<unsigned char> encodedData;
    
    encodedData.push_back('B');
    encodedData.push_back('A');

    encodedData.push_back((image.width >> 24) & 0xFF);
    encodedData.push_back((image.width >> 16) & 0xFF);
    encodedData.push_back((image.width >> 8) & 0xFF);
    encodedData.push_back(image.width & 0xFF);

    encodedData.push_back((image.height >> 24) & 0xFF);
    encodedData.push_back((image.height >> 16) & 0xFF);
    encodedData.push_back((image.height >> 8) & 0xFF);
    encodedData.push_back(image.height & 0xFF);

    std::vector<bool> emptyRowIndex(image.height, false);

    std::vector<unsigned char> compressedData;

    for(int j = 0; j < image.height; ++j)
    {
        bool isEmptyRow = true;
        for(int i = 0; i < image.width; ++i)
        {
            if(image.data[j * image.width + i] != 0xFF)
            {
                isEmptyRow = false;
                break;
            }
        }
        emptyRowIndex[j] = isEmptyRow;

        if(!isEmptyRow)
        {
            int i = 0;
            while(i < image.width)
            {
                if(i + 3 < image.width &&
                    image.data[j * image.width + i] == 0xFF &&
                    image.data[j * image.width + i + 1] == 0xFF && 
                    image.data[j * image.width + i + 2] == 0xFF && 
                    image.data[j * image.width + i + 3] == 0xFF)
                {
                    compressedData.push_back(0b00);
                    i += 4;
                }
                else if(i + 3 < image.width &&
                   image.data[j * image.width + i] == 0x00 &&
                   image.data[j * image.width + i + 1] == 0x00 &&
                   image.data[j * image.width + i + 2] == 0x00 &&
                   image.data[j * image.width + i + 3] == 0x00)
                {
                    compressedData.push_back(0b10);
                    i += 4;
                }
                else
                {
                    compressedData.push_back(0b11);
                    for (int k = 0; k < 4 && i + k < image.width; ++k)
                    {
                        compressedData.push_back(image.data[j * image.width + i + k]);
                    }
                    i += 4;
                }
            }
        }
    }

    for(bool isEmpty : emptyRowIndex)
    {
        encodedData.push_back(isEmpty ? 1 : 0);
    }

    encodedData.insert(encodedData.end(), compressedData.begin(), compressedData.end());

    return encodedData;
}

RawImageData decodeImage(const std::vector<unsigned char>& encodedData)
{
    int index = 2;

    int width = (encodedData[index] << 24) | (encodedData[index + 1] << 16) | (encodedData[index + 2] << 8) | encodedData[index + 3];
    index += 4;
    int height = (encodedData[index] << 24) | (encodedData[index + 1] << 16) | (encodedData[index + 2] << 8) | encodedData[index + 3];
    index += 4;

    std::vector<bool> emptyRowIndex(height, false);
    for(int j = 0; j < height; ++j)
    {
        emptyRowIndex[j] = encodedData[index++] == 1;
    }

    uint8_t* imageData = (uint8_t*)calloc(1, width * height);
    int dataIndex = index;

    for(int j = 0; j < height; ++j)
    {
        if(emptyRowIndex[j])
        {
            for(int i = 0; i < width; ++i)
            {
                imageData[j * width + i] = 0xFF;
            }
        }
        else
        {
            int i = 0;
            while(i < width)
            {
                if(encodedData[dataIndex] == 0b00)
                {
                    for(int k = 0; k < 4 && i + k < width; ++k)
                    {
                        imageData[j * width + i + k] = 0xFF;
                    }
                    i += 4;
                    dataIndex++;
                }
                else if(encodedData[dataIndex] == 0b10)
                {
                    for(int k = 0; k < 4 && i + k < width; ++k)
                    {
                        imageData[j * width + i + k] = 0x00;
                    }
                    i += 4;
                    dataIndex++;
                }
                else if(encodedData[dataIndex] == 0b11)
                {
                    dataIndex++;
                    for (int k = 0; k < 4 && i + k < width; ++k) {
                        imageData[j * width + i + k] = encodedData[dataIndex++];
                    }
                    i += 4;
                }
            }
        }
    }

    RawImageData image = { width, height, imageData };
    return image;
}
