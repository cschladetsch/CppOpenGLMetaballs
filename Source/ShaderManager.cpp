#include "ShaderManager.h"
#include <iostream>

ShaderManager::ShaderManager() {
    blobShader = std::make_unique<sf::Shader>();
    metaballShader = std::make_unique<sf::Shader>();
}

bool ShaderManager::loadShaders() {
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders are not available on this system!" << std::endl;
        return false;
    }
    
    if (!loadShaderFromFile(*blobShader, "shaders/blob.vert", "shaders/blob.frag")) {
        std::cerr << "Failed to load blob shader!" << std::endl;
        return false;
    }
    
    if (!loadShaderFromFile(*metaballShader, "shaders/metaball.vert", "shaders/metaball.frag")) {
        std::cerr << "Failed to load metaball shader!" << std::endl;
        return false;
    }
    
    return true;
}

bool ShaderManager::loadShaderFromFile(sf::Shader& shader, const std::string& vertexPath, const std::string& fragmentPath) {
    return shader.loadFromFile(vertexPath, fragmentPath);
}