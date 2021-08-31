#pragma once

#include <vector>
#include "ofMain.h"

#define MAXSIZE 10000

struct posInfo
{
	int x;
	int y;
	posInfo() {};
	posInfo(int _x, int _y) : x(_x), y(_y) {};
};

struct lineInfo
{
	posInfo first;
	posInfo second;
	lineInfo() {};
	lineInfo(posInfo _first, posInfo _second) : first(_first), second(_second) {};
};

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // flag variables
    int draw_flag;
    int load_flag;
    
    // Line segment and dot related variables
    int num_of_line, num_of_dot;
    float dot_diameter;
    
	vector<lineInfo> linez;
	vector<posInfo> dotz;
	int redindex = 0;

    /* WaterFall-related member functions */
	bool isWater = false;
	vector<lineInfo> waterLinez;
	posInfo dowaterfall(posInfo currentPos);
	int ccw(posInfo first, posInfo second, posInfo third);
	bool lineCrossCheck(lineInfo first, lineInfo second);
	int findFallingYpos(lineInfo line, posInfo dot);
	posInfo findSlopedPos(lineInfo line, posInfo dot);
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void initializeWaterLines(); // 2nd week portion.
		
};