#pragma once

#include <SFML/System/Vector2.hpp>

constexpr double PI = 3.141592653589793238463;
constexpr float PI_F = 3.14159265358979f;

float lerp(float a, float b, float f);
float sign(float f);
float random(float min, float max);

float distanceSquared(sf::Vector2f a, sf::Vector2f b);
float distance(sf::Vector2f a, sf::Vector2f b);

float lengthSquared(sf::Vector2f a);
float length(sf::Vector2f a);
sf::Vector2f normalize(sf::Vector2f a);
