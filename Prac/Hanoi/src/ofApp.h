#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		
		//하노이 탑을 그려주는 flag변수이다.
		int draw_flag = 0;
		//하노이탑을 만들기 위한 변수이다.
		int make_flag = 0;
		//모든 원판이 도착점으로 이동했는지 확인하는 변수이다.
		int count_flag = 0;
		//종료 후 총 몇번의 이동을 했는지 확인하는 변수이다.
		int count_num = 0;

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
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
};
