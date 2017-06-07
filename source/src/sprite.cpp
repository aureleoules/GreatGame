#include "graphics.h"
#include "globals.h"
#include "sprite.h"
#include <stdio.h>
Sprite::Sprite() {

}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
                float posX, float posY):
                        _x(posX),
                        _y(posY)
    {
        this->_sourceRect.x = sourceX;
        this->_sourceRect.y = sourceY;
        this->_sourceRect.w = width;
        this->_sourceRect.h = height;

        this->_spriteSheets = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
        if(this->_spriteSheets == NULL) {
        	printf("Couldn't load image.\n");
        }
    }

Sprite::~Sprite() {

}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect destinationRectangle = {x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE};
    graphics.blitSurface(this->_spriteSheets, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {

}
