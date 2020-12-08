#ifndef LIGHTH
#define LIGHTH

#include "vec3.h"

struct light {
    bool directional;
    vec3 point;
    vec3 direction;
    vec3 light_color;
    double light_intensity;
};

#endif