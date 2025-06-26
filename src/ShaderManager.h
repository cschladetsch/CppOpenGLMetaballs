#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class ShaderManager {
public:
    ShaderManager();
    
    bool loadShaders();
    
    sf::Shader* getBlobShader() { return blobShader.get(); }
    sf::Shader* getMetaballShader() { return metaballShader.get(); }
    
private:
    std::unique_ptr<sf::Shader> blobShader;
    std::unique_ptr<sf::Shader> metaballShader;
    
    bool loadShaderFromFile(sf::Shader& shader, const std::string& vertexPath, const std::string& fragmentPath);
};