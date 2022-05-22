#pragma once
#include "vector.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include <vector>

class Stage
{
public:
    std::vector<Sphere> sphere_arr;
    std::vector<pointLight> light_arr;
    Stage(std::vector<Sphere> s_arr, std::vector<pointLight> l_arr);

    // CONSTRUCTOR FOR INTERSECT: Vector intrs_pt, Vector n, double sol, Vector albedo
    // automatically set to true

    Intersection calculate_intersect(Ray ray);
    Intersection calculate_intersect_nontransp(Ray ray);
    Vector get_color(Ray ray, uint depth);
    double light_intensity(Intersection intersect);
};
