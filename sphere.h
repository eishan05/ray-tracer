#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "material.h"

class sphere: public hitable {
    public:
    sphere() {}
    
    sphere(vec3 cen, float r, material* m_ptr) : center(cen), radius(r), material_ptr(m_ptr) {};

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

    vec3 center;
    float radius;
    material *material_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record & rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b - a*c)) / a;
        rec.mat_ptr = material_ptr;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = -(b + sqrt(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}

#endif
