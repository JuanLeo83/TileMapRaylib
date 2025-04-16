#include "Player.h"

#include <vector>

Player::Player() {
    texture = LoadTexture("../assets/Warrior_Blue.png");
    std::vector animations = {
        new Animation(texture, 0, 6, 0.1f, 192, 192),
        new Animation(texture, 1, 6, 0.1f, 192, 192),
        new Animation(texture, 2, 6, 0.1f, 192, 192),
    };
    animator = new Animator(animations);
    animator->setCurrentAnimation(2);
}

void Player::update(const float deltaTime) {
    animator->update(deltaTime);

    if (IsKeyDown(KEY_I)) {
        animator->setCurrentAnimation(Idle);
    } else if (IsKeyDown(KEY_O)) {
        animator->setCurrentAnimation(Run);
    } else if (IsKeyDown(KEY_P)) {
        animator->setCurrentAnimation(Attack);
    }
}

void Player::draw() const {
    animator->draw();
}
