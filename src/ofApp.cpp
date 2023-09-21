#include "ofApp.h"
#include "CubePath.h"
#include "Functions.h"
#include <iostream>

// Grid* g_point;
//--------------------------------------------------------------
void ofApp::setup()
{
    std::cout << "setup" << std::endl;
    
    ofSetRandomSeed(0);
    
    grid = Grid(this, bg_rad);
    
    CubePath cubePath0 = CubePath(&grid, 2, std::vector<int>{mvm(4, 1), mvm(0, 1)}, ofVec3f(), 0, 0.99);
    cubePath0.submit();
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    bool preview = false;
    if (preview) {
        t += 0.01;
        t = fmod(t, 1);
    } else {
        t = ofGetMouseX()*1.0/ofGetWidth();
    } 
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofEnableAntiAliasing();
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -100, 1000);
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofBackground(0);

    if (ofGetFrameNum() % 200 == 0)
        std::cout << ofGetFrameNum() << std::endl;


    grid.show();

    for (CubePath *cubePath : grid.cubePaths)
    {

        cubePath->show();
    }

    ofSetColor(255, 0, 0, 255);
    ofFill();
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