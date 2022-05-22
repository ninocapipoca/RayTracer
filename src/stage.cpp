#include "../include/intersection.hpp"
#include "../include/stage.hpp"
#include "../include/sphere.hpp"
#include "../include/material.hpp"

#define SQUARE(a) (a * a)

Stage::Stage(std::vector<Sphere> s_arr, std::vector<pointLight> l_arr) : sphere_arr(s_arr), light_arr(l_arr){};

Intersection Stage::calculate_intersect(Ray ray)
{
    double dist = INFINITY;
    Intersection ret_int = Intersection();
    // std::cout << "calculating intersect of scene with ray" << std::endl;

    for (auto it = std::begin(sphere_arr); it < std::end(sphere_arr); ++it)
    {
        Sphere current = *it;
        Intersection temp = current.calculate_intersect(ray);
        if (temp.normal == Vector(0.0, 0.0, 0.0))
        {
            continue;
        }

        double newdist = distance(temp.intersect_pt, ray.origin);
        if (newdist < dist)
        { // if there IS an intersection
            // std::cout << "intersection found" << std::endl;
            dist = newdist;
            ret_int = temp;
            // ret_int.albedo = current.albedo;
        }
    }

    return ret_int;
}

Intersection Stage::calculate_intersect_nontransp(Ray ray)
{
    double dist = INFINITY;
    Intersection ret_int = Intersection();
    // std::cout << "calculating intersect of scene with ray" << std::endl;

    for (auto it = std::begin(sphere_arr); it < std::end(sphere_arr); ++it)
    {
        Sphere current = *it;
        Intersection temp = current.calculate_intersect(ray);
        if (temp.m == transparent){
          continue;
        }

        if (temp.normal == Vector(0.0, 0.0, 0.0))
        {
            continue;
        }

        double newdist = distance(temp.intersect_pt, ray.origin);
        if (newdist < dist)
        { // if there IS an intersection
            // std::cout << "intersection found" << std::endl;
            dist = newdist;
            ret_int = temp;
            // ret_int.albedo = current.albedo;
        }
    }

    return ret_int;
}

double Stage::light_intensity(Intersection intersect)
{
    double totalLight = 0;
    for (auto it = std::begin(light_arr); it < std::end(light_arr); ++it)
    {
        pointLight current = *it;
        Vector w_i = (current.pos - intersect.intersect_pt); // changed from center (i.e. current sphere.center) to light source position
        w_i.normalize();
        double d = distance(current.pos, intersect.intersect_pt);
        // added offset
        Ray p_w_ray = Ray(w_i, intersect.intersect_pt + 0.0001 * intersect.normal);
        // int visible = 1;
        Intersection visib_test = calculate_intersect_nontransp(p_w_ray);

        bool visible = (visib_test.normal == Vector(0.0, 0.0, 0.0)); // can we see the light source?
        if (visib_test.t > d)
        {
            visible = true;
        }
        if (visible)
        {
            totalLight += (current.intensity / (4 * M_PI * M_PI * d * d)) * dot(intersect.normal, w_i);
            ;
        }
    }
    return totalLight;
}

Vector Stage::get_color(Ray ray, uint depth)
{
    if (depth == 0){
      return Vector(0,0,0);
    }
    std::cout << "running get_color" << std::endl;

    Intersection raysphere = calculate_intersect(ray);
    if (raysphere.normal != Vector(0.0, 0.0, 0.0))
    {
      // check material
      if (raysphere.m == matte){
        return light_intensity(raysphere) * raysphere.albedo;
      }

      bool tir = false;
      // bool inside = false;
      if (raysphere.m == transparent){
        // refraction
        double ratio;
        if (raysphere.inside){
          ratio = raysphere.r_ind;
        }
        else{
          ratio = 1/raysphere.r_ind;
        }

        Vector tangent_c = ratio * (ray.direction - dot(ray.direction, raysphere.normal)*raysphere.normal);
        double dot_prod = dot(ray.direction, raysphere.normal);
        double inside = 1-(SQUARE(ratio))*(1-(SQUARE(dot_prod)));
        if (inside < 0){
          tir = true;
        }
        else {
          Vector normal_c = -1 * raysphere.normal * sqrt(inside);
          Vector w_r = tangent_c + normal_c;
          Ray resulting_ray = Ray(w_r, raysphere.intersect_pt - 0.001*raysphere.normal);
          return get_color(resulting_ray, depth-1);
        }



      }

      if (raysphere.m == mirror || tir){
        Vector w_r = ray.direction - 2*dot(ray.direction, raysphere.normal)*raysphere.normal;
        Ray resulting_ray = Ray(w_r, raysphere.intersect_pt + 0.001*raysphere.normal);
        return get_color(resulting_ray, depth-1);
      }

    }

    return Vector(0.0, 0.0, 0.0); // black
}
