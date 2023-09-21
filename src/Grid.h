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
    std::vector<ofIcoSpherePrimitive> spheres;
    std::vector<int> row_length_cum;
    std::vector<CubePath*> cubePaths;
    ofApp* parent;

public:
    Grid(ofApp* parent_ = nullptr, int rad_ = 0)
    {   
        // std::cout << "grid creation" << std::endl;
        ofLog() << "grid creation at: " << this << ", parent: " << parent_ << "rad: "<< rad <<std::endl;
        parent = parent_;
        rad = rad_;
        // occupations = std::vector<Occupation>();
        for (int i = -rad; i < rad + 1; i++)
        {
            for (int j = -rad; j < rad + 1; j++)
            {
                if (in_grid(i, j))
                {
                    occupations.push_back(Occupation());

                    ofIcoSpherePrimitive sphere = ofIcoSpherePrimitive();
                    sphere.setRadius(3);
                    sphere.setResolution(2);
                    spheres.push_back(sphere);
                }
            }
        }
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
            get_occupation(i, j).intervals.push_back(interval);
    }

    Occupation get_occupation(int i, int j)
    {
        return occupations[co_to_idx(i, j)];
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
        if (idx < 0 || idx > npoints-1)
            return std::array<int, 2>{0,0};
            
        int j = std::distance(row_length_cum.begin(), 
            std::upper_bound(row_length_cum.begin(), row_length_cum.end(), idx)) - 1;
        int i = idx-row_length_cum[j];
        i += max(0, j-rad);
        return std::array<int, 2>{rad-i,j-rad};
    }

    void show() {
        ofSetColor(255);
        ofFill();
        for (int i = 0; i < npoints; i++) {
            std::array<int, 2> co = idx_to_co(i);
            ofVec3f pos = grid_to_isom(ofVec3f(co[0], co[1], 0));
            pos *= global_scale;
            spheres[i].setPosition(pos.x, pos.y, pos.z);
            spheres[i].draw();
        }
    }
};