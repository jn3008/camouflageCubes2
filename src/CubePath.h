#pragma once
#include "Grid.h"
#include "ofMain.h"
#include "Occupation.h"

class CubePath
{
    public:
        int cubeSize, numMovements;
        std::vector<int> movements;
        std::vector<ofVec3f> positions;
        float timeOffset, duration;
        Interval interval;
        Grid* g;
    public:
        CubePath(Grid* g_, int cubeSize_, std::vector<int> movements_, ofVec3f startPosition,
             float timeOffset_, float duration_);
        bool in_grid(int i, int j);
        Interval get_time_interval(int move_idx);
        void submit();
        void add_to_paths();
        bool does_cubepath_overlap();
        void show();
        void show(float q);
        void show(int move_idx, float q);
        ofVec3f show(ofVec3f v, int move_idx, float q);
        ofVec3f action(ofVec3f v, int move_idx, float q);
        ofVec3f squish(ofVec3f v, int move_idx, float q);
        ofVec3f roll(ofVec3f v, int move_idx, float q);
};