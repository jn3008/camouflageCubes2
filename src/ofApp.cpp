#include <iostream>
#include "ofApp.h"
#include "Patterns.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    std::cout << "begin setup" << std::endl;
    
    ofSetRandomSeed(0);
    
    grid = Grid(this, bg_rad);
    
    // CubePath cubePath0 = CubePath(&grid, 2, std::vector<int>{mvm(4, 1), mvm(0, 1), mvm(13, 1)}, ofVec3f(), 0, 0.45);
    // cubePath0.submit();
    
    // CubePath cubePath1 = CubePath(&grid, 2, std::vector<int>{mvm(4, 1), mvm(0, 1), mvm(13, 1)}, ofVec3f(1, 0, 0), 0.1, 0.45);
    // cubePath1.submit();

    // submit_unsquish_wall(&grid, 0, 13);
    // submit_unsquish_wall(&grid, 0.5, 12);
    
    // sumbit_rolls(&grid, 0.20, 4, 1);
    // sumbit_rolls(&grid, 0.30, 10, 1);
    // sumbit_rolls(&grid, 0.70, 0, 1);
    // sumbit_rolls(&grid, 0.80, 9, -1);  
    // submit_randoms(&grid);
    
    submit_unsquish_wall(&grid, 0, 13);
    submit_unsquish_wall(&grid, 0.25, 12);
    submit_unsquish_wall(&grid, 0.5, 13);
    submit_unsquish_wall(&grid, 0.75, 12);


    std::cout << "end setup" << std::endl;
}

//--------------------------------------------------------------
void ofApp::update()
{
    bool preview = true;
    if (preview) {
        t = ofGetElapsedTimeMillis()*1.0/(20 * 350);
        t = fmod(t, 1);
    } else {
        t = ofGetMouseX()*1.0/ofGetWidth();
    } 
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // GL_SMOOTH
    ofEnableAntiAliasing(); ofEnableDepthTest();
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -300, 300);

    ofBackground(0);
    
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    if (ofGetMousePressed())
        ofRotateYRad(PI*2*ofGetMouseY()*1.0/ofGetHeight());

    grid.show();

    for (CubePath *cubePath : grid.cubePaths)
        cubePath->show();

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
    std::cout << "ofGetMouseX(), ofGetMouseY()" << std::endl;
    std::cout << ofGetMouseX() << ", " << ofGetMouseY() << std::endl;
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