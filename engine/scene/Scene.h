#pragma once

class Scene {
public:
    virtual ~Scene() = default;

    virtual void update(float deltaTime) = 0;

    virtual void draw() const = 0;
};