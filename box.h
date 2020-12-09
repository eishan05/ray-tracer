#ifndef BOXH
#define BOXH

#include "hitable.h"
#include "material.h"
#include "triangle.h"

class rectangle: public hitable {
    public:
    rectangle() {}
    // Should be counter clockwise contiguous
    rectangle(vec3 a, vec3 b, vec3 c, vec3 d, material* ptr) {
        t1 = new triangle(a, b, c, ptr);
        t2 = new triangle(c, d, a, ptr);
    }
    triangle* t1;
    triangle* t2;

    void translate(vec3 translation) {
        t1->translate(translation);
        t2->translate(translation);
    }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
};

bool rectangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    return t1->hit(r, tmin, tmax, rec) || t2->hit(r, tmin, tmax, rec);
}

class box: public hitable {
    public:
    box() {}

    box(vec3 _0, vec3 _1, vec3 _2, vec3 _3, vec3 _4, vec3 _5, vec3 _6, vec3 _7, material* ptr) {
        r1 = new rectangle(_1, _2, _6, _5, ptr);
        r2 = new rectangle(_2, _3, _7, _6, ptr);
        r3 = new rectangle(_6, _7, _4, _5, ptr);
        r4 = new rectangle(_0, _4, _7, _3, ptr);
        r5 = new rectangle(_5, _4, _1, _0, ptr);
        r6 = new rectangle(_1, _0, _3, _2, ptr);
    }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

    void translate(vec3 translation) {
        r1->translate(translation);
        r2->translate(translation);
        r3->translate(translation);
        r4->translate(translation);
        r5->translate(translation);
        r6->translate(translation);
    }

    rectangle* r1;
    rectangle* r2;
    rectangle* r3;
    rectangle* r4;
    rectangle* r5;
    rectangle* r6;
};

bool box:: hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    return r1->hit(r, tmin, tmax, rec) || 
        r2->hit(r, tmin, tmax, rec) || 
        r3->hit(r, tmin, tmax, rec) || 
        r4->hit(r, tmin, tmax, rec) || 
        r5->hit(r, tmin, tmax, rec) || 
        r6->hit(r, tmin, tmax, rec);
}

#endif