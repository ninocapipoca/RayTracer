#include "../include/vector.hpp"
#include "../include/sphere.hpp"
#include "../include/intersection.hpp"
#include "../include/ray.hpp"
#include "../include/material.hpp"

#define SQUARE(a) (a * a)

Sphere::Sphere(Vector ctr, Vector alb, double rad, Material m) : center(ctr), albedo(alb), radius(rad), mat(m){};
Sphere::Sphere(Vector ctr, Vector alb, double rad, Material m, double r_ind) : center(ctr), albedo(alb), radius(rad), mat(m), r_ind(r_ind){};

Intersection Sphere::calculate_intersect(Ray ray)
{
    Vector diff = ray.origin - center;
    double norm = diff.norm();
    double delta = SQUARE(dot(ray.direction, diff)) - (SQUARE(norm) - SQUARE(radius));

    if (delta < 0)
    {
        return Intersection(); // no intersection
    }

    double t1, t2;
    double p = dot(ray.direction, (center - ray.origin));
    t1 = p - sqrt(delta);
    t2 = p + sqrt(delta);
    if (delta == 0)
    { // Both solutions are the same
        double sol = p;
        if (sol >= 0)
        { // The unique solution is in front of ray origin
            Vector intersection_pos = ray.origin + ray.direction * sol;
            Vector normal = (intersection_pos - center);
            normal.normalize();
            return Intersection(intersection_pos, normal, sol, albedo, mat, false);
        }
        return Intersection();
    }
    // if we got this far it means there's two solutions
    if (t1 >= 0) // The first solution is in front of the ray
    {
        Vector p = ray.origin + (ray.direction * t1); // intersection pt
        Vector diff2 = p - center;
        Vector n = diff2 / diff2.norm(); // normal vector
        if (mat == transparent){ // if transparent, we have a refractive index
          return Intersection(p, n, t1, albedo, mat, false, r_ind);
        }
        return Intersection(p, n, t1, albedo, mat, false);

    }
    if (t2 >= 0) // The second solution is in front of the ray
    {
        // going from inside to outside
        Vector p = ray.origin + (ray.direction * t2); // intersection pt
        Vector diff2 = p - center;
        Vector n = -1* (diff2 / diff2.norm()); // normal vector

        if (mat == transparent){
          return Intersection(p, n, t2, albedo, mat, true, r_ind);
        }
        return Intersection(p, n, t2, albedo, mat, true);
    }
    // Neither solution is in front of the ray
    return Intersection();
}
