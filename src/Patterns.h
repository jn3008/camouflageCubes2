#pragma once

class Grid;

#include "Patterns.h"
#include <cmath>
#include "Functions.h"
#include "ofMain.h"
#include "CubePath.h"

//--------------------------------------------------------------
// check whether a dot with grid coords (i,j) is in the
// hexagon of radius rad
bool in_grid_rad(int i, int j, int rad)
{
    return -j < 1 + i + rad &&
           (j - rad - 1) < -i &&
           min(i, j) > -rad - 1 &&
           max(i, j) < rad + 1;
}

//--------------------------------------------------------------
// check whether a 3d is in the hexagon of radius rad
// after rotating to isometric projection and morphing
// to square grid
bool in_grid_from_pos(ofVec3f v, int rad)
{
    Pair co = get_grid_coords(ofVec3f(v));
    return in_grid_rad(co.first, co.second, rad);
}

//--------------------------------------------------------------
// returns random integer x with abs(x) <= mag
int randintmag(int mag)
{
    return round(ofRandom(-1, 1) * mag);
}

//--------------------------------------------------------------
// finds random point in hexagon of radius rad and
// transforms to 3d point in non-isometric projection
ofVec3f random_pos(int rad)
{
    int x = randintmag(rad);
    int y = randintmag(rad);
    while (!in_grid_rad(x, y, rad))
    {
        x = randintmag(rad);
        y = randintmag(rad);
    }
    return inv_isometric(grid_to_isom(ofVec3f(x, y, 0)));
}

//--------------------------------------------------------------
// fill grid with unsquish movements
// with a given time offset and axis
inline void submit_unsquish_wall(Grid *gridptr, float offset, int mvm_type)
{
    int ss = bg_rad;

    float tries = 1e5;
    for (int i = 0; i < tries; i++)
    {
        int size = floor(ofRandom(2, 5));
        int info = 1;

        int x = round(ofRandom(-20, 20));
        int y = round(ofRandom(-20, 20));

        ofVec3f startPosition = ofVec3f(x, y, 0);
        startPosition.y -= round(scroll_amt * offset);
        if (!in_grid_from_pos(startPosition, ss))
            continue;

        float time_offset = fmod(startPosition.length() * 0.01 + offset, 1);
        float duration = 0.15 + ofRandom(-1, 1) * 0.03;
        CubePath(gridptr, size, std::vector<int>{get_mvm_id(mvm_type, info)}, startPosition, time_offset, duration).submit();
    }
}

//--------------------------------------------------------------
// fill grid with roll movements with a given time offset 
// and information about type of roll
inline void sumbit_rolls(Grid *gridptr, float offset, int roll_type, int rotation_amt)
{
    float tries = 1e5;
    for (int i = 0; i < tries; i++)
    {
        ofVec3f start_pos = random_pos(bg_rad);

        float time_offset = offset + ofRandom(0, 1) * 0.15;
        float duration = 0.15 + ofRandom(-1, 1) * 0.05;
        int size = round(ofRandom(2, 4));

        CubePath(gridptr, size, std::vector<int>{get_mvm_id(roll_type, rotation_amt)}, start_pos, time_offset, duration).submit();
    }
}

//--------------------------------------------------------------
// fill grid with random cube paths
inline void submit_randoms(Grid *gridptr)
{
    float tries = 3e6;
    for (int i = 0; i < tries; i++)
    {
        int size = round(ofRandom(2, 4));
        int mvm_type = round(ofRandom(11));
        int info = 0;
        if (mvm_type == 8 || mvm_type == 2 || mvm_type == 6)
            continue;
        else if (mvm_type == 0 || mvm_type == 4 || mvm_type == 1 || mvm_type == 5 || mvm_type == 10)
            info = 1;
        else
            info = -1;

        float time_offset = ofRandom(0, 1);
        float duration = ofMap(size, 1, 4, 0.08, 0.2) + ofRandom(-1, 1) * 0.04;
        ofVec3f startPosition = random_pos(bg_rad);
        CubePath(gridptr, size, std::vector<int>{get_mvm_id(mvm_type, info)}, startPosition, time_offset, duration).submit();
    }
}