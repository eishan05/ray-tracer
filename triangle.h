#ifndef TRIANGLEH
#define TRIANGLEH

#include "hitable.h"
#include "material.h"

class triangle: public hitable {
    public:
    triangle() {}
    triangle(vec3 a, vec3 b, vec3 c, material* ptr) {
        p1 = a;
        p2 = b;
        p3 = c;
        normal = -unit_vector(cross(b - a, c - a));
        material_ptr = ptr;
    }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;


    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 normal;
    material* material_ptr;
};

bool triangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    // vec3 w = p1 - r.origin();
    // double a = dot(w, normal);
    // double b = dot(normal, r.direction());
    double t = (dot(normal, p1) - dot(normal, r.origin())) / dot(normal, r.direction());
    vec3 intersection_point = r.point_at_parameter(t);
    if (abs(dot(r.direction(), normal)) < 1e-4 || !point_inside_triangle(p1, p2, p3, intersection_point) || t >= tmax || t <= tmin) {
        return false;
    }
    rec.mat_ptr = material_ptr;
    rec.normal = normal;
    rec.p = intersection_point;
    rec.t = t;
    return true;
}

#endif