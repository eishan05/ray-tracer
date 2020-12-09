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
        material_ptr = ptr;
    }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;


    vec3 p1;
    vec3 p2;
    vec3 p3;
    material* material_ptr;
};

bool triangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    // compute plane normal
    vec3 p1p2 = p2 - p1;
    vec3 p1p3 = p3 - p1;

    vec3 N = cross(p1p2, p1p3);

    // Step 1: finding P

    // chek if ray and plane are parallel?
    double NdotRayDirection = dot(r.direction(), N);
    if (fabs(NdotRayDirection) < 1e-8) {
        return false;
    }

    // compute d param
    double d = dot(N, p1);

    // compute t
    double t = -(dot(N, r.origin()) - d) / NdotRayDirection;
    // check if the triangle is behind the ray or out of bounds
    if (t <= tmin || t >= tmax) {
        return false;
    }

    // compute the intersection point
    vec3 intersection = r.point_at_parameter(t);

    // inside-outside test
    vec3 C; // vector perpendicular to triangle's plane

    // edge 0
    vec3 edge0 = p2 - p1;
    vec3 intersection_p1 = intersection - p1;
    C = cross(edge0, intersection_p1);
    // intersection is on the right side
    if (dot(N, C) < 0) {
        return false;
    }

    // edge 1
    vec3 edge1 = p3 - p2;
    vec3 intersection_p2 = intersection - p2;
    C = cross(edge1, intersection_p2);
    if (dot(N, C) < 0) {
        return false;
    }

    // edge 2
    vec3 edge2 = p1 - p3;
    vec3 intersection_p3 = intersection - p3;
    C = cross(edge2, intersection_p3);
    if (dot(N, C) < 0) {
        return false;
    }
    
    rec.t = t;
    rec.normal = -unit_vector(N);
    rec.mat_ptr = material_ptr;
    rec.p = intersection;
    return true;
}

#endif