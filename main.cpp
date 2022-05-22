#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "include/stage.hpp"
#include "include/ray.hpp"
#include "include/intersection.hpp"
#include "include/vector.hpp"
#include "include/light.hpp"
#include "include/material.hpp"

#define NB_RAYS         15
#define ANTI_ALIASING   1

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
          std::cout << "each pixel" << std::endl;
          int sent = 0;
          double rand_x = 0;
          double rand_y = 0;
          double rand_z = 0;
          Vector color = Vector(0,0,0);

          while (sent < NB_RAYS){
            #if ANTI_ALIASING == 1
            std::cout << "attempting anti-aliasing" << std::endl;
              double rand1 = rand()/double(RAND_MAX);
              double rand2 = rand()/double(RAND_MAX);


              rand_x = std::cos(2*M_PI*rand1)*std::sqrt(1-rand2);
              rand_y = std::sin(2*M_PI*rand1)*std::sqrt(1-rand2);
              rand_z = std::sqrt(rand2);
           #endif



            double x = (j + rand_y) - W / 2 + 0.5;
            double y = H / 2 - (i+ rand_x) - 0.5;
            double z = -W / (2 * tan(fov / 2));
            Vector u = Vector(x,y,z);


            sent++;

            std::cout << u << std::endl;
            u.normalize();

            // Ray: origin, direction
            Ray cameraRay(u, camera);
            Vector pixel_color = stage.get_color(cameraRay, ray_depth);
            color = color + (pixel_color/NB_RAYS);
          }




            //Vector u(j - W / 2 + 0.5, H / 2 - i - 0.5, -W / (2 * tan(fov / 2)));
            //u.normalize();

            clamp_color(color); // makes sure values are within 0 and 255

            // some sort of anti-aliasing here?

            image[(i * W + j) * 3 + 0] = color.data[0];
            image[(i * W + j) * 3 + 1] = color.data[1];
            image[(i * W + j) * 3 + 2] = color.data[2];
            continue;
            //}
            std::cout << "A ray didnt hit anything?!" << std::endl;
        }
    }

    stbi_write_png("image.png", W, H, 3, &image[0], 0);
    return 0;
}
