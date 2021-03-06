#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"
#include "light.h"

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const light& light_source) const = 0;
};

bool dielectric_refract(const vec3 v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

class lambertian: public material {
    public:
        lambertian(const vec3& a) :albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const light& light_source) const override {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target - rec.p);
            vec3 l_direction;
            if (light_source.directional) {
                l_direction = -light_source.direction;
            } else {
                l_direction = unit_vector(light_source.point - rec.p);
            }
            double NLAngle = std::max(dot(l_direction, rec.normal), 0.0);
            double specular = 0.0;
            if (NLAngle > 0.0) {
                vec3 viewVector = unit_vector(-light_source.view_vector);
                vec3 half_vector = (l_direction + viewVector) / 2.0;
                double specAngle = std::max(dot(rec.normal, half_vector), 0.0);
                specular = std::pow(specAngle, 5.0);
            }
            attenuation = albedo * NLAngle + 2.3 * specular * light_source.light_color + albedo;
            return true;
        }
    
    vec3 albedo;
};

class metal: public material {
    public:
    metal(const vec3& a, float f) : albedo(a) {
        if (f > 1) {
            fuzz = 1;
        } else {
            fuzz = f;
        }
    }
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const light& light_source) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

    vec3 albedo;
    float fuzz;
};

class dielectric : public material {
    public:
        dielectric(float ri) : ref_idx(ri) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, const light& light_source) const override {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            float ni_over_nt;
            attenuation = vec3(1.0, 1.0, 1.0);
            vec3 refracted;
            float reflect_prob;
            float cosine;
            if (dot(r_in.direction(), rec.normal) > 0) {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            } else {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
            }
            if (dielectric_refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
                reflect_prob = reflectance(cosine, ref_idx);
            } else {
                scattered = ray(rec.p, reflected);
                reflect_prob = 1.0;
            }
            if (drand48() < reflect_prob) {
                scattered = ray(rec.p, reflected);
            } else {
                scattered = ray(rec.p, refracted);
            }
            return true;
        }
    
    private:

        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }

        float ref_idx;
};

#endif