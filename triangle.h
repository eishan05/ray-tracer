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
        normal = unit_vector(cross(b - a, c - a));
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
    vec3 w = p1 - r.origin();
    double a = dot(w, normal);
    double b = dot(normal, r.direction());
    vec3 intersection_point = r.direction() * (a / b) + r.origin();
    double area_triangle = 0.5 * (cross(p2 - p1, p3 - p1).length());
    double a1 = 0.5 * (cross(p1 - intersection_point, p2 - intersection_point).length());
    double a2 = 0.5 * (cross(p2 - intersection_point, p3 - intersection_point).length());
    double a3 = 0.5 * (cross(p3 - intersection_point, p1 - intersection_point).length());
    if (abs(area_triangle - (a1 + a2 + a3)) > 1e-4 || (a / b) >= tmax || (a / b) <= tmin) {
        return false;
    }
    rec.mat_ptr = material_ptr;
    rec.normal = normal;
    rec.p = intersection_point;
    rec.t = (a / b);
    return true;
}

#endif