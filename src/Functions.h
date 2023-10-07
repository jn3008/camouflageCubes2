#include <cmath>
#include "ofMain.h"
#include "Constants.h"

// Some useful global functions

//--------------------------------------------------------------
inline float ease(float p, float g)
{
    if (p < 0.5)
        return 0.5 * pow(2 * p, g);
    else
        return 1 - 0.5 * pow(2 * (1 - p), g);
}

//--------------------------------------------------------------
// constrain between 0 and 1
inline float c01(float x)
{
    return std::min(1.0f, std::max(0.0f, x));
}

//--------------------------------------------------------------
inline float sq(float x)
{
    return x * x;
}

//--------------------------------------------------------------
inline float pow_(float p, float g)
{
    return 1 - pow(1 - p, g);
}

//--------------------------------------------------------------
// custom elastic easing function
inline float elastic(float x) 
{
    return x == 0 ? 0 : x == 1 ? 1
                               : 1 + pow(2, -10 * x * 0.62) * cos(PI + x * (25 * 0.6)) * (1 - pow_(x, 1));
}

//--------------------------------------------------------------
// custom bounce easing function
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

//--------------------------------------------------------------
// map from isometric projection to 2D grid
// (bijection between hexagonal grid and square grid)
inline ofVec3f isom_to_grid(ofVec3f v)
{
    float C1 = cos(-PI * 0.25);
    float S12 = sin(-PI * 0.25) * sin(-special_constant);
    float C2 = cos(-special_constant);
    return ofVec3f(v.x / C1, (-S12 * v.x / C1 / C2 + v.y / C2));
}

//--------------------------------------------------------------
// rotate view to isometric projection
inline ofVec3f isometric(ofVec3f v)
{
    return v.rotateRad(PI * 0.25, ofVec3f(0, 1, 0)).rotateRad(-special_constant, ofVec3f(1, 0, 0));
}

//--------------------------------------------------------------
// inverse of isometric function above
inline ofVec3f inv_isometric(ofVec3f v)
{
    return v.rotateRad(special_constant, ofVec3f(1, 0, 0)).rotateRad(-PI * 0.25, ofVec3f(0, 1, 0));
}

//--------------------------------------------------------------
// scale everything up to be visible and 'isometric'
inline ofVec3f global_transform(ofVec3f v)
{
    return isometric(v * global_scale);
}

//--------------------------------------------------------------
// make everything scroll (shift vertically continuously)
inline ofVec3f scrolling(ofVec3f v, float t)
{
    v.y += global_transform(ofVec3f(0, scroll_amt * t)).y;
    return v;
}

//--------------------------------------------------------------
// simple class to store a pair of integers
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

//--------------------------------------------------------------
// take a regular 3D point, use 'isometric',
// then 'isom_to_grid', and 'round'
// this converts a 3d point to the coordinates of
// the closest point in the isometric projection
// theoretically for an input in Z^3 (integer), 
// the output should be in Z^2 
inline Pair get_grid_coords(ofVec3f v)
{
    ofVec3f v_ = isom_to_grid(isometric(v));
    return Pair(round(v_.x), round(v_.y));
}

//--------------------------------------------------------------
// encode information about a type of movement
// of a cube from a pair of integers to a single
// integer. since 'axis' is bounded from above
// we can store it in the first few bits,
// the 'amount' is supposedly is unbounded in Z,
// we generally don't go beyond [-2, 2] but we could
// for example 'type' could be the integer that
// signifies a rotation about an edge of the cube in
// the x-axis, while amount tells us how many
// rotations of HALF_PI.
inline int get_mvm_id(int mvm_type, int amount)
{
    int ret = abs(amount) << 5;
    ret += mvm_type;
    ret *= amount < 0 ? -1 : 1;

    return ret;
}

//--------------------------------------------------------------
// decode movmement information, inverse of 
// get_mvm_id function above
inline Pair get_mvm_info(int movement)
{
    int mask_type = 0b11111;
    int mvm_type = abs(movement) & mask_type;
    int sign = movement < 0 ? -1 : 1;
    int amount = sign * (abs(movement) >> 5);

    return Pair(mvm_type, amount);
}