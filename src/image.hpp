  
#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <vector>

#include "vec3.hpp"
#include "color.hpp"
#include "bitmap.h"

struct Image
{
    int size_x;
    int size_y;

    std::vector<std::vector<vec3>> buffer;

    void reallocate()
    {
        buffer.clear();
        buffer.resize(size_x);
        for (int i = 0; i < size_x; i++)
            buffer[i].resize(size_y);
    }

    Image(int sx = 0, int sy = 0) : size_x(sx), size_y(sy)
    {
        this->reallocate();
    }

    void Set(int x, int y, vec3 color)
    {
        assert(0 <= x && x < size_x);
        assert(0 <= y && y < size_y);
        buffer[x][y] = color;
    }

    vec3 Get(int x, int y)
    {
        assert(0 <= x && x < size_x);
        assert(0 <= y && y < size_y);
        return buffer[x][y];
    }

    vec3 Add(int x, int y, vec3 color)
    {
        // std::cerr << "Add " << x << " " << y << " " << color.x << " " << color.y << " " << color.z << std::endl;

        auto origin = this->Get(x, y);
        origin = origin + color;
        this->Set(x, y, origin);
        return origin;
    }

};

Image createImageFromBMP(std::string filePath)
{
	Bitmap bitmap(filePath);
	if (!bitmap.isLoaded())
	{
		std::cerr << "BMP Load error." << std::endl;
		exit(0);
	}

	BITMAP_INFO_HEADER &infoHeader = bitmap.getInfoHeader();

	int img_width = infoHeader.width;
	int img_height = infoHeader.height;

	Image image(img_width, img_height);
	uint8_t *pixels = bitmap.getPixels();

	for (int y = 0; y < img_height; y++)
	{
		for (int x = 0; x < img_width; x++)
		{
			int i = x * 3 + (img_height - y - 1) * img_width * 3;
			uint8_t r = pixels[i + 2];
			uint8_t g = pixels[i + 1];
			uint8_t b = pixels[i + 0];
			image.Set(x, y, colorUint8ToFloat(r, g, b));
		}
	}

	return image;
}

#endif