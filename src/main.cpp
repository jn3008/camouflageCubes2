#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// ofLogToConsole();
	// std::cout.flush();
	// printf("test");

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(720, 720);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}