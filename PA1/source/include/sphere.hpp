#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

using namespace std;
// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Vector3f center;
    float radius;
    Material *material;
    Sphere() {
        // unit ball at the center
        this->center = Vector3f(0.0f, 0.0f, 0.0f);
        this->radius = 1.0f;
        this->material = nullptr;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
        this->material = material;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection().normalized();
        Vector3f origin_to_center = this->center - origin;
        if (origin_to_center.squaredLength() < this->radius * this->radius) {
            //光源在球内部
            float projected_len = origin_to_center.dot(origin_to_center, direction);
            float squared_dis = origin_to_center.squaredLength() - projected_len *projected_len;
            float bias = sqrt(this->radius * this->radius - squared_dis);
            float t = projected_len + bias;
            if (t < tmin || t > h.getT()) {
                return false;
            }
            else {
                Vector3f hit_point = origin + t * direction;
                Vector3f norm = (this->center - hit_point).normalized();
                h.set(t, this->material, norm);
                cout << "Sphere hit, t=" << t << endl;
                return true;
            }
        }
        else if (origin_to_center.squaredLength() > this->radius * this->radius) {
            //光源在球外部
            float projected_len = origin_to_center.dot(origin_to_center, direction);
            if (projected_len < 0){
                return false;
            }
            float squared_dis = origin_to_center.squaredLength() - projected_len *projected_len;
            if (squared_dis > this->radius * this->radius) {
                return false;
            }        
            float bias = sqrt(this->radius * this->radius - squared_dis);
            float t = projected_len - bias;
            if (t < tmin || t > h.getT()) {
                return false;
            }
            else {
                Vector3f hit_point = origin + t * direction;
                Vector3f norm = (hit_point - this->center).normalized();
                h.set(t, this->material, norm); 
                return true;
            }
        }
        else {
            //光源在球面上；此时没有把光源算作交点
            float projected_len = origin_to_center.dot(origin_to_center, direction);
            if (projected_len <= 0){
                return false;
            }
            float squared_dis = origin_to_center.squaredLength() - projected_len *projected_len;
            if (squared_dis > this->radius * this->radius) {
                return false;
            }        
            float bias = sqrt(this->radius * this->radius - squared_dis);
            float t = projected_len + bias;
            if (t < tmin || t > h.getT()) {
                return false;
            }
            else {
                Vector3f hit_point = origin + t * direction;
                Vector3f norm = (this->center - hit_point).normalized();
                h.set(t, this->material, norm);
                return true;
            }
        }
        return false;
    }

protected:

};


#endif
