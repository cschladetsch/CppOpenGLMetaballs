#include "BlobSimulation.h"
#include <iostream>
#include <algorithm>

BlobSimulation::BlobSimulation(unsigned int width, unsigned int height)
    : window(sf::VideoMode(width, height), "Blob Simulation", sf::Style::Titlebar | sf::Style::Close)
    , rng(std::random_device{}())
    , posDist(0.0f, 1.0f)
    , radiusDist(10.0f, 40.0f)
    , colorDist(50, 255) {
    
    window.setFramerateLimit(60);
}

void BlobSimulation::run() {
    if (!shaderManager.loadShaders()) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return;
    }
    
    initialize();
    
    while (window.isOpen()) {
        handleEvents();
        
        float frameTime = clock.restart().asSeconds();
        frameTime = std::min(frameTime, 0.1f); // Cap frame time to prevent spiral of death
        
        update(frameTime);
        render();
    }
}

void BlobSimulation::initialize() {
    const int numBlobs = 30; // Start with fewer blobs
    auto windowSize = window.getSize();
    
    
    // Create a grid to better distribute initial positions
    int gridSize = static_cast<int>(std::sqrt(numBlobs)) + 2; // More spacing
    float cellWidth = windowSize.x / gridSize;
    float cellHeight = windowSize.y / gridSize;
    
    for (int i = 0; i < numBlobs; ++i) {
        // Place blobs in grid cells with some randomness
        int gridX = i % gridSize;
        int gridY = i / gridSize;
        
        float x = (gridX + 0.2f + posDist(rng) * 0.6f) * cellWidth;
        float y = (gridY + 0.2f + posDist(rng) * 0.6f) * cellHeight;
        float radius = radiusDist(rng);
        sf::Color color = generateRandomColor();
        
        blobs.emplace_back(x, y, radius, color);
        
        // Set initial velocity by manipulating previous position
        float angle = posDist(rng) * 2 * M_PI;
        float speed = 30.0f + posDist(rng) * 70.0f; // Moderate speeds for visible movement
        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);
        
        // For Verlet integration, we set velocity by adjusting previous position
        blobs.back().setPosition(sf::Vector2f(x, y));
        // This creates the initial velocity
        sf::Vector2f prevPos = sf::Vector2f(x, y) - velocity * 0.016f; // Assume 60Hz
        blobs.back().setPosition(sf::Vector2f(x, y)); // Reset current position
        blobs.back().setPreviousPosition(prevPos);
    }
    
}

void BlobSimulation::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                float x = posDist(rng) * window.getSize().x;
                float y = posDist(rng) * window.getSize().y;
                float radius = radiusDist(rng);
                sf::Color color = generateRandomColor();
                blobs.emplace_back(x, y, radius, color);
                
                // Set initial velocity
                float angle = posDist(rng) * 2 * M_PI;
                float speed = 30.0f + posDist(rng) * 70.0f; // Match the initialization speeds
                sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);
                sf::Vector2f prevPos = sf::Vector2f(x, y) - velocity * 0.016f;
                blobs.back().setPreviousPosition(prevPos);
            } else if (event.key.code == sf::Keyboard::R) {
                blobs.clear();
                initialize();
            }
        }
    }
}

void BlobSimulation::update(float dt) {
    static int frameCount = 0;
    
    applyForces();
    
    for (auto& blob : blobs) {
        blob.update(dt, window.getSize());
    }
    
    
    auto windowSize = window.getSize();
    
    for (size_t i = 0; i < blobs.size(); ++i) {
        for (size_t j = i + 1; j < blobs.size(); ++j) {
            // Check if blobs are close considering wrap-around
            sf::Vector2f diff = blobs[j].getPosition() - blobs[i].getPosition();
            
            // Only handle collision if they're actually close (considering wrap-around)
            bool closeX = std::abs(diff.x) < 150.0f || std::abs(diff.x) > windowSize.x - 150.0f;
            bool closeY = std::abs(diff.y) < 150.0f || std::abs(diff.y) > windowSize.y - 150.0f;
            
            if (closeX && closeY) {
                blobs[i].handleCollision(blobs[j]);
            }
        }
    }
    
    checkMerging();
}

void BlobSimulation::applyForces() {
    const float gravityStrength = 2000.0f; // Strong gravity for clear attraction
    const float minDistance = 20.0f; // Minimum distance to prevent extreme forces
    auto windowSize = window.getSize();
    
    for (size_t i = 0; i < blobs.size(); ++i) {
        for (size_t j = i + 1; j < blobs.size(); ++j) {
            sf::Vector2f diff = blobs[j].getPosition() - blobs[i].getPosition();
            
            // Check for wrap-around distances
            if (std::abs(diff.x) > windowSize.x / 2) {
                diff.x = diff.x > 0 ? diff.x - windowSize.x : diff.x + windowSize.x;
            }
            if (std::abs(diff.y) > windowSize.y / 2) {
                diff.y = diff.y > 0 ? diff.y - windowSize.y : diff.y + windowSize.y;
            }
            
            float distSq = diff.x * diff.x + diff.y * diff.y;
            
            if (distSq < minDistance * minDistance) {
                distSq = minDistance * minDistance;
            }
            
            float dist = std::sqrt(distSq);
            sf::Vector2f direction = diff / dist;
            
            float forceMagnitude = gravityStrength * blobs[i].getMass() * blobs[j].getMass() / distSq;
            forceMagnitude = std::min(forceMagnitude, 1000.0f); // Higher force cap for more movement
            
            sf::Vector2f force = direction * forceMagnitude;
            blobs[i].applyForce(force);
            blobs[j].applyForce(-force);
        }
    }
}

void BlobSimulation::checkMerging() {
    std::vector<Blob> newBlobs;
    std::vector<bool> merged(blobs.size(), false);
    
    for (size_t i = 0; i < blobs.size(); ++i) {
        if (merged[i]) continue;
        
        bool foundMerge = false;
        for (size_t j = i + 1; j < blobs.size(); ++j) {
            if (merged[j]) continue;
            
            if (blobs[i].shouldMerge(blobs[j])) {
                newBlobs.push_back(Blob::merge(blobs[i], blobs[j]));
                merged[i] = true;
                merged[j] = true;
                foundMerge = true;
                break;
            }
        }
        
        if (!foundMerge) {
            newBlobs.push_back(blobs[i]);
        }
    }
    
    if (newBlobs.size() < blobs.size()) {
        std::cout << "Merged: " << blobs.size() << " -> " << newBlobs.size() << " blobs" << std::endl;
    }
    
    blobs = std::move(newBlobs);
}

void BlobSimulation::render() {
    window.clear(sf::Color(20, 20, 30));
    
    // Use metaball rendering for morphing effect
    renderMetaballs();
    
    window.display();
}

void BlobSimulation::renderBlob(const Blob& blob) {
    const int segments = 64;
    sf::VertexArray vertices(sf::TriangleFan, segments + 2);
    
    vertices[0].position = blob.getPosition();
    vertices[0].color = blob.getColor();
    
    for (int i = 0; i <= segments; ++i) {
        float angle = (i * 2 * M_PI) / segments;
        float x = blob.getPosition().x + std::cos(angle) * blob.getRadius();
        float y = blob.getPosition().y + std::sin(angle) * blob.getRadius();
        vertices[i + 1].position = sf::Vector2f(x, y);
        vertices[i + 1].color = blob.getColor();
    }
    
    // Skip shader for now to ensure basic rendering works
    window.draw(vertices);
    return;
    
    sf::Shader* shader = shaderManager.getBlobShader();
    if (shader) {
        auto windowSize = window.getSize();
        sf::Transform transform;
        transform = sf::Transform::Identity;
        transform.scale(2.0f / windowSize.x, -2.0f / windowSize.y);
        transform.translate(-1.0f, 1.0f);
        
        shader->setUniform("projection", sf::Glsl::Mat4(transform));
        shader->setUniform("blobCenter", blob.getPosition());
        shader->setUniform("blobRadius", blob.getRadius());
        shader->setUniform("blobColor", sf::Glsl::Vec4(blob.getColor()));
        shader->setUniform("distortionFactor", blob.getDistortionFactor());
        shader->setUniform("distortionDirection", sf::Glsl::Vec2(blob.getDistortionDirection()));
        
        window.draw(vertices, shader);
    } else {
        for (std::size_t i = 0; i < vertices.getVertexCount(); ++i) {
            vertices[i].color = blob.getColor();
        }
        window.draw(vertices);
    }
}

void BlobSimulation::renderMetaballs() {
    sf::Shader* shader = shaderManager.getMetaballShader();
    
    if (!shader) {
        // Fallback to regular rendering
        for (const auto& blob : blobs) {
            renderBlob(blob);
        }
        return;
    }
    
    // Create fullscreen quad
    sf::VertexArray quad(sf::TriangleStrip, 4);
    quad[0].position = sf::Vector2f(0, 0);
    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].position = sf::Vector2f(window.getSize().x, 0);
    quad[1].texCoords = sf::Vector2f(1, 0);
    quad[2].position = sf::Vector2f(0, window.getSize().y);
    quad[2].texCoords = sf::Vector2f(0, 1);
    quad[3].position = sf::Vector2f(window.getSize().x, window.getSize().y);
    quad[3].texCoords = sf::Vector2f(1, 1);
    
    // Set shader uniforms
    shader->setUniform("resolution", sf::Vector2f(window.getSize()));
    shader->setUniform("blobCount", static_cast<int>(blobs.size()));
    
    // Pass blob data to shader
    for (size_t i = 0; i < blobs.size() && i < 100; ++i) {
        std::string posName = "blobPositions[" + std::to_string(i) + "]";
        std::string radiusName = "blobRadii[" + std::to_string(i) + "]";
        std::string colorName = "blobColors[" + std::to_string(i) + "]";
        
        shader->setUniform(posName, blobs[i].getPosition());
        shader->setUniform(radiusName, blobs[i].getRadius());
        shader->setUniform(colorName, sf::Glsl::Vec4(blobs[i].getColor()));
    }
    
    // Draw with shader
    window.draw(quad, shader);
}

sf::Color BlobSimulation::generateRandomColor() {
    return sf::Color(
        colorDist(rng),
        colorDist(rng),
        colorDist(rng),
        255
    );
}