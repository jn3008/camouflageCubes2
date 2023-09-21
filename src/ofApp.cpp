#include "ofApp.h"
#include "CubePath.h"
#include "Functions.h"
#include <iostream>

// Grid* g_point;
//--------------------------------------------------------------
void ofApp::setup()
{

    std::cout << "setup" << std::endl;
    // // ofVec3f v;
    ofSetRandomSeed(0);
    // ofSetFrameRate(4);

    // t = 0;
    // // Grid g_obj = Grid(this, bg_rad);
    // // g_point = &g_obj;
    // // ofDrawBitmapString("Hello World!", 100, 100);
    // ofLogToConsole();
    // ofLog() << "helloooo";
    // std::cerr << "yo" << std::endl;
    // Grid grid = Grid(this, bg_rad);
    // gridptr = &grid;
    grid = Grid(this, bg_rad);
    // //
    //     // ofLog() << "submitting cube now";
    //     std::cout << "before submit";
    //     // ofDrawBitmapString("before submit", 100, 100);
    CubePath cubePath0 = CubePath(&grid, 2, std::vector<int>{mvm(0, 1)}, ofVec3f(), 0, 0.6);
    cubePath0.submit();
    ofLog() << "&cubePath0: " << &cubePath0;

    std::cout << grid.cubePaths.size() << std::endl;
    std::cout << &grid << std::endl;

    for (CubePath *cubePath : grid.cubePaths)
    {
        ofLog() << "setup() cubepath address for each in  grid.cubePaths: " << cubePath;
        ofLog() << "        duration: " << cubePath->duration;
    }
    //     // ofDrawBitmapString("after submit", 100, 200);

    // CubePath(grid,2,std::vector<int>{mvm(0,1)},ofVec3f(),0, 0.6).add_to_paths();
}

//--------------------------------------------------------------
void ofApp::update()
{
    t += 0.01;

    t = fmod(t, 1);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -100, 1000);
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofBackground(0);

    if (ofGetFrameNum() % 200 == 0)
        std::cout << "yo" << std::endl;

    // ofDrawCircle(20*cos(t*PI*2), 20*sin(t*PI*2), 100);

    grid.show();
    if (ofGetFrameNum() % 200 == 0)
        std::cout << "yo2" << std::endl;

    // std::cout << "drawing grid\n";
    // ofLog() << "drawing";

    for (CubePath *cubePath : grid.cubePaths)
    {
        // ofLog() << "draw() cubepath address for each in  grid.cubePaths: " << cubePath;
        // ofLog() << "        duration: " << cubePath->duration;

        cubePath->show();

        // std::cout << "drawing grid" << cubePointer << std::endl;
    }

    ofSetColor(255, 0, 0, 255);
    ofFill();

    // ofDrawTriangle(
    //     30, -88.5884, 31.1818,
    //     39, -87.4066, 91.1701,
    //     30, -29.6, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}