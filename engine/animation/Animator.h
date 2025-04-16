#pragma once
#include "Animation.h"
#include <vector>

class Animator {
    Animation *currentAnimation = nullptr;
    std::vector<Animation*> animations;
    bool isPaused = false;

public:
    explicit Animator(std::vector<Animation *>& animations);

    void update(float deltaTime) const;

    void draw() const;

    void setCurrentAnimation(int index);

    void pause(bool pause);
};

