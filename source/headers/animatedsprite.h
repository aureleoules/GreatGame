#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <string>
#include <vector>
#include "sprite.h"
#include "globals.h"

class Graphics;

class AnimatedSprite : public Sprite {
    public: 
        AnimatedSprite();
        AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
                        float posX, float posY, float timeToUpdate);

        void playAnimation(std::string animation, bool once = false);

        void update(int elapsedTime);

        void draw(Graphics &graphics, int x, int y);

    protected:
        double _timeToUpdate;
        bool _currentAnimationOnce;
        std::string _currentAnimation;

        void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

        void resetAnimations();

        void stopAnimation();

        void setVisible(bool visible);

        virtual void animationDone(std::string currentAnimation) = 0;

        virtual void setupAnimations() = 0;

    private:
        std::map<std::string, std::vector<SDL_Rect> > _animations;
        std::map<std::string, Vector2> _offsets;

        int _frameIndex;
        double _timeElapsed;
        bool _visible;
};


#endif