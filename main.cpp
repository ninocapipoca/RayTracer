#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// use headers!!!!!! one file for each class, import headers as necessary

#include "include/stage.hpp"
#include "include/ray.hpp"
#include "include/intersection.hpp"
#include "include/vector.hpp"
#include "include/light.hpp"
#include "include/material.hpp"

double square(double a)
{
    return a * a;
}

void clamp_color(Vector &color) // and gamma correct
{
    for (int i = 0; i < 3; ++i)
    {
        color[i] = pow(color[i], 0.45);
        if (color[i] < 0)
        {
            color[i] = 0;
        }

        if (color[i] > 255)
        {
            color[i] = 255;
        }
    }
}

void tests()
{
    std::cout << "Intersection test:" << std::endl;
    Sphere s(Vector(10, 0, 0), Vector(0, 0, 0), 5, Material::matte);
    Ray r(Vector(1, 0, 0), Vector(0, 0, 0));
    Intersection i = s.calculate_intersect(r);
    std::cout << "Intersection pos: " << i.intersect_pt << std::endl
              << "Intersection normal: " << i.normal << std::endl;
    std::vector<Sphere> sphere_arr;
    std::vector<pointLight> l_arr;
    sphere_arr.push_back(s);
    Stage scene(sphere_arr, l_arr);
    Intersection i2 = scene.calculate_intersect(r);
    std::cout << "Intersection pos: " << i2.intersect_pt << std::endl
              << "Intersection normal: " << i2.normal << std::endl;
}

int main()
{
    tests();
    int W = 512;
    int H = 512;
    uint ray_depth = 10;

    // TODO - constructors for transparent and mirror spheres, which have no color
    Sphere mirror(Vector(-20, 0, 0), Vector(1, 0, 0), 10, Material::mirror);
    Sphere matte(Vector(0, 0, 0), Vector(0.5, 0, 0),10, Material::matte);
    Sphere transparent(Vector(20, 0, 0), Vector(0.3, 0,0), 10, Material::transparent, 1.4);
    Sphere back_wall(Vector(0, 0, 1000), Vector(1, 0, 0.86), 940, Material::matte);
    Sphere bottom_wall(Vector(0, -1000, 0), Vector(0, 0, 1), 990, Material::matte);
    Sphere left_wall(Vector(0, 0, -1000), Vector(0.13, 0.78, 0.01), 940, Material::matte);
    Sphere top_wall(Vector(0, 1000, 0), Vector(0.7, 0, 0), 940, Material::matte);

    // Sphere light_source(Vector(-10, 20, 40), Vector(0, 1, 0), 1);

    double fov = 60 * M_PI / 180;
    double intensity = pow(10, 9) * 8; // intensity is cst 10^6
    Vector camera(0, 0, 55);

    std::vector<Sphere> sphere_arr{mirror, matte, transparent, back_wall, bottom_wall, left_wall, top_wall};
    std::vector<pointLight> lights;
    lights.push_back(pointLight(Vector(-10, 20, 40), pow(10, 9) * 8));

    Stage stage = Stage(sphere_arr, lights);

    std::vector<unsigned char> image(W * H * 3, 0);

    // for each pixel
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            // send ray through each pixel
            // images from top left to bottom right
            Vector u(j - W / 2 + 0.5, H / 2 - i - 0.5, -W / (2 * tan(fov / 2)));
            u.normalize();

            // Ray: origin, direction
            Ray cameraRay(u, camera);

            // calculate intersect sphere
            // Intersection raysphere = stage.calculate_intersect(cameraRay);
            // if (raysphere.normal != Vector(0.0, 0.0, 0.0))
            // {
            //
            //     // w_i is lightsource - intersection vector normalised, i.e. reflected direction
            //     Vector w_i = (light_source.center - raysphere.intersect_pt);
            //     w_i.normalize();
            //     double d = distance(light_source.center, raysphere.intersect_pt);
            //     // std::cout << distance << std::endl;
            //     // std::cout << d << std::endl;
            //
            //     // added offset
            //     Ray p_w_ray = Ray(w_i, raysphere.intersect_pt + 0.0001 * raysphere.normal);
            //
            //     // int visible = 1;
            //     Intersection visib_test = stage.calculate_intersect(p_w_ray);
            //     //std::cout << "albedo" << std::endl;
            //     //std::cout << visib_test.albedo.data[0] << " " << visib_test.albedo.data[1] << " " << visib_test.albedo.data[2] << std::endl;
            //     bool visible = (visib_test.normal == Vector(0.0, 0.0, 0.0)); // can we see the light source?
            //
            //     if (visib_test.t > d)
            //     {
            //         visible = true;
            //     }

            // std::cout << "visibility" << std::endl;
            // std::cout << visible << std::endl;

            // equivalent to the value of L in the lecture notes; gives color
            // std::max(0.0,dot(raysphere.normal, w_i))
            // Vector pixel_color = (intensity / (4 * M_PI * d * d)) * (raysphere.albedo / M_PI) * visible * dot(raysphere.normal, w_i);
            Vector pixel_color = stage.get_color(cameraRay, ray_depth);
            clamp_color(pixel_color); // makes sure values are within 0 and 255
            // Vector pixel_color = Vector(255,4,56);

            image[(i * W + j) * 3 + 0] = pixel_color.data[0];
            image[(i * W + j) * 3 + 1] = pixel_color.data[1];
            image[(i * W + j) * 3 + 2] = pixel_color.data[2];
            continue;
            //}
            std::cout << "A ray didnt hit anything?!" << std::endl;
        }
    }

    stbi_write_png("image.png", W, H, 3, &image[0], 0);
    return 0;
}
