#pragma once

#include "ofMain.h"

#include <array>

//--------------------------------------------------------------
// angle to rotate x-axis for isometric projection
constexpr float special_constant = 0.6154797086703873410674645891239936;
constexpr float global_scale = 30;
constexpr int dotSize = 5;
constexpr int scroll_amt = 5;
constexpr int bg_rad = 30; // dot radius of background hexagon of dots

//--------------------------------------------------------------
// 8 vertices of the cube
constexpr std::array<std::array<float, 3>, 8> cube_verts =
    {{{-0.5, -0.5, -0.5},
      {-0.5, -0.5, 0.5},
      {-0.5, 0.5, -0.5},
      {-0.5, 0.5, 0.5},
      {0.5, -0.5, -0.5},
      {0.5, -0.5, 0.5},
      {0.5, 0.5, -0.5},
      {0.5, 0.5, 0.5}}};

//--------------------------------------------------------------
// indices of vertices that make each face of the cube
constexpr std::array<std::array<int, 4>, 6> cube_faces =
    {{{0, 1, 2, 3},
      {4, 5, 6, 7},
      {2, 3, 6, 7},
      {0, 1, 4, 5},
      {1, 3, 5, 7},
      {0, 2, 4, 6}}};

//--------------------------------------------------------------
// indices of vertices for which the cube uses as a pivot
// to rotate when rolling.
// the first four are for rotation about x-axis, second
// four for z-axis, last four for y-axis.
constexpr int pivots[12] = {
    6, 2, 4, 0,
    6, 7, 4, 5,
    6, 7, 2, 3};