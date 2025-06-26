#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>

class Blob {
public:
    static constexpr float DENSITY = 1.0f; // All blobs have same density
    
    Blob(float x, float y, float radius, sf::Color color);
    
    void update(float dt, const sf::Vector2u& windowSize);
    void applyForce(const sf::Vector2f& force);
    void handleCollision(Blob& other);
    
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    sf::Vector2f getPosition() const { return position; }
    float getRadius() const { return radius; }
    float getMass() const { return mass; }
    sf::Color getColor() const { return color; }
    float getDensity() const { return DENSITY; }
    
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setPreviousPosition(const sf::Vector2f& pos) { previousPosition = pos; }
    void setRadius(float r);
    void setColor(const sf::Color& c) { color = c; }
    
    float getDistortionFactor() const { return distortionFactor; }
    sf::Vector2f getDistortionDirection() const { return distortionDirection; }
    
    bool shouldMerge(const Blob& other) const;
    static Blob merge(const Blob& a, const Blob& b);
    
private:
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Vector2f acceleration;
    float radius;
    float mass;
    sf::Color color;
    
    float distortionFactor = 0.0f;
    sf::Vector2f distortionDirection;
    
    void verletIntegration(float dt);
    void wrapBounds(const sf::Vector2u& windowSize);
    float calculateDistance(const Blob& other) const;
    void updateMass();
};