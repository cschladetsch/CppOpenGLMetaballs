#include <gtest/gtest.h>
#include "../src/Blob.h"
#include <cmath>

class BlobTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(BlobTest, ConstructorTest) {
    Blob blob(100.0f, 200.0f, 50.0f, sf::Color::Red);
    
    EXPECT_FLOAT_EQ(blob.getX(), 100.0f);
    EXPECT_FLOAT_EQ(blob.getY(), 200.0f);
    EXPECT_FLOAT_EQ(blob.getRadius(), 50.0f);
    EXPECT_EQ(blob.getColor(), sf::Color::Red);
    EXPECT_FLOAT_EQ(blob.getDensity(), Blob::DENSITY);
}

TEST_F(BlobTest, MassCalculation) {
    float radius = 30.0f;
    Blob blob(0, 0, radius, sf::Color::Blue);
    
    float expectedMass = Blob::DENSITY * M_PI * radius * radius;
    EXPECT_FLOAT_EQ(blob.getMass(), expectedMass);
}

TEST_F(BlobTest, SetRadiusUpdatesMass) {
    Blob blob(0, 0, 20.0f, sf::Color::Green);
    float oldMass = blob.getMass();
    
    blob.setRadius(40.0f);
    float newMass = blob.getMass();
    
    EXPECT_FLOAT_EQ(newMass / oldMass, 4.0f); // Mass scales with radius squared
}

TEST_F(BlobTest, ApplyForce) {
    Blob blob(100.0f, 100.0f, 25.0f, sf::Color::Yellow);
    sf::Vector2f initialPos = blob.getPosition();
    
    blob.applyForce(sf::Vector2f(100.0f, 0.0f));
    blob.update(0.016f, sf::Vector2u(800, 600)); // ~60Hz frame
    
    EXPECT_GT(blob.getX(), initialPos.x);
    EXPECT_FLOAT_EQ(blob.getY(), initialPos.y);
}

TEST_F(BlobTest, WrapBounds) {
    sf::Vector2u windowSize(800, 600);
    
    // Test horizontal wrap
    Blob blob1(-100.0f, 300.0f, 20.0f, sf::Color::Cyan);
    blob1.update(0.016f, windowSize);
    EXPECT_GT(blob1.getX(), 600.0f);
    
    // Test vertical wrap
    Blob blob2(400.0f, -100.0f, 20.0f, sf::Color::Magenta);
    blob2.update(0.016f, windowSize);
    EXPECT_GT(blob2.getY(), 400.0f);
}

TEST_F(BlobTest, ColorMixing) {
    Blob blob1(0, 0, 30.0f, sf::Color(255, 0, 0)); // Red
    Blob blob2(0, 0, 30.0f, sf::Color(0, 0, 255)); // Blue
    
    Blob merged = Blob::merge(blob1, blob2);
    
    // Equal mass should result in 50/50 color mix
    EXPECT_NEAR(merged.getColor().r, 127, 2);
    EXPECT_NEAR(merged.getColor().g, 0, 2);
    EXPECT_NEAR(merged.getColor().b, 127, 2);
}

TEST_F(BlobTest, MergeConservesMass) {
    Blob blob1(0, 0, 20.0f, sf::Color::Red);
    Blob blob2(50, 0, 30.0f, sf::Color::Blue);
    
    float totalMassBefore = blob1.getMass() + blob2.getMass();
    
    Blob merged = Blob::merge(blob1, blob2);
    
    EXPECT_NEAR(merged.getMass(), totalMassBefore, 0.01f);
}

TEST_F(BlobTest, MergePosition) {
    Blob blob1(0, 0, 20.0f, sf::Color::Red);
    Blob blob2(100, 0, 20.0f, sf::Color::Blue);
    
    Blob merged = Blob::merge(blob1, blob2);
    
    // Equal mass should result in midpoint position
    EXPECT_NEAR(merged.getX(), 50.0f, 0.01f);
    EXPECT_NEAR(merged.getY(), 0.0f, 0.01f);
}

TEST_F(BlobTest, ShouldMergeDistance) {
    Blob blob1(0, 0, 30.0f, sf::Color::Red);
    Blob blob2(50, 0, 30.0f, sf::Color::Blue);
    
    // Total radius = 60, merge threshold = 60 * 0.6 = 36
    // Distance = 50, so should not merge
    EXPECT_FALSE(blob1.shouldMerge(blob2));
    
    Blob blob3(0, 0, 30.0f, sf::Color::Red);
    Blob blob4(35, 0, 30.0f, sf::Color::Blue);
    
    // Distance = 35, threshold = 36, so should merge
    EXPECT_TRUE(blob3.shouldMerge(blob4));
}