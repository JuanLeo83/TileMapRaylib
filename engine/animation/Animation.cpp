#include "Animation.h"

Animation::Animation(Texture &texture, const int row, const int frames, const float frameDuration, const int frameWidth,
                     const int frameHeight) : texture(texture), row(row), frames(frames), frameWidth(frameWidth),
                                              frameHeight(frameHeight), frameDuration(frameDuration) {
}

void Animation::update(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= frameDuration) {
        frameCounter = 0;
        currentFrame++;
        if (currentFrame == frames) currentFrame = 0;
    }
}

void Animation::draw() const {
    DrawTextureRec(
        texture,
        (Rectangle){
            static_cast<float>(frameWidth * currentFrame), static_cast<float>(frameHeight * row),
            static_cast<float>(frameWidth), static_cast<float>(frameHeight)
        },
        position,
        WHITE
    );
}
