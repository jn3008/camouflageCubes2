#pragma once

// class Grid;

#include <vector>
#include <cmath>
#include "Constants.h"
#include "Occupation.h"

// ofVec3f grid_to_isom();
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

        std::vector<int> row_length(2 * rad + 1);
        for (int i = 0; i < 2 * rad + 1; i++)
            row_length[i] = 2 * rad + 1 - abs(i - rad);

        row_length_cum = std::vector<int>(2 * rad + 1);
        for (int i = 0; i < 2 * rad + 1; i++)
        {
            row_length_cum[i] = 0;
            for (int j = 0; j < i; j++)
                row_length_cum[i] += row_length[j];
        }
    };

    bool in_grid(int i, int j)
    {
        return -j < 1 + i + rad && (j - rad - 1) < -i && std::min(i, j) > -rad - 1 && std::max(i, j) < rad + 1;
    }

    bool isOccupied(int i, int j, float q)
    {
        return get_occupation(i, j).contains(q);
    }

    void add_interval(int i, int j, Interval interval)
    {
        if (in_grid(i, j))
        {
            Occupation &temp_occupation = get_occupation(i, j);
            temp_occupation.intervals.push_back(interval);
        }
    }

    Occupation &get_occupation(int i, int j)
    {
        int idx = co_to_idx(i, j);
        if (idx < 0 || idx >= npoints)
        {
            ofLog() << "get_occupation idx out of range [0, " << npoints << "]: " << idx;
        }
        return occupations[idx];
    }

    int co_to_idx(int i, int j)
    {
        // assume co is centered about (0, 0)
        if (!in_grid(i, j))
            return -1;
        i += rad;
        j += rad;

        return i + row_length_cum[j];
    }

    std::array<int, 2> idx_to_co(int idx)
    {
        if (idx < 0 || idx > npoints - 1)
            return std::array<int, 2>{0, 0};

        int j = std::distance(row_length_cum.begin(),
                              std::upper_bound(row_length_cum.begin(), row_length_cum.end(), idx)) -
                1;
        int i = idx - row_length_cum[j];
        i += max(0, j - rad);
        return std::array<int, 2>{rad - i, j - rad};
    }

    int countintervals()
    {
        int ret = 0;
        for (Occupation occupation : occupations)
            ret += occupation.intervals.size();
            
        return ret;
    }

    void show();
};