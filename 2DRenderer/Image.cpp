#include "Image.h"

#include "Renderer.h"

#include <fstream>
#include <iostream>

bool Image::Load(const std::string& filename, uint8_t alpha)
{
    std::ifstream stream(filename, std::ios::binary);

    if (stream.fail())
    {
        std::cout << "Error: Cannot open file: " << filename << std::endl;
        return false;
    }

    uint8_t header[54];
    stream.read((char*)header, 54);

    uint16_t id = *((uint16_t*)(header));
    if (id == 'BM')
    {
        std::cout << "Error: This File Is Not A BMP!";
        return false;
    }

    colorBuffer.width = *((int*)(&header[18]));
    colorBuffer.height = *((int*)(&header[22]));

    colorBuffer.pitch = colorBuffer.width * sizeof(color_t);

    colorBuffer.data = new uint8_t[colorBuffer.height * colorBuffer.pitch];

    uint16_t bipp = *((uint16_t*)(&header[28]));
    uint16_t bypp = bipp / 8;

    size_t size = colorBuffer.width * colorBuffer.height * bypp;

    uint8_t* data = new uint8_t[size];

    stream.read((char*)data, size);

    for (int i = 0; i < colorBuffer.width * colorBuffer.height; ++i)
    {
        color_t color;

        int index = i * bypp;
        color.b = data[index];
        color.g = data[index + 1];
        color.r = data[index + 2];
        color.a = alpha;

        ((color_t*)(colorBuffer.data))[i] = color;
    }

    delete[] data;
    stream.close();

    return true;
}

void Image::Flip()
{
    uint8_t* temp = new uint8_t[colorBuffer.pitch];

    for (int i = 0; i < colorBuffer.height / 2; i++)
    {
        uint8_t* line1 = &((colorBuffer.data)[i * colorBuffer.pitch]);
        uint8_t* line2 = &((colorBuffer.data)[((colorBuffer.height - 1) - i) * colorBuffer.pitch]);
        memcpy(temp, line1, colorBuffer.pitch);
        memcpy(line1, line2, colorBuffer.pitch);
        memcpy(line2, temp, colorBuffer.pitch);
    }

    delete[] temp;
}
