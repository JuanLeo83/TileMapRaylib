#pragma once
#include <raylib.h>
#include "animation/Animator.h"

class Player {
    Texture2D texture{};
    Animator *animator;

public:
    Player();

    void update(float deltaTime);

    void draw() const;
};

enum PlayerAnimations {
    Idle = 0,
    Run = 1,
    Attack = 2,
};
