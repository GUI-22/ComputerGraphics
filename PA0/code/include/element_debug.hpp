#pragma once

#include <image.hpp>
#include <algorithm>
#include <stack>
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
            // printf("x1=%d, y1=%d, x2=%d, y2=%d\n\n", x1, y1, x2, y2);
            if (y1 < y2) {
                long long dx = x2 - x1, dy = y2 - y1;
                if (dy < dx) {
                    long long e = -dx, y = y1;
                    for (long long x = x1; x <= x2; x++) {
                        img.SetPixel(x, y, color);
                        e += 2 * dy;
                        if (e > 0) {
                            y += 1;
                            e -= 2 * dx;
                        }
                    }
                }
                else {
                    std::swap(x1, y1);
                    std::swap(x2, y2);
                    long long dx = x2 - x1, dy = y2 - y1;
                    long long e = -dx, y = y1;
                    for (long long x = x1; x <= x2; x++) {
                        img.SetPixel(y, x, color);
                        e += 2 * dy;
                        if (e > 0) {
                            y += 1;
                            e -= 2 * dx;
                        }
                    }
                }
            }
            else {
                y2 = 2 * y1 - y2;
                long long dx = x2 - x1, dy = y2 - y1;
                if (dy < dx) {
                    long long e = -dx, y = y1;
                    for (long long x = x1; x <= x2; x++) {
                        img.SetPixel(x, 2 * y1 - y, color);
                        e += 2 * dy;
                        if (e > 0) {
                            y += 1;
                            e -= 2 * dx;
                        }
                    }
                }
                else {
                    std::swap(x1, y1);
                    std::swap(x2, y2);
                    long long dx = x2 - x1, dy = y2 - y1;
                    long long e = -dx, y = y1;
                    for (long long x = x1; x <= x2; x++) {
                        img.SetPixel(y, 2 * x1 - x, color);
                        e += 2 * dy;
                        if (e > 0) {
                            y += 1;
                            e -= 2 * dx;
                        }
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
    inline void Check_SetPixel(Image &img, int x, int y, Vector3f color) {
        if (x >= 0 && x < img.Width() && y >= 0 && y < img.Height()) {
            img.SetPixel(x, y, color);
        }
    }
    void set_circle_pixel(Image &img, int x, int y, Vector3f color, int cx, int cy) {
        Check_SetPixel(img, x + cx, y + cy, color);
        Check_SetPixel(img, y + cx, x + cy, color);
        Check_SetPixel(img, -x + cx, y + cy, color);
        Check_SetPixel(img, y + cx, -x + cy, color);
        Check_SetPixel(img, x + cx, -y + cy, color);
        Check_SetPixel(img, -y + cx, x + cy, color);
        Check_SetPixel(img, -x + cx, -y + cy, color);
        Check_SetPixel(img, -y + cx, -x + cy, color);
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
    // void draw_sub(Image &img, int x, int y, Vector3f color, Vector3f old_color) {
    //     if (x >= 0 && x < img.Width() && y >= 0 && y < img.Height() && img.GetPixel(x, y) == old_color) {
    //         img.SetPixel(x, y, color);
    //         draw_sub(img, x-1, y, color, old_color);
    //         draw_sub(img, x, y-1, color, old_color);
    //         draw_sub(img, x+1, y, color, old_color);
    //         draw_sub(img, x, y+1, color, old_color);
    //     }
    // }
    inline bool is_available(Image &img, int x, int y){
        return x >= 0 && y >= 0 && x < img.Width() && y < img.Height();
    }
    void draw(Image &img) override {
        // TODO: Flood fill
        // printf("w:%d, h:%d\n", img.Width(), img.Height());
        // exit(12);
        long long max_long = 0, len = 1;
        std::stack<int> stack_x;
        std::stack<int> stack_y;
        // std::queue<int> stack_x;
        // std::queue<int> stack_y;
        stack_x.push(cx);
        stack_y.push(cy);
        Vector3f old_color = img.GetPixel(cx, cy);
        if (old_color == color) {
            return;
        }
        img.SetPixel(cx, cy, color);
        while(!stack_x.empty() && max_long < 512 * 512) {
            if (len > max_long) {
                max_long = len;
                // printf("max_len %d\n", max_long);
            }
            int x = stack_x.top(), y = stack_y.top();
            // int x = stack_x.front(), y = stack_y.front();
            stack_x.pop();
            stack_y.pop();
            len -= 1;
            // img.SetPixel(x, y, color);
            if (is_available(img, x - 1, y) && img.GetPixel(x - 1, y) == old_color) {
                stack_x.push(x - 1);
                stack_y.push(y);
                img.SetPixel(x - 1, y, color);
                len += 1;
            }
            if (is_available(img, x + 1, y) && img.GetPixel(x + 1, y) == old_color) {
                stack_x.push(x + 1);
                stack_y.push(y);
                img.SetPixel(x + 1, y, color);
                len += 1;
            }
            if (is_available(img, x, y - 1) && img.GetPixel(x, y - 1) == old_color) {
                stack_x.push(x);
                stack_y.push(y - 1);
                img.SetPixel(x, y - 1, color);
                len += 1;
            }
            if (is_available(img, x, y + 1) && img.GetPixel(x, y + 1) == old_color) {
                stack_x.push(x);
                stack_y.push(y + 1);
                img.SetPixel(x, y + 1, color);
                len += 1;
            }
        }
        // img.SetPixel(cx, cy, old_color);
        // if (color != old_color) {
        //     draw_sub(img, cx, cy, color, old_color);
        // }
        printf("max_len %d\n", max_long);
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};




