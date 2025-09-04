#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {

    public:

        GameState State;
        unsigned int Width, Height;
        bool Keys[1024];
        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize shaders, textures, levels
        void Init();

        // core game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();

};

#endif