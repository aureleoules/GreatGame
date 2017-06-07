#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Player : public AnimatedSprite {
    public:
        Player();
        Player(Graphics &graphics , float x, float y);
        void draw(Graphics &graphics);
        void update(float elapsedTime);

        void moveLeft();
        void moveRight();
        void stopMoving();

        virtual void animationDone(std::string currentAnimation);
        virtual void setupAnimations();

    private:
        float _dx, _dy;

        Direction _facing;      
};

#endif