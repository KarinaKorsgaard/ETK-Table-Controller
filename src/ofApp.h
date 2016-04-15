#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxClipboard.h"

#define PORT 9000
#define HOST "localhost"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    bool showResutlsFuckCrap;
    bool updateXml = false;
    bool up = false;
    bool sendStartScene = false;
    ofxPanel gui;
    ofxButton allOff;
    ofxToggle startScene;
    int xP;
    int prevScene;
    bool sendAll = false; 
    int yP;
    ofxToggle showResults;
    ofxToggle nextQuestion;
    ofxToggle nextStage;
    ofxToggle pause;
    int prevPause;
    bool sendDisable; 
    int prewShowResults;
    ofxToggle clearPoint;
    int prevStartScene ;
    ofxButton updateScene;
    vector<string>sceneNames;
    int prevNextQuestion;
    int stageCounter;
    int scene;
    int countDown = 0;
    ofxToggle showChart;
    ofxToggle tableMode;
    ofxToggle averaged;
    ofxToggle grid;
    ofxButton simulator;
    int prevGrid; 
    ofxSlider<int>slider;
    int prevSlider; 
    ofxOscReceiver receiver;
    ofxOscSender sender;
    int prevStageCounter;
    vector<bool>hasbeenseen;
    string dataFolder;
    vector<bool>disable;
    
    vector<bool>whoAreConnected;
    vector<string>tablenames;
    vector<int>howmanystages;
    int prevTableToggle;
    int prevShowChart;
    int prevAveraged; 
    int guiX;
    int guiY;
    ofPoint guiP;
    
    ofxXmlSettings xml;

};
