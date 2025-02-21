#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>
using namespace std;
// TODO (PA2): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    int n, k;
    double bgn, end;
    vector<double> tpad, t;

    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    int get_bpos(double mu) {
       int bpos;
        for (bpos = 0; bpos < t.size(); ++bpos) {
            if (mu < t[bpos]) {
                break;
            }
        }
        bpos -= 1;
        return bpos;
    }

    void evaluete(vector<double> &s, vector<double> &ds, int bpos, double mu) {
        double w1, dw1, w2, dw2;
        // 外层循环，是循环次数degree
        for(int p = 1; p < k + 1; p++) {
            // 内存循环，是循环“支撑区间”；因为“有效值”是从s[-2]开始扩散的，所以循环范围也是“扩散”的（以提升程序效率）
            for(int ii = k - p; ii < k + 1; ii++) {
                int i = ii + bpos - k;
                // 避免分母为0
                if (tpad[i + p] == tpad[i]){
                    w1 = mu;
                    dw1 = 1;
                }
                else {
                    w1 = (mu - tpad[i]) / (tpad[i + p] - tpad[i]);
                    dw1 = 1 / (tpad[i + p] - tpad[i]);
                }
                // 避免分母为0
                if (tpad[i + p + 1] == tpad[i + 1]){
                    w2 = 1 - mu;
                    dw2 = -1;
                }
                else {
                    w2 = (tpad[i + p + 1] - mu) / (tpad[i + p + 1] - tpad[i + 1]);
                    dw2 = - 1 / (tpad[i + p + 1] - tpad[i + 1]);
                }
                if (p == k) {
                    ds[ii] = (dw1 * s[ii] + dw2 * s[ii + 1]) * p;
                }
                s[ii] = w1 * s[ii] + w2 * s[ii + 1];
            }
        }
        s.pop_back();
    }
    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        // this->n = controls.size();
        // this->k = n - 1;
        // this->t.resize(n + k + 1);
        // this->tpad.resize(n + 2*k + 1);
        // this->bgn = 0;
        // this->end = 1;
        // for (int i = 0; i < n; i++) {
        //     t[i] = 0;
        //     t[i + n] = 1;
        //     tpad[i] = 0;
        //     tpad[i + n] = 1; 
        // }
        // for (int i = n + k + 1; i < n + 2*k + 1; i++) {
        //     tpad[i] = t.back();
        // }
    }

    //de casteljau
    //ori
    // void discretize(int resolution, std::vector<CurvePoint>& data) override {
    //     data.clear();
    //     n = getControls().size();
    //     // printf("in Bezier n = %d\n", n);
    //     vector<Vector3f> subpoints;
    //     int num_points = resolution * (n / 3);
    //     for (int i = 0; i <= num_points; i++) {
    //         double t = i / (double) num_points;
    //         subpoints.clear();
    //         // copy(getControls().begin(), getControls().end(), back_inserter(subpoints));
    //         for (auto p: this->getControls()) {
    //             subpoints.push_back(p);
    //         }
    //         while(1) {
    //             for (int i = 0; i < subpoints.size() - 1; i++) {
    //                 subpoints[i] = (t) * subpoints[i + 1] + (1 - t) * subpoints[i] ;
    //             }
    //             subpoints.pop_back();
    //             if (subpoints.size() <= 2) {
    //                 break;
    //             }
    //         }

    //         CurvePoint new_curvepoint;
    //         new_curvepoint.T = (subpoints[1] - subpoints[0]).normalized();
    //         new_curvepoint.V = (1 - t) * subpoints[0] + t * subpoints[1];
    //         data.push_back(new_curvepoint);

    //         subpoints.pop_back();
    //         subpoints.pop_back();
    //     }
    // }

    //de casteljau
    //modified
    // void discretize(int resolution, std::vector<CurvePoint>& data) override {
    //     data.clear();
    //     n = getControls().size();
    //     for (int seg = 0; seg < n - 1; seg += 3) {
    //         for (int i = 0; i <= resolution; i++) {
    //             double t = i * 1.0 / resolution;
    //             vector<Vector3f> sub_controls;
    //             for (int j = 0; j <= 3; j++) {
    //                 sub_controls.push_back(controls[seg+j]);
    //             } 
    //             while(sub_controls.size() > 2) {
    //                 for (int k = 0; k < sub_controls.size() - 1; k++) {
    //                     sub_controls[k] = (t) * sub_controls[k + 1] + (1 - t) * sub_controls[k] ;
    //                 }
    //                 sub_controls.pop_back();
    //             }
    //             Vector3f V = (1 - t) * sub_controls[0] + (t) * sub_controls[1];
    //             Vector3f T = (sub_controls[1] - sub_controls[0]).normalized();
    //             data.push_back(CurvePoint{V, T});
    //         }
    //     }
    // }



    //method: fomula
    // ori
    // void discretize(int resolution, std::vector<CurvePoint>& data) override {
    //     data.clear();
    //     n = getControls().size();
    //     vector<Vector3f> control = getControls();
    //     int num_points = resolution * (n / 3);
    //     for (int i = 0; i <= num_points; i++) {
    //         double t = i / (double) num_points;
    //         Vector3f V = control[0] * t * t * t + control[1] * 3 * t * t * (1 - t) + control[2] * 3 * t * (1 - t) * (1 - t) + control[3] * (1 - t) * (1 - t) * (1 - t);
    //         Vector3f T = control[0] * (-t) * t + control[1] * t * (3 * t - 2) + control[2] * (1 - t) * (3 * t - 1) + control[3] * (1 - t) * (1 - t);
    //         data.push_back(CurvePoint{V, T.normalized()});
    //     }
    // }

    //modified
    // void discretize(int resolution, std::vector<CurvePoint>& data) override {
    //     data.clear();
    //     n = getControls().size();
    //     vector<Vector3f> control = getControls();
    //     int num_points = resolution * (n / 3);
    //     for (int i = 0; i <= num_points; i++) {
    //         double t = i / (double) num_points;
    //         Vector3f V = control[3] * t * t * t + control[2] * 3 * t * t * (1 - t) + control[1] * 3 * t * (1 - t) * (1 - t) + control[0] * (1 - t) * (1 - t) * (1 - t);
    //         Vector3f T = control[3] * (-t) * t + control[2] * t * (3 * t - 2) + control[1] * (1 - t) * (3 * t - 1) + control[0] * (1 - t) * (1 - t);
    //         data.push_back(CurvePoint{V, T.normalized()});
    //     }
    // }

    //ori
    // void discretize(int resolution, std::vector<CurvePoint>& data) override {
    //     data.clear();
    //     double step = 1.0 /resolution;
    //     for(int i=0;i<controls.size()-1;i += 3) {
    //         for(double t=0;t<=1;t += step){
    //             Vector3f V= controls[3+i] * t * t * t + controls[2+i] * 3 * t * t * (1 - t) + controls[1+i] * 3 * t * (1 - t) * (1 - t) + controls[0+i] * (1 - t) * (1 - t) * (1 - t);
    //             Vector3f T = controls[3+i] * (-t) * t + controls[2+i] * t * (3 * t - 2) + controls[1+i] * (1 - t) * (3 * t - 1) + controls[0+i] * (1 - t) * (1 - t);
    //             data.push_back(CurvePoint{V,T.normalized()});
    //         }

    //     }


        // vector<Vector3f> control = getControls();
        // double step = 1.0 / resolution;
        // for (double t = 0; t <= 1.0 + 1e-8; t+=step) {
        //     Vector3f V = control[0] * t * t * t + control[1] * 3 * t * t * (1 - t) + control[2] * 3 * t * (1 - t) * (1 - t) + control[3] * (1 - t) * (1 - t) * (1 - t);
        //     Vector3f T = control[0] * (-t) * t + control[1] * t * (3 * t - 2) + control[2] * (1 - t) * (3 * t - 1) + control[3] * (1 - t) * (1 - t);
        //     data.push_back(CurvePoint{V, T.normalized()});
        // }
    // }

    //modified
    void discretize(int resolution, std::vector<CurvePoint>& data) {
        data.clear();
        vector<Vector3f> control = getControls();
        double step = 1.0 / resolution;
        for (int i = 0; i < getControls().size(); i+=3) {
            for (double t = 1.0; t >= 0.0; t-=step) {
                Vector3f V = control[i+3] * t * t * t + control[i+2] * 3 * t * t * (1 - t) + control[i+1] * 3 * t * (1 - t) * (1 - t) + control[i+0] * (1 - t) * (1 - t) * (1 - t);
                Vector3f T = control[i+3] * (3 * t) * t + control[i+2] * (3 * t) * (2 - 3 * t) + control[i+1] * (3) *(t - 1) * (3 * t - 1) + control[i+0] * (-3) * (1 - t) * (1 - t);
                data.push_back(CurvePoint{V, T.normalized()});
            }
        }
    }


protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
        this->k = 3;
        this->n = controls.size();
        this->t.resize(n + k + 1);
        this->tpad.resize(n + 2*k + 1);
        for (int i = 0; i < t.size(); i++) {
            //注意，一共n+k+1个点，但是n+k个线段，分母为n+k
            t[i] = (double)i / (n + k);
            tpad[i] = t[i];
        }
        for (int i = n + k + 1; i < n + 2*k + 1; i++) {
            tpad[i] = t.back();
        }
        this->bgn = t[k];
        this->end = t[n];
    }
    

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) {
        int num_points = resolution * (n);
        data.resize(num_points);
        for (int i = 0; i < num_points; i++) {
            double mu = bgn + (end - bgn) * (i / (double)num_points);
            int bpos = get_bpos(mu);

            vector<double> s(k+2, 0);
            s[k] = 1;
            vector<double> ds(k+1, 1);
            evaluete(s, ds, bpos, mu);

            data[i].V = Vector3f::ZERO;
            data[i].T = Vector3f::ZERO;

            int lsk = bpos - k;
            int rsk = n - bpos - 1;
            if (lsk < 0) {
                s.erase(s.begin(), s.begin() - lsk);
                ds.erase(ds.begin(), ds.begin() - lsk);
                lsk = 0;
            }
            if (rsk < 0) {
                if (rsk + s.size() < 0) {
                    s.clear();
                    ds.clear();
                } else {
                    s.erase(s.end() + rsk, s.end());
                    ds.erase(ds.end() + rsk, ds.end());
                }
            }

            data[i].V = Vector3f::ZERO;
            data[i].T = Vector3f::ZERO;
            for (int r = 0; r < s.size(); r++) {
                data[i].T += ds[r] * controls[lsk + r];
                data[i].V += s[r] * controls[lsk + r];
            }
            data[i].T = data[i].T.normalized();
        }
        
    }



protected:

};

#endif // CURVE_HPP
