#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <queue>

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Bresenham Algorithm
        // 斜率不存在
        long long x1 = xA, y1 = yA, x2 = xB, y2 = yB;
        if (xA == xB) {
            if (y1 > y2) {
                std::swap(y1, y2);
            }
            for (long long y = y1; y <= y2; y++) {
                img.SetPixel(xA, y, color);
            }
        }
        else {
            //总是(x1, y1)在(x2, y2)左边
            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            //斜率大于0
            if (y2 > y1) {
                long long dx = x2 - x1, dy = y2 - y1;
                long long e = -dx, k_0 = dy / dx, y = y1;
                for (long long x = x1; x <= x2; x++) {
                    img.SetPixel(x, y, color);
                    y += k_0;
                    e += 2 * dy - 2 * k_0 * dx;
                    if (e > 0) {
                        y += 1;
                        e -= 2 * dx;
                    }
                }
            }
            else {
                y2 = 2 * y1 - y2;
                long long dx = x2 - x1, dy = y2 - y1;
                long long e = -dx, k_0 = dy / dx, y = y1;
                for (long long x = x1; x <= x2; x++) {
                    img.SetPixel(x, 2 * y1 - y, color);
                    y += k_0;
                    e += 2 * dy - 2 * k_0 * dx;
                    if (e > 0) {
                        y += 1;
                        e -= 2 * dx;
                    }
                }
            }
        }
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void set_circle_pixel(Image &img, int x, int y, Vector3f color, int cx, int cy) {
        img.SetPixel(x + cx, y + cy, color);
        img.SetPixel(y + cx, x + cy, color);
        img.SetPixel(-x + cx, y + cy, color);
        img.SetPixel(y + cx, -x + cy, color);
        img.SetPixel(x + cx, -y + cy, color);
        img.SetPixel(-y + cx, x + cy, color);
        img.SetPixel(-x + cx, -y + cy, color);
        img.SetPixel(-y + cx, -x + cy, color);
    }
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        int x = 0, y = radius;
        double d = 1.25 - radius;
        set_circle_pixel(img, x, y, color, cx, cy);
        while ( x <= y ) {
            if (d < 0) {
                d += 2 * x + 3;
            }
            else {
                d += 2 * (x - y) + 5;
                y -= 1;
            }
            x += 1;
            set_circle_pixel(img, x, y, color, cx, cy);
        } 
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill
        std::queue<int> queue_x;
        std::queue<int> queue_y;
        queue_x.push(cx);
        queue_y.push(cy);
        Vector3f old_color = img.GetPixel(cx, cy);
        while(queue_x.empty() != true) {
            int x = queue_x.front(), y = queue_y.front();
            queue_x.pop();
            queue_y.pop();
            img.SetPixel(x, y, color);
            if (x - 1 >= 0 && y - 1 >= 0 && img.GetPixel(x - 1, y - 1) == old_color) {
                queue_x.push(x - 1);
                queue_y.push(y - 1);
            }
            if (x - 1 >= 0 && y + 1 < img.Height() && img.GetPixel(x - 1, y + 1) == old_color) {
                queue_x.push(x - 1);
                queue_y.push(y + 1);
            }
            if (x + 1 < img.Width() && y - 1 >= 0 && img.GetPixel(x + 1, y - 1) == old_color) {
                queue_x.push(x + 1);
                queue_y.push(y - 1);
            }
            if (x + 1 < img.Width() && y + 1 < img.Height() && img.GetPixel(x + 1, y + 1) == old_color) {
                queue_x.push(x + 1);
                queue_y.push(y + 1);
            }
        }
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};