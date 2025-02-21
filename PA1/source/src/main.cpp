#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include <string>

using namespace std;















// // transforms a 3D point using a matrix, returning a 3D point
// static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
//     return (mat * Vector4f(point, 1)).xyz();
// }

// // transform a 3D direction using a matrix, returning a direction
// static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
//     return (mat * Vector4f(dir, 0)).xyz();
// }

// class Transform : public Object3D {
// public:
//     Transform() {}

//     Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
//         transform = m.inverse();
//     }

//     ~Transform() {
//     }

//     virtual bool intersect(const Ray &r, Hit &h, float tmin) {

//         Vector3f trSource = transformPoint(transform, r.getOrigin());
//         Vector3f trDirection = transformDirection(transform, r.getDirection());
//         std::cout << "trSource=" ;
//         trSource.print();
//         std::cout << "trDir=" ;
//         trDirection.print();
//         Ray tr(trSource, trDirection);
//         bool inter = o->intersect(tr, h, tmin);
//         if (inter) {
//             std::cout << "transformed t=" << h.getT() << endl;
//             h.set(h.getT(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized());
//         }
//         return inter;
//     }

// protected:
//     Object3D *o; //un-transformed object
//     Matrix4f transform;
// };





















int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    SceneParser sceneParser(inputFile.c_str());
    Camera* camera = sceneParser.getCamera();
    Image Image(camera->getWidth(), camera->getHeight());


    // 循环屏幕空间的像素
    for (int x = 0; x < camera->getWidth(); ++x) {
        for (int y = 0; y < camera->getHeight(); ++y) {
            // 计算当前像素(x, y)处相机出射光线camRay
            Ray camRay = sceneParser.getCamera()->generateRay(Vector2f(x, y));
            Group* baseGroup = sceneParser.getGroup();
            Hit hit;
            // 判断camRay是否和场景有交点，并返回最近交点的数据，存储在hit中
            // if ( x%10==0 && y%10==0) {
            //     cout << "x=" << x << endl;
            // cout << "y=" << y << endl;
            // }
            
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            // cout << "isInter=" << isIntersect << endl;
            if (isIntersect) {
                Vector3f finalColor = Vector3f::ZERO;
                // 找到交点之后，累加来自所有光源的光强影响
                for (int li = 0; li < sceneParser.getNumLights(); ++li) {
                    Light* light = sceneParser.getLight(li);
                    Vector3f L, lightColor;
                    // 获取光照强度
                    light->getIllumination(camRay.pointAtParameter(hit.getT()), L, lightColor);
                    // 计算局部光强
                    // cout << "Before shade" << endl;
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightColor);
                }
                Image.SetPixel(x, y, finalColor);
            } else {
                // 不存在交点，返回背景色
                Image.SetPixel(x, y, sceneParser.getBackgroundColor());
            }
        }
    }
    Image.SaveImage(outputFile.c_str());






























// // #include "plane.hpp"
// //     Vector3f P1(1.0f, 2.0f, 0.0f);
// //     Vector3f P2(1.0f, 0.0f, 7.0f);
// //     Vector3f P3(1.0f, 0.0f, 3.0f);
// //     Plane myPlane(P1, P2, P3, nullptr);
// #include "ray.hpp"
//     Vector3f ori(0.0f, 0.0f, 0.0f);
//     Vector3f direction(0.0f, 2.0f, 0.0f);
//     Ray myRay(ori, direction);
// #include "triangle.hpp"
//     Vector3f P1(2.0f, 2.0f, 0.0f);
//     Vector3f P2(0.0f, 2.0f, 0.0f);
//     Vector3f P3(0.0f, 2.0f, 2.0f);
//     Triangle myTriangle(P1, P2, P3, nullptr);
// #include "sphere.hpp"
//     Vector3f center(0.0f, 0.0f, 0.0f);
//     Sphere mySphere(center, 2.0f, nullptr);
//     Object3D* mySpherePointer = &myTriangle;
// #include "hit.hpp"
//     Hit myHit(10, nullptr, ori);
// //     std::cout << mySphere.intersect(myRay, myHit, 0) << endl;
// // #include "transform_std.hpp"
//     Matrix4f compressionMatrix(1.0f, 0.0f, 0.0f, 0.0f,
//                            0.0f, -2.0f, 0.0f, 0.0f,
//                            0.0f, 0.0f, 1.0f, 0.0f,
//                            0.0f, 0.0f, 0.0f, 1.0f);
//     // compressionMatrix = compressionMatrix.identity();
                           
//     Transform * myTransform = new Transform(compressionMatrix, mySpherePointer);
//     myTransform->intersect(myRay, myHit, 0);
//     // compressionMatrix.print();
    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

