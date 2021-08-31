#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		
		//�ϳ��� ž�� �׷��ִ� flag�����̴�.
		int draw_flag = 0;
		//�ϳ���ž�� ����� ���� �����̴�.
		int make_flag = 0;
		//��� ������ ���������� �̵��ߴ��� Ȯ���ϴ� �����̴�.
		int count_flag = 0;
		//���� �� �� ����� �̵��� �ߴ��� Ȯ���ϴ� �����̴�.
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
