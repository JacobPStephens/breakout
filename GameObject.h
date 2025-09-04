#ifndef CLASS_GAMEOBJECT_H
#define CLASS_GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Texture2D.h>
#include <SpriteRenderer.h>

class GameObject {
    public:
        glm::vec2 Position, Size, Velocity;
        glm::vec3 Color;
        float Rotation;
        bool IsSolid;
        bool Destroyed;

        // render state
        Texture2D Sprite;

        // constructors
        GameObject();
        GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f), bool solid = false);

        // draw sprite (confused about virtual)
        virtual void Draw(SpriteRenderer &renderer);
};

#endif