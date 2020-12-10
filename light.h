#ifndef LIGHTH
#define LIGHTH

#include "vec3.h"

struct light {
    bool directional;
    vec3 point;
    vec3 direction;
    vec3 light_color;
    vec3 view_vector;
    double light_intensity;
};

#endif