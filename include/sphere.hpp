#pragma once
#include "intersection.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <vector>
#include "material.hpp"


class Sphere
{
public:
    Vector center, albedo;
    double radius;
    Material mat;
    double r_ind;
    Sphere(Vector ctr, Vector alb, double rad, Material m);
    Sphere(Vector ctr, Vector alb, double rad, Material m, double r_ind);

    Intersection calculate_intersect(Ray ray);
};
