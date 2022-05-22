#pragma once
#include "vector.hpp"
#include "../include/material.hpp"

class Intersection
{
public:
    Vector albedo;
    Vector intersect_pt, normal;
    double t;
    Material m;
    bool inside;
    double r_ind;
    Intersection(Vector intrs_pt, Vector n, double sol, Vector alb, Material m, bool in);
    Intersection(Vector intrs_pt, Vector n, double sol, Vector alb, Material m, bool in, double index);
    Intersection();
};
