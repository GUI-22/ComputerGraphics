#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
#include "plane.hpp"
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Vector3f normal;
	Vector3f vertices[3];
	Material *material;

	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = a;
		this->vertices[1] = b;
		this->vertices[2] = c;
		this->material = m;
		Vector3f a_to_b = b - a;
        Vector3f a_to_c = c - a;
        this->normal = a_to_b.cross(a_to_b, a_to_c).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
		float d = this->normal.dot(this->normal, this->vertices[0]);
		Plane triangle_plane(this->normal, d, this->material);
		Ray ray_copy(ray);
		Hit hit_copy(hit);
		if (triangle_plane.intersect(ray_copy, hit_copy, tmin) == false) {
			return false;
		}
		Vector3f origin = ray.getOrigin();
        Vector3f direction = ray.getDirection();
		Vector3f E1 = this->vertices[0] - this->vertices[1];
		Vector3f E2 = this->vertices[0] - this->vertices[2];
		Vector3f S = this->vertices[0] - origin;
		Matrix3f matrix_1(direction, E1, E2);
		float denominator = matrix_1.determinant();
		Matrix3f matrix_2(S, E1, E2);
		Matrix3f matrix_3(direction, S, E2);
		Matrix3f matrix_4(direction, E1, S);
		if (std::abs(denominator) < 1e-6) {
			return false;
		}
		float t = matrix_2.determinant() / denominator;
		float beta = matrix_3.determinant() / denominator;
		float gamma = matrix_4.determinant() / denominator;
		// cout << "triangle t=" << t << " beta=" << beta << " gamma=" << gamma << endl;
		if (t > 0 && t >= tmin && t < hit.getT() && \
		beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1\
		&& beta + gamma <= 1) {
			Vector3f N =  this->normal;
			if (this->normal.dot(this->normal, direction) > 0) {
				N = - this->normal;
			}
			hit.set(t, this->material, N);
			return true;
		}
        return false;
	}

protected:

};

#endif //TRIANGLE_H
