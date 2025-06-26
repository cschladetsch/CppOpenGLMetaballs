#include "Blob.h"
#include <algorithm>

Blob::Blob(float x, float y, float radius, sf::Color color)
    : position(x, y)
    , previousPosition(x, y)
    , acceleration(0.0f, 0.0f)
    , radius(radius)
    , color(color) {
    updateMass();
}

void Blob::update(float dt, const sf::Vector2u& windowSize) {
    verletIntegration(dt);
    wrapBounds(windowSize);
    
    distortionFactor *= 0.95f;
}

void Blob::applyForce(const sf::Vector2f& force) {
    acceleration += force / mass;
}

void Blob::verletIntegration(float dt) {
    sf::Vector2f velocity = position - previousPosition;
    velocity *= 0.995f; // Less damping for more sustained movement
    previousPosition = position;
    position += velocity + acceleration * dt * dt;
    acceleration = sf::Vector2f(0.0f, 0.0f);
}

void Blob::wrapBounds(const sf::Vector2u& windowSize) {
    if (position.x < -radius) {
        position.x += windowSize.x + 2 * radius;
        previousPosition.x += windowSize.x + 2 * radius;
    } else if (position.x > windowSize.x + radius) {
        position.x -= windowSize.x + 2 * radius;
        previousPosition.x -= windowSize.x + 2 * radius;
    }
    
    if (position.y < -radius) {
        position.y += windowSize.y + 2 * radius;
        previousPosition.y += windowSize.y + 2 * radius;
    } else if (position.y > windowSize.y + radius) {
        position.y -= windowSize.y + 2 * radius;
        previousPosition.y -= windowSize.y + 2 * radius;
    }
}

float Blob::calculateDistance(const Blob& other) const {
    sf::Vector2f diff = position - other.position;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void Blob::handleCollision(Blob& other) {
    float distance = calculateDistance(other);
    float minDistance = radius + other.radius;
    
    if (distance < minDistance * 1.5f && distance > 0.001f) {
        sf::Vector2f direction = (other.position - position) / distance;
        
        float distortionStrength = 1.0f - (distance / (minDistance * 1.5f));
        distortionFactor = distortionStrength * 0.3f;
        distortionDirection = direction;
        
        other.distortionFactor = distortionStrength * 0.3f;
        other.distortionDirection = -direction;
        
        if (distance < minDistance) {
            // Don't separate blobs - let them overlap for merging
            // Only apply distortion effects, no physical separation
        }
    }
}

bool Blob::shouldMerge(const Blob& other) const {
    float distance = calculateDistance(other);
    float mergeThreshold = (radius + other.radius) * 0.8f; // Merge more easily
    return distance < mergeThreshold;
}

Blob Blob::merge(const Blob& a, const Blob& b) {
    float totalMass = a.mass + b.mass;
    float newRadius = std::sqrt(totalMass / (DENSITY * M_PI));
    
    sf::Vector2f newPosition = (a.position * a.mass + b.position * b.mass) / totalMass;
    
    sf::Color newColor;
    float massRatio = a.mass / totalMass;
    newColor.r = static_cast<sf::Uint8>(a.color.r * massRatio + b.color.r * (1 - massRatio));
    newColor.g = static_cast<sf::Uint8>(a.color.g * massRatio + b.color.g * (1 - massRatio));
    newColor.b = static_cast<sf::Uint8>(a.color.b * massRatio + b.color.b * (1 - massRatio));
    newColor.a = 255;
    
    Blob merged(newPosition.x, newPosition.y, newRadius, newColor);
    
    sf::Vector2f momentum = (a.position - a.previousPosition) * a.mass + 
                           (b.position - b.previousPosition) * b.mass;
    sf::Vector2f newVelocity = momentum / totalMass;
    merged.previousPosition = merged.position - newVelocity;
    
    return merged;
}

void Blob::setRadius(float r) {
    radius = r;
    updateMass();
}

void Blob::updateMass() {
    mass = DENSITY * M_PI * radius * radius;
}