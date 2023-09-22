// #include "Grid.h"
#include <vector>
#include <cmath>

#include "CubePath.h"
#include "ofMain.h"
#include "Functions.h"
#include "ofApp.h"

//--------------------------------------------------------------
CubePath::CubePath(Grid *g_, int cubeSize_, std::vector<int> movements_,
                   ofVec3f startPosition, float timeOffset_, float duration_)
{
    g = g_;
    cubeSize = cubeSize_;
    movements = movements_;
    numMovements = movements.size();
    timeOffset = timeOffset_;
    duration = duration_;
    // easingType = easingType_;
    interval = Interval(timeOffset, fmod(timeOffset + duration, 1));
    positions = std::vector<ofVec3f>(numMovements + 1);
    positions[0] = startPosition;
    for (int i = 1; i < numMovements + 1; i++)
    {
        int move_type = get_info(movements[i - 1]).first;
        if (move_type > 11 && move_type < 15) // squishing
            positions[i] = positions[i - 1];
        else
            positions[i] = (action(ofVec3f(0, 0, 0), i - 1, 1) * cubeSize) + positions[i - 1];
    }
}

//--------------------------------------------------------------
bool CubePath::in_grid(int i, int j)
{
    return -j < 1 + i + cubeSize &&
           (j - cubeSize - 1) < -i &&
           std::min(i, j) > -cubeSize - 1 &&
           std::max(i, j) < cubeSize + 1;
}

//--------------------------------------------------------------
Interval CubePath::get_time_interval(int move_idx)
{
    float t0 = fmod(move_idx * duration / numMovements + timeOffset, 1);
    float t1 = fmod((move_idx + 1) * duration / numMovements + timeOffset, 1);
    return Interval(t0, t1);
}

//--------------------------------------------------------------
void CubePath::submit()
{
    if (!does_cubepath_overlap())
        add_to_paths();
}

//--------------------------------------------------------------
void CubePath::add_to_paths()
{
    float eps = 1e-6;
    g->cubePaths.push_back(new CubePath(*this));

    for (int i = 0; i < numMovements + 1; i++)
    {
        Interval interval0 = get_time_interval(i);
        Interval interval1 = get_time_interval(i - 1);

        std::vector<Interval> intervals_to_add;
        if (i < numMovements)
            intervals_to_add.push_back(interval0);
        if (i > 0)
            intervals_to_add.push_back(interval1);

        int mm = cubeSize;
        for (int x = -mm; x < mm + 1; x++)
        { // go through each 2d iso view dot that a cube occupies
            for (int y = -mm; y < mm + 1; y++)
            {
                if (!in_grid(x, y))
                    continue;
                Pair co = get_grid_coords(positions[i]);
                int xx = co.get(0) + x;
                int yy = co.get(1) + y;

                for (Interval add_interval : intervals_to_add)
                {
                    if (add_interval.contains(0))
                    { // interval crosses the loop point
                        if (g->in_grid(xx, yy))
                            g->add_interval(xx, yy, add_interval.set1(1 + eps));
                        if (g->in_grid(xx, yy + scroll_amt))
                            g->add_interval(xx, yy + scroll_amt, add_interval.set0(0 - eps));
                    }
                    else
                    {
                        if (g->in_grid(xx, yy))
                            g->add_interval(xx, yy, add_interval);
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
bool CubePath::does_cubepath_overlap()
{
    float eps = 1e-6;
    for (int i = 0; i < numMovements + 1; i++)
    {
        Interval interval0 = get_time_interval(i);
        Interval interval1 = get_time_interval(i - 1);

        std::vector<Interval> intervals_to_test;
        if (i < numMovements)
            intervals_to_test.push_back(interval0);
        if (i > 0)
            intervals_to_test.push_back(interval1);

        int mm = cubeSize;
        for (int x = -mm; x < mm + 1; x++)
        { // go through each 2d iso view dot that a cube occupies
            for (int y = -mm; y < mm + 1; y++)
            {
                if (!in_grid(x, y))
                    continue;
                Pair co = get_grid_coords(positions[i]);
                int xx = co.get(0) + x;
                int yy = co.get(1) + y;

                for (Interval test_interval : intervals_to_test)
                {
                    if (test_interval.contains(0))
                    { // interval crosses the loop point
                        if (g->in_grid(xx, yy))
                            if (g->get_occupation(xx, yy).clashes(test_interval.set1(1 + eps)))
                                return true;
                        if (g->in_grid(xx, yy + scroll_amt))
                            if (g->get_occupation(xx, yy + scroll_amt).clashes(test_interval.set0(0 - eps)))
                                return true;
                    }
                    else
                    {
                        if (g->in_grid(xx, yy))
                            if (g->get_occupation(xx, yy).clashes(test_interval))
                                return true;
                    }
                }
            }
        }
    }

    return false;
}

//--------------------------------------------------------------
void CubePath::show()
{
    if (interval.contains(g->parent->t))
    {
        show(fmod(g->parent->t + 1 - timeOffset, 1) / duration * .999);
    }
}

//--------------------------------------------------------------
void CubePath::show(float q = 0)
{ // q in [0,1]

    float Q = q * numMovements;

    int type = get_info(movements[floor(Q)]).get(0);

    if (type > 11 && type < 15)
        show(floor(Q), elastic(fmod(Q, 1)));
    else if (type > 20 && type < 24)
        show(floor(Q), elastic(fmod(Q, 1)));
    else
        show(floor(Q), bounce(fmod(Q, 1)));
}

//--------------------------------------------------------------
void CubePath::show(int move_idx, float q)
{
    ofFill();

    float cube_verts_transformed[8][3];

    for (int i = 0; i < 8; i++)
    {
        ofVec3f v = show(ofVec3f(cube_verts[i][0], cube_verts[i][1], cube_verts[i][2]), move_idx, q);
        cube_verts_transformed[i][0] = v.x;
        cube_verts_transformed[i][1] = v.y;
        cube_verts_transformed[i][2] = v.z;
    }

    // draw cube faces
    ofSetColor(0);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            ofDrawTriangle(
                cube_verts_transformed[cube_faces[i][0 + j]][0],
                cube_verts_transformed[cube_faces[i][0 + j]][1],
                cube_verts_transformed[cube_faces[i][0 + j]][2],
                cube_verts_transformed[cube_faces[i][1 + j]][0],
                cube_verts_transformed[cube_faces[i][1 + j]][1],
                cube_verts_transformed[cube_faces[i][1 + j]][2],
                cube_verts_transformed[cube_faces[i][2 + j]][0],
                cube_verts_transformed[cube_faces[i][2 + j]][1],
                cube_verts_transformed[cube_faces[i][2 + j]][2]);


    // draw vertices
    ofSetColor(255);
    for (int i = 0; i < cubeSize + 1; i++)
    {
        for (int j = 0; j < cubeSize + 1; j++)
        {
            for (int k = 0; k < cubeSize + 1; k++)
            {
                if (min(i, min(j, k)) > 0 && max(i, max(j, k)) < cubeSize)
                    continue;
                ofVec3f v = show(ofVec3f(
                                     i * 1.0 / cubeSize - 0.5,
                                     j * 1.0 / cubeSize - 0.5,
                                     k * 1.0 / cubeSize - 0.5),
                                 move_idx, q);
                v.z += dotSize * 1.5;

                g->sphere.setPosition(v);
                g->sphere.draw();
            }
        }
    }
}

//--------------------------------------------------------------
ofVec3f CubePath::show(ofVec3f v, int move_idx, float q)
{
    // prepare the vertex or point to be shown
    v = action(v, move_idx, q);
    v *= cubeSize;

    if (interval.contains(0) && g->parent->t < 0.5)
        v.y += scroll_amt;

    v = global_transform(v);
    // v.z *= 0.3; // flatten // ofGetMouseX()*1.0/ofGetWidth();
    ofVec3f pos = global_transform(ofVec3f(positions[move_idx]));
    pos.z = 0;
    v += pos;
    v = scrolling(v, g->parent->t);
    return v;
}

//--------------------------------------------------------------
ofVec3f CubePath::action(ofVec3f v, int move_idx, float q)
{
    Pair info = get_info(movements[move_idx]);
    int move_type = info.get(0);
    if (move_type < 12) // rotate
        return roll(v, move_idx, q);
    else
        return squish(v, move_idx, q);
}

//--------------------------------------------------------------
ofVec3f CubePath::squish(ofVec3f v, int move_idx, float q)
{
    Pair info = get_info(movements[move_idx]);
    int squish_axis = info.get(0) - 12;
    int squish_amount = info.get(1); // if negative this means squish not stretch

    float mult = (squish_amount < 0 ? 1 - q : q) * abs(squish_amount);
    if (squish_axis == 0)
    {
        v.x -= 0.5;
        v.x *= mult;
        v.x += 0.5;
    }
    else if (squish_axis == 1)
    {
        v.z += 0.5;
        v.z *= mult;
        v.z -= 0.5;
    }
    else
    {
        v.y -= 0.5;
        v.y *= mult;
        v.y += 0.5;
    }
    return v;
}

//--------------------------------------------------------------
ofVec3f CubePath::roll(ofVec3f v, int move_idx, float q)
{
    Pair info = get_info(movements[move_idx]);
    int axis = info.get(0);
    int rotation_amount = info.get(1);

    std::array<float, 3> pivot_arr = {cube_verts[pivots[axis]]};
    ofVec3f pivot = ofVec3f(pivot_arr[0], pivot_arr[1], pivot_arr[2]);
    // v -= pivot;
    int xyz = axis / 4; // this is floor div
    if (xyz == 0)
        v.rotateRad(-rotation_amount * HALF_PI * q, pivot, ofVec3f(1, 0, 0));
    else if (xyz == 1)
        v.rotateRad(-rotation_amount * HALF_PI * q, pivot, ofVec3f(0, 0, 1));
    else
        v.rotateRad(-rotation_amount * HALF_PI * q, pivot, ofVec3f(0, 1, 0));

    // v -= pivot;
    return v;
}