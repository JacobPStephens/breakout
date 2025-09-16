#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "BallObject.h"

#include <iostream>
#include <tuple>

SpriteRenderer* Renderer;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{

}

Game::~Game() {
    delete Renderer;

}


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
GameObject *Player;

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
BallObject *Ball;

void Game::Init() {
    // load textures
    ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("textures/background.jpg", false, "background");
    //ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/ball.png", true, "ball");
    ResourceManager::LoadTexture("textures/block.png", true, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", true, "block_solid");



    glm::vec2 playerPos = glm::vec2(
        this->Width/2.0f - PLAYER_SIZE.x/2.0f,
        this->Height - PLAYER_SIZE.y
    );

    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
    // load levels
    //GameLevel test; test.Load("levels/test.level", this->Width, this->Height / 2);

    GameLevel one; one.Load("levels/standard.level", this->Width, this->Height / 2);
    // GameLevel two; two.Load("levels/gaps.level", this->Width, this->Height / 2);
    // GameLevel three; three.Load("levels/spaceInvader.level", this->Width, this->Height / 2);
    // GameLevel four; four.Load("levels/bounce.level", this->Width, this->Height / 2);

    //Levels.push_back(test);
    Levels.push_back(one);
    // Levels.push_back(two);
    // Levels.push_back(three);
    // Levels.push_back(four);
    this->Level = 0;

    // load shaders
    ResourceManager::LoadShader("shaders/default.vert", "shaders/default.frag", nullptr, "sprite"); 
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // update uniform variables in shader
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);

    // create renderer
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");


}

void Game::Update(float dt) {
    Ball->Move(dt, this->Width);
    HandleCollisions();
}

//Direction VectorDirection(glm::vec2 closest);
void Game::HandleCollisions() {

    Collision result = CheckCollision(*Ball, *Player);

    // ball-player collision
    if (std::get<0>(result) && !Ball->Stuck) {
        //std::cout << "Ball-player collision" << std::endl;
        float playerCenterX = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - playerCenterX;
        float percentage = distance / (Player->Size.x / 2.0f);
        
        float strength = 2.0f;
        glm::vec2 previousVelocity = Ball->Velocity;
        
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(previousVelocity);

        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
        return;
    }

    // ball-box collision
    for (GameObject& box : this->Levels[this->Level].Bricks) {

        if (box.Destroyed) continue;
        
        Collision collision = CheckCollision(*Ball, box);

        if (!std::get<0>(collision)) continue;

        Direction dir = std::get<1>(collision);
        glm::vec2 vecToClosestPoint = std::get<2>(collision);

        if (dir == LEFT || dir == RIGHT) {
            Ball->Velocity.x = -Ball->Velocity.x;
            float infiltration = Ball->Radius - std::abs(vecToClosestPoint.x);
            if (dir == LEFT) 
                Ball->Position.x += infiltration;
            else
                Ball->Position.x -= infiltration;
        }
        else if (dir == UP || dir == DOWN) {
            Ball->Velocity.y = -Ball->Velocity.y;
            float infiltration = Ball->Radius - std::abs(vecToClosestPoint.y);
            if (dir == UP)
                Ball->Position.y -= infiltration; // I thought the signs would be flipped
            else
                Ball->Position.y += infiltration;
        }

        if (!box.IsSolid)
            box.Destroyed = true;

    }
}

void Game::ProcessInput(float dt) {
    if (this->State != GAME_ACTIVE) {
        return;
    }

    float velocity = PLAYER_VELOCITY * dt;

    if (this->Keys[GLFW_KEY_A]) {
        if (Player->Position.x > 0.0f) {
            Player->Position.x -= velocity;
            if (Ball->Stuck) {
                Ball->Position.x -= velocity;
            }
        }
    }

    if (this->Keys[GLFW_KEY_D]) {
        if (Player->Position.x < this->Width - Player->Size.x) {
            Player->Position.x += velocity;
            if (Ball->Stuck) {
                Ball->Position.x += velocity;
            }
        }
    }

    if (this->Keys[GLFW_KEY_SPACE]) {
        Ball->Stuck = false;
    }
}

void Game::Render() {

    if (this->State != GAME_ACTIVE) {
        return;
    }
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
    glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
    );

    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
    Ball->Draw(*Renderer);

    // Renderer->DrawSprite(ResourceManager::GetTexture("face"), 
    // glm::vec2(200.0f, 200.0f), 
    // glm::vec2(300.0f, 400.0f), 
    // 90.0f, 
    // glm::vec3(0.77f, 0.60f, 0.35f));
}

// Circle and Rectangle
Collision Game::CheckCollision(BallObject& ball, GameObject& otherBox) {
    glm::vec2 center = glm::vec2(ball.Position + ball.Radius);
    glm::vec2 halfSizes = glm::vec2(otherBox.Size.x / 2.0f, otherBox.Size.y / 2.0f);
    glm::vec2 boxCenter = glm::vec2(otherBox.Position.x + halfSizes.x, otherBox.Position.y + halfSizes.y);

    glm::vec2 centerDist = center - boxCenter;
    glm::vec2 clamped = glm::clamp(centerDist, -halfSizes, halfSizes);
    glm::vec2 closest = boxCenter + clamped;

    glm::vec2 difference = closest - center;
    if (glm::length(difference) < ball.Radius) {
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else {
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
    }

}
// Rectangle and Rectangle
bool Game::CheckCollision(GameObject& one, GameObject& two) {
    return (((
        (one.Position.x + one.Size.x >= two.Position.x) &&
        (two.Position.x + two.Size.x >= one.Position.x) &&
        (one.Position.y + one.Size.y >= two.Position.y) &&
        (two.Position.y + two.Size.y >= one.Position.y))));
}

// This function calculates dot product of every direction vector to
// determine which direction it is closest to
Direction Game::VectorDirection(glm::vec2 target) {

    glm::vec2 compass[] = {
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // up
        glm::vec2(-1.0f, 0.0f), // left
        glm::vec2(0.0f, 1.0f),  // down
    };
    float maxDotProduct = 0.0f;
    unsigned int maxIdx = -1;

    for (int i = 0; i < 4; i++) {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if (dotProduct > maxDotProduct) {
            maxDotProduct = dotProduct;
            maxIdx = i;
        }
    }
    return (Direction)maxIdx;
}
