#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Blob.h"
#include "ShaderManager.h"

class BlobSimulation {
public:
    BlobSimulation(unsigned int width, unsigned int height);
    
    void run();
    
private:
    sf::RenderWindow window;
    ShaderManager shaderManager;
    std::vector<Blob> blobs;
    
    std::mt19937 rng;
    std::uniform_real_distribution<float> posDist;
    std::uniform_real_distribution<float> radiusDist;
    std::uniform_int_distribution<int> colorDist;
    
    sf::Clock clock;
    const float targetFrameTime = 1.0f / 60.0f; // 60 Hz
    
    void initialize();
    void handleEvents();
    void update(float dt);
    void render();
    void renderBlob(const Blob& blob);
    void renderMetaballs();
    void checkMerging();
    void applyForces();
    
    sf::Color generateRandomColor();
};