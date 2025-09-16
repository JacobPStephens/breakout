#include "GameLevel.h"
#include <fstream>
#include <sstream>
#include <iostream>


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {

    // clear bricks vector
    this->Bricks.clear();

    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tiles;

    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) {
                row.push_back(tileCode);
            }
            tiles.push_back(row);
        }
        if (tiles.size() > 0) {
            this->init(tiles, levelWidth, levelHeight);            
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer) {
    for (GameObject &tile : this->Bricks) {
        if (!tile.Destroyed) {
            tile.Draw(renderer);
        }
    }
}
bool GameLevel::IsCompleted() {
    for (GameObject &tile : this->Bricks) {
        if (!tile.IsSolid && !tile.Destroyed) {
            return false;
        }
    }
    return false;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tiles, unsigned int levelWidth, unsigned int levelHeight) {

    unsigned int height = tiles.size();
    unsigned int width = tiles[0].size(); // function is only called if tiles.size() >= 1. So we can index at 0
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    std::map<unsigned int, glm::vec3> numToCol = {
        {1, glm::vec3(1.0f, 1.0f, 1.0f)},
        {2, glm::normalize(glm::vec3(100.0f, 166.0f, 189.0f))},
        {3, glm::normalize(glm::vec3(144.0f, 168.0f, 195.0f))},
        {4, glm::normalize(glm::vec3(173.0f, 167.0f, 201.0f))},
        {5, glm::normalize(glm::vec3(215.0f, 185.0f, 213.0f))}
    };

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            if (tiles[y][x] == 0) 
                continue;

            glm::vec2 pos = glm::vec2(unit_width * x, unit_height * y);
            glm::vec2 size = glm::vec2(unit_width, unit_height);
            glm::vec3 color = numToCol[tiles[y][x]];
            std::string tex;
            bool solid;
            if (tiles[y][x] == 1) {
                tex = "block_solid";
                solid = true;
            }
            else if (tiles[y][x] > 1) {
                tex = "block";
                solid = false;
            }

            GameObject obj = GameObject(pos, size, ResourceManager::GetTexture(tex), color, glm::vec2(0.0f), solid);
            this->Bricks.push_back(obj);

        }
    }
}