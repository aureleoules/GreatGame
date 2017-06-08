#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <string>

#include "rectangle.h"
#include "globals.h"

class Graphics;

class Sprite {
    public: 
        Sprite();
        Sprite(Graphics &graphics,
                const std::string &filePath,
                int sourceX, int sourceY, 
                int width, int height, 
                float posX, float posY);
        virtual ~Sprite();
        virtual void update();
        void draw(Graphics &graphics, int x, int y);
    
        const Rectangle getBoundingBox() const;
        const sides::Side getCollisionSide(Rectangle &other) const;

    protected:
        SDL_Rect _sourceRect;
        SDL_Texture* _spriteSheets;

        Rectangle _boundingBox;

        float _x, _y;

};

#endif
