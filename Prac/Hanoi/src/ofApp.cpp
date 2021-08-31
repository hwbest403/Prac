#include "ofApp.h"
#include <utility>
#include <vector>


//x좌표와 y좌표를 함께 가지고 있는 점을 나타낼 수 있는 새로운 변수 ii.
typedef pair<int, int> ii;

//원판을 나타내기 위하여 원판의 양 끝 점을 가지고 있는 새로운 변수 pp.
typedef pair<ii, ii> pp;

//mode는 원판의 개수를 나타내고 이는 1~9까지 숫자로 사용자에 따라 입력 받아 정해진다.
int mode;

//Hanoi1, Hanoi2, Hanoi3은 각 기둥의 원판의 정보가 저장된 vector이다.
vector<pp> Hanoi1, Hanoi2, Hanoi3;

//하노이 탑의 원판의 정보를 생성해주는 함수로 입력 받은 mode에 따라 좌표에 맞게 원판의 끝 점을 설정한다.
void makeHanoi(int i) {

    //plate는 원판의 양끝점 정보를 생성하고 저장하여 초기 출발점 stack에 구성한다.
    pp plate;
    //p는 원판 외 목적지나 나머지 기둥의 stack에 0으로 초기화한 빈 데이터를 입력하여 기둥의 stack을 구성하지만 출력 시 빈 기둥으로 나오게 한다.
    pp p;

    //초기 출발 기둥은 Hanoi1기둥으로 1기둥에 push_back한다.
    for (int j = 0; j < i; j++) {
        plate.first.first = 256 - 120 + 10 * j;
        plate.first.second = 643 - 40 * j;
        plate.second.first = 256 + 120 - 10 * j;
        plate.second.second = 643 - 40 * j;
        Hanoi1.push_back(plate);
    }

    p.first.first = 0;
    p.first.second = 0;
    p.second.first = 0;
    p.second.second = 0;

    //나머지 기둥과 도착점 기둥인 2, 3기둥에는 0으로 초기화한 데이터를 입력한다.
    for (int j = 0; j < i; j++) {
        Hanoi2.push_back(p);
        Hanoi3.push_back(p);
    }

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60); // Limit the speed of our program to 15 frames per second
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetLineWidth(10);
    ofSetColor(0, 0, 0);

    //3개의 기둥을 그린다.
    ofDrawLine(256, 200, 256, 668);
    ofDrawLine(512, 200, 512, 668);
    ofDrawLine(768, 200, 768, 668);

    //바닥을 그린다.
    ofSetLineWidth(20);
    ofSetColor(100, 100, 100);
    ofDrawLine(56, 673, 968, 673);

    //모드, 즉 원판의 개수를 입력 받고 draw_flag를 통해 원판을 그린다.
    //s를 누른 후 사용자의 입력에 따라 3개의 기둥의 stack에 변화가 생기므로 항상 3개의 기둥 모두 출력한다
    //이때 원판이 없는 stack은 0 데이터를 입력함에 따라 아무것도 출력되지 않고 오로지 원판만 출력된다.
    ofSetLineWidth(200);
    if (draw_flag) {
        for (int i = 0; i < mode; i++) {
            if (Hanoi1[i].first.first == 0) continue;
            else {
                ofSetColor(20 * i, 20 * i, 255 - 20 * i);
                ofDrawLine(Hanoi1[i].first.first, Hanoi1[i].first.second, Hanoi1[i].second.first, Hanoi1[i].second.second);
            }
        }
        for (int i = 0; i < mode; i++) {
            if (Hanoi2[i].first.first == 0) continue;
            else {
                ofSetColor(20 * i, 20 * i, 255 - 20 * i);
                ofDrawLine(Hanoi2[i].first.first, Hanoi2[i].first.second, Hanoi2[i].second.first, Hanoi2[i].second.second);
            }
        }
        for (int i = 0; i < mode; i++) {
            if (Hanoi3[i].first.first == 0) continue;
            else {
                ofSetColor(20 * i, 20 * i, 255 - 20 * i);
                ofDrawLine(Hanoi3[i].first.first, Hanoi3[i].first.second, Hanoi3[i].second.first, Hanoi3[i].second.second);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    //s를 누르고 콘솔창에서 plate의 개수를 정하여 1~9의 숫자를 입력한다.
    if (key == 's' || key=='S') {
        int select;
        cout << "Choose the numer (1 ~ 9 ) : ";
        cin >> select;
        mode = select;
        cout << "Move 1->2 : T" << endl;
        cout << "Move 1->3 : Y" << endl;
        cout << "Move 2->1 : G" << endl;
        cout << "Move 2->3 : H" << endl;
        cout << "Move 3->1 : B" << endl;
        cout << "Move 3->2 : N" << endl;
        make_flag = 1;
        //입력받은 mode에 따라 hanoi탑을 생성한다.
        if (make_flag) {
            makeHanoi(mode);
            make_flag = 0;
        }
        //탑 생성 후 그려준다.
        draw_flag = 1;
    }

    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q' || key=='Q') {
        //Reset flags
        draw_flag = 0;
        make_flag = 0;
        count_num = 0;
        count_flag = 0;
        // Free the dynamically allocated memory exits.
        Hanoi1.clear();
        Hanoi2.clear();
        Hanoi3.clear();
        vector<pp>().swap(Hanoi1);
        vector<pp>().swap(Hanoi2);
        vector<pp>().swap(Hanoi3);
        cout << "Dynamically allocated memory has been freed." << endl;
        _Exit(0);
    }

    //아래의 키 6개는 각각 기둥에서 기둥으로 이동시키는 키이다.
    //여기서 옮기려는 plate의 사이즈가 더 크면 이동시킬 수 없다.
    //옮길 수 없는 경우 에러메세지가 콘솔창에 출력된다.


    //1->2 기둥으로 plate 이동.
    if (key == 't' || key=='T' ) {
        //호출 시 count_num을 통해 횟수를 저장한다.
        count_num++;
        //empty변수는 타워가 비워져 있는지 확인하는 변수이다.
        int empty = 0;
        //원판 데이터는 0이면 없는 것이므로 vector의 모든 데이터가 0이면 타워가 비워져있음을 뜻한다.
        for (int i = 0; i < mode; i++) {
            if (Hanoi1[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower1 is empty" << endl;
        }
        //원판을 이동시키는 작업이다.
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;
            //count는 원판을 이동시킬 곳에 몇개의 원판이 있는지 계산해준다. 이는 후에 이동시킨 원판의 y좌표를 계산하는데 이용하여 타워의 맨위에 원판이 그려질 수 있도록 한다.
            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi2[i].first.first == 0) continue;
                else count++;
            }
            //tmp는 이동하는 원판의 정보이다.
            pp tmp;
            //Hanoi1의 제일 위의 원판의 정보를 꺼낸다.
            tmp = Hanoi1.back();
            //이동시키는 곳에 더 작은 원판이 있는지 확인 후 있으면 에러메세지를 출력한다.
            if (Hanoi2.back().first.first > tmp.first.first + 256) {
                cout << "There is smaller plate in Tower2." << endl;
            }
            //이동이 가능하면, Hanoi2의 0데이터를 앞에서 제거한 후, tmp정보를 Hanoi2의 마지막에 넣어준다.
            //그 후, Hanoi1에서 마지막 데이터가 빠졌으므로 0데이터를 앞에서 넣어 vector를 유지한다.
            else {
                Hanoi2.erase(Hanoi2.begin());
                tmp.first.first = tmp.first.first + 256;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first + 256;
                tmp.second.second = 643 - 40 * count;
                Hanoi2.push_back(tmp);
                Hanoi1.pop_back();
                Hanoi1.insert(Hanoi1.begin(), t);
            }
        }
    }
    //1->3 기둥으로 plate 이동.
    if (key == 'y' || key=='Y' ) {
        count_num++;
        int empty = 0;
        for (int i = 0; i < mode; i++) {
            if (Hanoi1[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower1 is empty" << endl;
        }
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;

            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi3[i].first.first == 0) continue;
                else count++;
            }
            pp tmp;
            tmp = Hanoi1.back();
            if (Hanoi3.back().first.first > tmp.first.first + 512 ) {
                cout << "There is smaller plate in Tower3." << endl;
            }
            else {
                Hanoi3.erase(Hanoi3.begin());
                tmp.first.first = tmp.first.first + 512;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first + 512;
                tmp.second.second = 643 - 40 * count;
                Hanoi3.push_back(tmp);
                Hanoi1.pop_back();
                Hanoi1.insert(Hanoi1.begin(), t);
            }
        }
        //게임의 종료를 알리는 count_flag이다. Hanoi3에 모든 원판이 꽉차면 종료한다. 이때 옮긴 횟수를 같이 출력한다.
        for (int i = 0; i < mode; i++) {
            if (Hanoi3[i].first.first == 0) count_flag++;
        }
        if (!count_flag) {
            cout << "Clear!" << endl;
            cout << "You move " << count_num << " times to clear." << endl;
            count_num = 0;
            //Reset flags
            draw_flag = 0;
            make_flag = 0;
            count_num = 0;
            count_flag = 0;
            // Free the dynamically allocated memory exits.
            Hanoi1.clear();
            Hanoi2.clear();
            Hanoi3.clear();
            vector<pp>().swap(Hanoi1);
            vector<pp>().swap(Hanoi2);
            vector<pp>().swap(Hanoi3);
            cout << "Dynamically allocated memory has been freed." << endl;
            _Exit(0);
        }
        count_flag = 0;
    }
    //2->1 기둥으로 plate 이동.
    if (key == 'g' || key == 'G') {
        count_num++;
        int empty = 0;
        for (int i = 0; i < mode; i++) {
            if (Hanoi2[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower2 is empty" << endl;
        }
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;

            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi1[i].first.first == 0) continue;
                else count++;
            }
            pp tmp;
            tmp = Hanoi2.back();
            if (Hanoi1.back().first.first > tmp.first.first - 256 ) {
                cout << "There is smaller plate in Tower1." << endl;
            }
            else {
                Hanoi1.erase(Hanoi1.begin());
                tmp.first.first = tmp.first.first - 256;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first - 256;
                tmp.second.second = 643 - 40 * count;
                Hanoi1.push_back(tmp);
                Hanoi2.pop_back();
                Hanoi2.insert(Hanoi2.begin(), t);
            }
        }
    }
    //2->3 기둥으로 plate 이동.
    if (key == 'h' || key == 'H') {
        count_num++;
        int empty = 0;
        for (int i = 0; i < mode; i++) {
            if (Hanoi2[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower2 is empty" << endl;
        }
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;

            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi3[i].first.first == 0) continue;
                else count++;
            }
            pp tmp;
            tmp = Hanoi2.back();
            if (Hanoi3.back().first.first > tmp.first.first + 256 ) {
                cout << "There is smaller plate in Tower3." << endl;
            }
            else {
                Hanoi3.erase(Hanoi3.begin());
                tmp.first.first = tmp.first.first + 256;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first + 256;
                tmp.second.second = 643 - 40 * count;
                Hanoi3.push_back(tmp);
                Hanoi2.pop_back();
                Hanoi2.insert(Hanoi2.begin(), t);
            }
        }
        for (int i = 0; i < mode; i++) {
            if (Hanoi3[i].first.first == 0) count_flag++;
        }
        if (!count_flag) {
            cout << "Clear!" << endl;
            cout << "You move " << count_num << " times to clear." << endl;
            //Reset flags
            draw_flag = 0;
            make_flag = 0;
            count_num = 0;
            count_flag = 0;
            // Free the dynamically allocated memory exits.
            Hanoi1.clear();
            Hanoi2.clear();
            Hanoi3.clear();
            vector<pp>().swap(Hanoi1);
            vector<pp>().swap(Hanoi2);
            vector<pp>().swap(Hanoi3);
            cout << "Dynamically allocated memory has been freed." << endl;
            _Exit(0);
        }
        count_flag = 0;
    }
    //3->1 기둥으로 plate 이동.
    if (key == 'b' || key == 'B') {
        count_num++;
        int empty = 0;
        for (int i = 0; i < mode; i++) {
            if (Hanoi3[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower3 is empty" << endl;
        }
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;

            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi1[i].first.first == 0) continue;
                else count++;
            }
            pp tmp;
            tmp = Hanoi3.back();
            if (Hanoi1.back().first.first > tmp.first.first - 512 ) {
                cout << "There is smaller plate in Tower1." << endl;
            }
            else {
                Hanoi1.erase(Hanoi1.begin());
                tmp.first.first = tmp.first.first - 512;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first - 512;
                tmp.second.second = 643 - 40 * count;
                Hanoi1.push_back(tmp);
                Hanoi3.pop_back();
                Hanoi3.insert(Hanoi3.begin(), t);
            }
        }
    }
    //3->2 기둥으로 plate 이동.
    if (key == 'n' || key == 'N') {
        count_num++;
        int empty = 0;
        for (int i = 0; i < mode; i++) {
            if (Hanoi3[i].first.first == 0) empty++;
        }
        if (empty == mode) {
            cout << "Tower3 is empty" << endl;
        }
        else {
            pp t;
            t.first.first = 0;
            t.first.second = 0;
            t.second.first = 0;
            t.second.second = 0;

            int count = 0;
            for (int i = 0; i < mode; i++) {
                if (Hanoi2[i].first.first == 0) continue;
                else count++;
            }
            pp tmp;
            tmp = Hanoi3.back();
            if (Hanoi2.back().first.first > tmp.first.first - 256 ) {
                cout << "There is smaller plate in Tower2." << endl;
            }
            else {
                Hanoi2.erase(Hanoi2.begin());
                tmp.first.first = tmp.first.first - 256;
                tmp.first.second = 643 - 40 * count;
                tmp.second.first = tmp.second.first - 256;
                tmp.second.second = 643 - 40 * count;
                Hanoi2.push_back(tmp);
                Hanoi3.pop_back();
                Hanoi3.insert(Hanoi3.begin(), t);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    
}
