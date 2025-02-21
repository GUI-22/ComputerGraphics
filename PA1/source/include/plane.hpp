#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

using namespace std;

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d 和课件上面不同
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Vector3f normal;
    float d;
    Material *material;
    Plane() {
        //默认平面为xy平面 ?
        this->normal = Vector3f::UP;
        this->d = 0.0f;
        this->material = nullptr;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal.normalized();
        this->d = d;
        this->material = m;
    }

    // Plane(const Vector3f &P1, const Vector3f &P2, const Vector3f &P3, Material *m) : Object3D(m) {
    //     Vector3f P1_to_P2 = P2 - P1;
    //     Vector3f P1_to_P3 = P3 - P1;
    //     this->normal = P1_to_P2.cross(P1_to_P2, P1_to_P3).normalized();
    //     this->d = this->normal.dot(this->normal, P1);
    //     this->material = m;
    // }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection().normalized();
        float denominator = Vector3f::dot(this->normal, direction);
        if (denominator < 1e-6 && denominator > 1e-6) {
            return false;
        }
        float t =  (this->d - Vector3f::dot(this->normal, origin)) / denominator;
        if (t > 0 && t >= tmin && t < h.getT()) {
            h.set(t, this->material, this->normal);
            // cout << "Plane intersect t = " << t << endl;
            return true;
        }
        return false;
    }

protected:


};

#endif //PLANE_H
		

