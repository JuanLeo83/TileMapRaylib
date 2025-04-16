#pragma once
#include <raylib.h>

class Animation {
    Texture &texture;

    int row;
    int frames;
    int frameWidth;
    int frameHeight;
    float frameDuration;
    float frameCounter = 0;
    int currentFrame = 0;

    Vector2 position = {0, 0};

public:
    Animation(Texture &texture, int row, int frames, float frameDuration, int frameWidth, int frameHeight);

    void update(float deltaTime);

    void draw() const;
};
