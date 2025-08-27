#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <GLFW/glfw3.h>
#include <./glad/glad.h>

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

        // initialize shaders, textures, levels
        void Init();

        // core game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();

};

#endif