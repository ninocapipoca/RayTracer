#include "../include/vector.hpp"
#include "../include/sphere.hpp"


Intersection::Intersection(Vector intrs_pt, Vector n, double sol, Vector alb, Material m, bool in) : intersect_pt(intrs_pt), normal(n), t(sol), albedo(alb), m(m), inside(in){};
Intersection::Intersection(Vector intrs_pt, Vector n, double sol, Vector alb, Material m, bool in, double index) : intersect_pt(intrs_pt), normal(n), t(sol), albedo(alb), m(m), inside(in), r_ind(index){};

Intersection::Intersection() : albedo(Vector(0, 0, 0)), normal(Vector(0, 0, 0)), t(0.0), intersect_pt(Vector(0, 0, 0)), m(Material::matte), inside(false), r_ind(1){};
