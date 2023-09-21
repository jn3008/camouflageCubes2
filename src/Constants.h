#pragma once

#include "ofMain.h"

#include <array>

constexpr float special_constant = 0.6154797086703873410674645891239936;
constexpr float global_scale = 30;
constexpr int dotSize = 5;
constexpr int scroll_amt = 5;
constexpr int bg_rad = 8;

constexpr std::array<std::array<float, 3>, 8> cube_verts = // float cube_verts[8][3] =
    {{
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, 0.5, 0.5},
        {-0.5, -0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, 0.5, 0.5}}};
// = new PVector[8];
// for (int i = 0; i < 2; i++)
// 	for (int j = 0; j < 2; j++)
// 		for (int k = 0; k < 2; k++)
// 			cube_verts[k * 4 + j * 2 + i] = new PVector(i - 0.5,j - 0.5,k - 0.5);
constexpr std::array<std::array<int, 4>, 6> cube_faces = // int cube_faces[6][4] = {
    {{
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {2, 3, 6, 7},
        {0, 1, 4, 5},
        {1, 3, 5, 7},
        {0, 2, 4, 6}}};
constexpr int pivots[12] = {
    6, 2, 4, 0,
    6, 7, 4, 5,
    6, 7, 2, 3};