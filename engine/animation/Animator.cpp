#include "Animator.h"

Animator::Animator(
    std::vector<Animation *>& animations
) : animations(std::move(animations)) {
}

void Animator::update(const float deltaTime) const {
    if (currentAnimation) {
        currentAnimation->update(deltaTime);
    }
}

void Animator::draw() const {
    if (currentAnimation) {
        currentAnimation->draw();
    }
}

void Animator::setCurrentAnimation(const int index) {
    if (index >= 0 && index < animations.size()) {
        currentAnimation = animations[index];
    } else {
        currentAnimation = nullptr;
    }
}

void Animator::pause(const bool pause) {
    isPaused = pause;
}
