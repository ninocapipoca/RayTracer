#pragma once
#include "vector.hpp"

class pointLight
{
public:
    Vector pos;
    double intensity;

    pointLight(Vector p, double i);
    pointLight();
};
