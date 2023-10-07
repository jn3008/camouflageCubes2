#pragma once

#include <vector>
#include <cmath>
#include "Constants.h"
#include "Occupation.h"

//--------------------------------------------------------------
// this is the inverse of isom_to_grid in Functions.h
inline ofVec3f grid_to_isom(ofVec3f v)
{
    float C1 = cos(-PI * 0.25);
    float S12 = sin(-PI * 0.25) * sin(-special_constant);
    float C2 = cos(-special_constant);
    return ofVec3f(C1 * v.x, +S12 * v.x + C2 * v.y);
}

class ofApp;

class CubePath;

class Grid
{
public:
    int rad, npoints;
    std::vector<Occupation> occupations;
    ofIcoSpherePrimitive sphere;
    std::vector<int> row_length_cum;
    std::vector<CubePath *> cubePaths;
    ofApp *parent;

public:
    Grid(ofApp *parent_ = nullptr, int rad_ = 0)
    {
        parent = parent_;
        rad = rad_;

        for (int i = -rad; i < rad + 1; i++)
            for (int j = -rad; j < rad + 1; j++)
                if (in_grid(i, j))
                    occupations.push_back(Occupation());

        sphere = ofIcoSpherePrimitive();
        sphere.setRadius(3);
        sphere.setResolution(2);

        npoints = occupations.size();

        // the rest of the constructor here is for efficiency
        // it helps us pre-calculate a bijection between the
        // hexagon-shaped grid of the background with a 1-dim
        // array
        //
        // for example, suppose rad = 3
        // 0 for !in_grid(i,j)
        // 1 for in_grid(i,j)
        //
        // 0 0 0 1 1 1 1    // row_length = 4 = 2*3 + 1 - abs(0-3) // row_length_cum = 0
        // 0 0 1 1 1 1 1    // row_length = 5 = 2*3 + 1 - abs(1-3) // row_length_cum = 4
        // 0 1 1 1 1 1 1    // row_length = 6 = 2*3 + 1 - abs(2-3) // row_length_cum = 9
        // 1 1 1 1 1 1 1    // row_length = 7 = 2*3 + 1 - abs(3-3) // row_length_cum = 15
        // 1 1 1 1 1 1 0    // row_length = 6 = 2*3 + 1 - abs(4-3) // row_length_cum = 22
        // 1 1 1 1 1 0 0    // row_length = 5 = 2*3 + 1 - abs(5-3) // row_length_cum = 28
        // 1 1 1 1 0 0 0    // row_length = 4 = 2*3 + 1 - abs(6-3) // row_length_cum = 33
        // so npoints = 4+5+6+7+6+5+4
        //
        // when using grid_to_isom this will appear as a regular
        // hexagon.

        // number of background dots on each row
        std::vector<int> row_length(2 * rad + 1);
        for (int i = 0; i < 2 * rad + 1; i++)
            row_length[i] = 2 * rad + 1 - abs(i - rad);

        // cumulative count of dots on each row
        row_length_cum = std::vector<int>(2 * rad + 1);
        for (int i = 0; i < 2 * rad + 1; i++)
        {
            row_length_cum[i] = 0;
            for (int j = 0; j < i; j++)
                row_length_cum[i] += row_length[j];
        }
    };

    //--------------------------------------------------------------
    // check whether bg dot with coordinates i,j is in the hexagon
    bool in_grid(int i, int j)
    {
        return -j < 1 + i + rad &&
               (j - rad - 1) < -i &&
               std::min(i, j) > -rad - 1 &&
               std::max(i, j) < rad + 1;
    }

    //--------------------------------------------------------------
    // check whether a cubepath is occupying
    // the dot at (i,j) at time q
    bool isOccupied(int i, int j, float q)
    {
        return get_occupation(i, j).contains(q);
    }

    //--------------------------------------------------------------
    // add occupation to the dot at (i,j) for time interval 'interval'
    void add_interval(int i, int j, Interval interval)
    {
        if (in_grid(i, j))
        {
            Occupation &temp_occupation = get_occupation(i, j);
            temp_occupation.intervals.push_back(interval);
        }
    }

    //--------------------------------------------------------------
    // get the Occupation object of the dot (i,j) which is
    // essentially a list of intervals the mark when it is occupied
    Occupation &get_occupation(int i, int j)
    {
        int idx = co_to_idx(i, j);
        if (idx < 0 || idx >= npoints)
        {
            ofLog() << "get_occupation idx out of range [0, " << npoints << "]: " << idx;
        }
        return occupations[idx];
    }

    //--------------------------------------------------------------
    // map from grid coordinate (i,j) to the 1d array described
    // in the constructor
    int co_to_idx(int i, int j)
    {
        // assume co is centered about (0, 0)
        if (!in_grid(i, j)) // check input is well-defined
            return -1;
        i += rad;
        j += rad;

        return i + row_length_cum[j];
    }

    //--------------------------------------------------------------
    // inverse of co_to_idx
    std::array<int, 2> idx_to_co(int idx)
    {
        if (idx < 0 || idx > npoints - 1) // check input is well-defined
            return std::array<int, 2>{0, 0};

        int j = std::distance(row_length_cum.begin(),
                              std::upper_bound(row_length_cum.begin(), row_length_cum.end(), idx)) -
                1;
        int i = idx - row_length_cum[j];
        i += max(0, j - rad);
        return std::array<int, 2>{rad - i, j - rad};
    }

    //--------------------------------------------------------------
    int count_intervals()
    {
        int ret = 0;
        for (Occupation occupation : occupations)
            ret += occupation.intervals.size();

        return ret;
    }

    void show();
};