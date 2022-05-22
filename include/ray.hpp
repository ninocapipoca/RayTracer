#pragma once
#include "vector.hpp"

class Ray
{ // have a caller function; recursive, to calculate where ray is after a bunch of bounces & color
  // something like get color
public:
    Vector direction, origin;
    Ray(Vector d, Vector o);

    // w_i is lightsource - intersection vector normalised, i.e. reflected direction

    // careful w vector init
    //(intensity / (4 * M_PI * d * d)) * (raysphere.albedo / M_PI) * visible * dot(raysphere.normal, w_i);
};
