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
        this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
    }

Sprite::~Sprite() {

}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect destinationRectangle = {x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE};
    graphics.blitSurface(this->_spriteSheets, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amountRight, amountLeft, amountTop, amountBottom;
	amountRight = this->getBoundingBox().getRight() - other.getLeft();
	amountLeft = other.getRight() - this->getBoundingBox().getLeft();
	amountTop = other.getBottom() - this->getBoundingBox().getTop();
	amountBottom = this->getBoundingBox().getBottom() - other.getTop();

	int values[4] = {abs(amountRight), abs(amountLeft), abs(amountTop), abs(amountBottom)};
	int lowest = values[4];
	for(int i = 0; i < 4; i++) {
		if(values[i] < lowest) {
			lowest = values[i];
		}
	}
	return
			lowest == abs(amountRight) ? sides::RIGHT :
			lowest == abs(amountLeft) ? sides::LEFT :
			lowest == abs(amountTop) ? sides::TOP :
			lowest == abs(amountBottom) ? sides::BOTTOM :
			sides::NONE;

}















