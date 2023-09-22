#include <cmath>
#include "ofMain.h"
// #include "ofApp.h"
#include "Constants.h"

inline float ease(float p, float g)
{
    if (p < 0.5)
        return 0.5 * pow(2 * p, g);
    else
        return 1 - 0.5 * pow(2 * (1 - p), g);
}
inline float c01(float x)
{
    return std::min(1.0f, std::max(0.0f, x));
}
inline float sq(float x)
{
    return x * x;
}
inline float pow_(float p, float g)
{
    return 1 - pow(1 - p, g);
}

inline float elastic(float x)
{
    return x == 0 ? 0 : x == 1 ? 1
                               : 1 + pow(2, -10 * x * 0.62) * cos(PI + x * (25 * 0.6)) * (1 - pow_(x, 1));
}

inline float bounce(float x)
{
    int n = 3;
    float intervals[n] = {0, 0.55, 0.85};
    float heights[n] = {1, 0.18, 0.035};

    for (int i = 0; i < n; i++)
        if (x < intervals[i])
            return 1 - heights[i - 1] * (1 - sq(ofMap(x, intervals[i - 1], intervals[i], i == 1 ? 0 : -1, 1)));
    return 1 - (1 - c01(sq(ofMap(x, intervals[n - 1], 1, -1, 1)))) * heights[n - 1];
}

inline ofVec3f isom_to_grid(ofVec3f v)
{
    float C1 = cos(-PI * 0.25);
    float S12 = sin(-PI * 0.25) * sin(-special_constant);
    float C2 = cos(-special_constant);
    return ofVec3f(v.x / C1, (-S12 * v.x / C1 / C2 + v.y / C2));
}
// ofVec3f grid_to_isom(ofVec3f v)
// {
//     float C1 = cos(-PI * 0.25);
//     float S12 = sin(-PI * 0.25) * sin(-ofApp::special_constant);
//     float C2 = cos(-ofApp::special_constant);
//     return ofVec3f(C1 * v.x, +S12 * v.x + C2 * v.y);
// }
inline ofVec3f transform(ofVec3f v)
{
    return v.rotateRad(PI * 0.25, ofVec3f(0, 1, 0)).rotateRad(-special_constant, ofVec3f(1, 0, 0));
}
inline ofVec3f untransform(ofVec3f v)
{
    return v.rotateRad(special_constant, ofVec3f(1, 0, 0)).rotateRad(-PI * 0.25, ofVec3f(0, 1, 0));
}
// inline ofVec3f transform(ofVec3f v, float test)
// {
//     return v.rotateRad(PI * 0.25*test, ofVec3f(0, 1, 0)).rotateRad(-special_constant*test, ofVec3f(1, 0, 0));
// }
// inline ofVec3f untransform(ofVec3f v, float test)
// {
//     return v.rotateRad(special_constant*test, ofVec3f(1, 0, 0)).rotateRad(-PI * 0.25*test, ofVec3f(0, 1, 0));
// }

inline ofVec3f global_transform(ofVec3f v)
{
    return transform(v * global_scale);
}
inline ofVec3f scrolling(ofVec3f v, float t)
{
    // v.y += scroll_amt * t;
    v += global_transform(ofVec3f(0, scroll_amt * t));
    return v;
}

class Pair
{
public:
    int first, second;

public:
    Pair(int a = 0, int b = 0)
    {
        first = a;
        second = b;
    };
    int get(int idx)
    {
        if (idx % 2 == 0)
            return first;
        else
            return second;
    }
};

inline Pair get_grid_coords(ofVec3f v)
{
    ofVec3f v_ = isom_to_grid(transform(v));
    return Pair(round(v_.x), round(v_.y));
}

inline int mvm(int axis, int rotation_amount)
{
    int ret = abs(rotation_amount) << 5;
    ret += axis;
    ret *= rotation_amount < 0 ? -1 : 1;

    return ret;
}

inline Pair get_info(int movement)
{

    // ofLog() << "inline Pair get_info(int movement), movement: " << movement;
    int mask_axis = 0b11111;
    int axis = abs(movement) & mask_axis;
    int sign = movement < 0 ? -1 : 1;
    int rotation_amount = sign * (abs(movement) >> 5);

    return Pair(axis, rotation_amount);
}