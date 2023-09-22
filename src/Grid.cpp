// #pragma once

#include "Grid.h"

#include "Functions.h"
#include "ofApp.h"

void Grid::show()
{
    ofSetColor(255);
    ofFill();
    for (int i = 0; i < npoints; i++)
    {
        std::array<int, 2> co = idx_to_co(i);
        ofVec3f pos = grid_to_isom(ofVec3f(co[0], co[1], 0));

        pos *= global_scale;
        pos += scrolling(ofVec3f(), parent->t);

        sphere.setPosition(pos.x, pos.y, -150);
        sphere.draw();
    }
}