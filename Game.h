#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameLevel.h"
#include "BallObject.h"


enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
enum Direction {
    RIGHT,
    UP,
    LEFT,
    DOWN
};

// bool: collision, Direction, vec2: difference vector - closest pt
typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game {

    public:

        GameState State;
        unsigned int Width, Height;
        bool Keys[1024];

        std::vector<GameLevel> Levels;
        unsigned int Level;

        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize shaders, textures, levels
        void Init();

        // core game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void HandleCollisions();
        void Render();
        Collision CheckCollision(BallObject& ball, GameObject& otherBox);
        bool CheckCollision(GameObject& one, GameObject& two);
        Direction VectorDirection(glm::vec2 target);

};

#endif