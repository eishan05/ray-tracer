# include<iostream>
#include<stdio.h>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "light.h"
#include "triangle.h"
#include "box.h"

light light_source;

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                } else if (choose_mat < 0.95) {
                    // metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
                } else {
                    // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    light_source.directional = true;
    light_source.direction = unit_vector(vec3(0, 1, 1) - vec3(0, 0, 0));
    light_source.light_color = vec3(1.0, 1.0, 1.0);
    light_source.light_intensity = 0.80;
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    // list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    // list[i++] = new triangle(vec3(4, 2, 0), vec3(4, 0, 1), vec3(4, 0, -1), new metal(vec3(0.7, 0.6, 0.5), 0.0));
    box* b = new box(vec3(-1, -1, -1), vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1), vec3(-1, -1, 1), vec3(1, -1, 1), vec3(1,1, 1), vec3(-1, 1, 1), new lambertian(vec3(0.4, 0.2, 0.1)));
    b->translate(vec3(3, 0, 0));
    list[i++] = b;
    // list[i++] = new rectangle(vec3(4, 2, 1), vec3(4, 0, 1), vec3(4, 0, -1), vec3(4, 2, -1), new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}

vec3 calculateColor(const ray& r, hitable* world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            // Calculate the diffuse color at the point that is hit
            // vec3 diffuse = light_source.light_intensity * std::max(0.0, dot(light_source.direction, rec.normal)) * light_source.light_color;
            return attenuation*calculateColor(scattered, world, depth + 1);
        } else {
            return vec3(0.0, 0.0, 0.0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    (void)!freopen("output.ppm", "w", stdout);
    
    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int nx = 720;
    const int ny = static_cast<int>(nx / aspect_ratio);
    const int ns = 50;
    const int max_depth = 50;

    // World

    auto world = random_scene();

    // Camera

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += calculateColor(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}