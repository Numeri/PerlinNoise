#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <math.h>
#include <functional>
#include <math.h>
#include "Perlin.h"
#include "map.h"

const int xsize = 300, ysize = 300;

float map1[ysize][xsize];
perlinNoise noise;

Rectangle noiseBounds;

sf::Uint8* pixels = new sf::Uint8[ysize*xsize*4];

void initialize();
void generateNoise(Rectangle bounds);

int main()
{
    sf::RenderWindow window(sf::VideoMode(xsize, ysize), "SFML works!");
    sf::Image pxlImage;
    //pxlImage.create(xsize, ysize, sf::Color(0, 3, 0));
    sf::Sprite pxlSprite;
    sf::Texture pxlTexture;

    initialize();

    noise.setSeed(time(NULL));
    generateNoise(noiseBounds);
    pxlImage.create(xsize, ysize, pixels);
    pxlTexture.loadFromImage(pxlImage);
    pxlSprite.setTexture(pxlTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                window.clear();
                window.display();
                for (int i=0; i<ysize; i++)
                {
                    for (int j=0; j<xsize; j++)
                    {
                        map1[i][j] = 0.0;
                    }
                }
                noise.setSeed(rand());
                generateNoise(noiseBounds);
                pxlImage.create(xsize,ysize,pixels);

                pxlTexture.loadFromImage(pxlImage);
                pxlSprite.setTexture(pxlTexture);
                break;

            default:
                break;
            }
        }

        window.clear();
        window.draw(pxlSprite);
        window.display();
    }

    delete pixels;
    return 0;
}

void initialize()
{
    noiseBounds.x1 = 0.0;
    noiseBounds.y1 = 0.0;
    noiseBounds.x2 = 200.0;
    noiseBounds.y2 = 200.0;
}

void generateNoise(Rectangle bounds)
{
    noise.generate();
    int contourLineSpacing = 20;
    float waterLevel = 0.4;
    auto contourAndWater = std::function<float (float)>
                           ([contourLineSpacing, waterLevel](float x)
    {
        return (x>waterLevel ? (float)((int)(x*100)-(int)(x*100)%contourLineSpacing)/100.0 : 0);
    });
    auto sineWave = std::function<float (float, float, float)>
                    ([](float v, float x, float y)
    {
        return (0.5+sin(x*2*3.14159*8+v*5*3.14159)/2.0);
    });

    auto continantal = std::function<float (float, float, float)>
                    ([](float v, float x, float y)
    {
        float middleBiased = (3*v - 8*x*x + 8*x - 8*y*y + 8*y)/7.0;
        return (middleBiased>0.5 ? (float)((int)(middleBiased*100)-(int)(middleBiased*100)%5)/100.0 : 0);
    });
    noise.calcBrownianFractal(&map1[0][0], xsize, ysize, 1, (int)pow(2,9), 1.5, 2.0, continantal);

    for (int i=0; i<ysize; i++)
    {
        for (int j=0; j<xsize; j++)
        {
            int loc = (i*xsize+j)*4;
            pixels[loc] = (int)(map1[i][j]*255);
            pixels[loc+1] = (int)(map1[i][j]*78);
            pixels[loc+2] = (int)(map1[i][j]*50);
            pixels[loc+3] = 255;
        }
    }
}
