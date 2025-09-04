#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

SpriteRenderer* Renderer;
Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{

}

Game::~Game() {
    delete Renderer;
}

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("default.vert", "default.frag", nullptr, "sprite"); 
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // update uniform variables in shader
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);

    // create renderer
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    ResourceManager::LoadTexture("awesomeface.png", true, "face");


}

void Game::Update(float dt) {

}

void Game::ProcessInput(float dt) {

}

void Game::Render() {
    Renderer->DrawSprite(ResourceManager::GetTexture("face"), 
    glm::vec2(200.0f, 200.0f), 
    glm::vec2(300.0f, 400.0f), 
    90.0f, 
    glm::vec3(0.77f, 0.60f, 0.35f));
}