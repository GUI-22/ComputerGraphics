#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


class Group : public Object3D {

public:

    Group() {
        this->num_objects = 0;
        this->objects = std::vector<Object3D*>(0);
    }

    explicit Group (int num_objects) {
        this->num_objects = num_objects;
        this->objects = std::vector<Object3D*>(num_objects);
    }

    ~Group() override {
        for (auto obj : objects) {
            delete obj;
        }
        this->num_objects = 0;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool hitAnything = false;
        for (auto obj : this->objects) {
            if (obj->intersect(r, h, tmin) == true) {
                hitAnything = true;
            }
        }
        return hitAnything;
    }

    void addObject(int index, Object3D *obj) {
        this->objects[index] = obj;
    }

    int getGroupSize() {
        return this->objects.size();
    }

    void drawGL() override {
        for (auto obj : objects)
            if (obj) obj->drawGL();
    }

private:
    std::vector<Object3D*> objects;
    long long num_objects;
};

#endif
	
