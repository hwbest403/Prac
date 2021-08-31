#include <vector>
#include "ofApp.h" 

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    
    if( draw_flag==1 ){
        
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        ofSetLineWidth(5);
        ofSetColor(200, 100, 0);
        for (const auto& line : linez) {
            ofDrawLine(line.first.x, line.first.y, line.second.x, line.second.y);
        }

        for (int i = 0; i < dotz.size(); i++) {
            if (i == redindex) ofSetColor(255, 0, 0);
            else ofSetColor(0);
            ofDrawCircle(dotz[i].x, dotz[i].y, 10);
        }
        
        // 2nd week portion.
        ofSetLineWidth(4);
        ofSetColor(0, 0, 255);
        for (const auto& water : waterLinez) {
            ofDrawLine(water.first.x, water.first.y, water.second.x, water.second.y);
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'l' || key == 'L') {
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a txt");
        if (openFileResult.bSuccess) {
            cout << "We found the target file." << endl;
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        load_flag = 0;
        // Free the dynamically allocated memory exits.
        
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd' || key=='D'){
        if( !load_flag) return;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
        draw_flag = 1;
        
    }
    if (key == 's' || key == 'S') {
        // 2nd week portion.
        isWater = true;
        posInfo currentSource = dotz[redindex];
        while (1) {
            currentSource = dowaterfall(currentSource);
            if (currentSource.y == ofGetHeight()) break;
        }
    }
    if (key == 'e' || key == 'E'){
        // 2nd week portion.
        isWater = false;

        waterLinez.clear();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    /*if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }*/
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    
    if (key == OF_KEY_RIGHT && !isWater){
        redindex++;
        if (redindex >= dotz.size()) {
            redindex = 0;
        }
        cout << "Selcted Dot Coordinate is (" << dotz[redindex].x << ", " << dotz[redindex].y << ")" << endl;
    }
    if (key == OF_KEY_LEFT && !isWater){
        redindex--;
        if (redindex < 0) {
            redindex = dotz.size() - 1;
        }
        cout << "Selcted Dot Coordinate is (" << dotz[redindex].x << ", " << dotz[redindex].y << ")" << endl;
    }
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

posInfo ofApp::dowaterfall(posInfo currentPos)
{
    vector<lineInfo> targetLinez;
    lineInfo fallingLine, targetLine;
    fallingLine.first = currentPos;
    fallingLine.second = posInfo(currentPos.x, ofGetHeight());

    for (auto& currentLine : linez) {
        if (lineCrossCheck(currentLine, fallingLine)) {
            targetLinez.push_back(currentLine);
        }
    }

    int fallingY = 0;
    if (targetLinez.size() == 0) {
        waterLinez.push_back(lineInfo(posInfo(fallingLine.first.x, fallingLine.first.y),
            posInfo(fallingLine.second.x, fallingLine.second.y)));
        return fallingLine.second;
    }
    else if (targetLinez.size() == 1) {
        targetLine = targetLinez[0];
        fallingY = findFallingYpos(targetLine, currentPos);
    }
    else {
        int idx = 0, minYpos = 1000, minIdx = 0;
        for (auto& currentLine : targetLinez) {
            int tmpYpos = findFallingYpos(currentLine, currentPos);
            if (tmpYpos < minYpos) {
                minYpos = tmpYpos;
                minIdx = idx;
            }
            idx++;
        }
        targetLine = targetLinez[minIdx];
        fallingY = findFallingYpos(targetLine, currentPos);
    }

    waterLinez.push_back(lineInfo(posInfo(currentPos.x, currentPos.y), posInfo(currentPos.x, fallingY)));
    posInfo slopedPos = findSlopedPos(targetLine, posInfo(currentPos.x, fallingY));
    waterLinez.push_back(lineInfo(posInfo(currentPos.x, fallingY), posInfo(slopedPos.x, slopedPos.y)));
    return slopedPos;
}

int ofApp::ccw(posInfo first, posInfo second, posInfo third)
{
    int op = first.x * second.y + second.x * third.y + third.x * first.y;
    op -= (first.y * second.x + second.y * third.x + third.y * first.x);
    if (op > 0)return 1;
    else if (op == 0)return 0;
    else return -1;
}

bool ofApp::lineCrossCheck(lineInfo first, lineInfo second)
{
    int ab = ccw(first.first, first.second, second.first) * ccw(first.first, first.second, second.second);
    int cd = ccw(second.first, second.second, first.first) * ccw(second.first, second.second, first.second);
    if (ab == 0 && cd == 0) {
        int a, b, c, d;
        if (first.first.x < first.second.x) {
            a = first.second.x;
            b = first.first.x;
        }
        else {
            b = first.second.x;
            a = first.first.x;
        }
        if (second.first.x < second.second.x) {
            c = second.second.x;
            d = second.first.x;
        }
        else {
            d = second.second.x;
            c = second.first.x;
        }
        return c <= b && a <= d;
    }
    return ab <= 0 && cd <= 0;
}

int ofApp::findFallingYpos(lineInfo line, posInfo dot)
{
    int ypos = (line.second.y - line.first.y) * (dot.x - line.first.x) / (line.second.x - line.first.x) + line.first.y;
    return ypos;
}

posInfo ofApp::findSlopedPos(lineInfo line, posInfo dot)
{
    if (line.first.y > line.second.y) {
        return posInfo(line.first.x + 1, line.first.y + 1);
    }
    else {
        return posInfo(line.second.x + 1, line.second.y + 1);
    }
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
    
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    
    
    // Read file line by line
    int a = 0;
    int b = 0;

    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {

        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if (words.size() == 1) {
            if (input_type == 0) { // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                input_type++;
            }
            else if (input_type==1) { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
                input_type++;
            }
        }
        else if (words.size() >= 2) {
            int x1, y1, x2, y2;
            if (input_type == 1) { // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());

                if (y1 >= 728) break;
                else if (y2 >= 728) break;

                posInfo first, second;
                first.x = x1;
                first.y = y1;
                second.x = x2;
                second.y = y2;
                
                lineInfo new_line;
                new_line.first = first;
                new_line.second = second;
                linez.push_back(new_line);
                
                a++;
            }
            else if (input_type==2) { // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());

                if (y1 >= 728) break;

                posInfo new_dot;
                new_dot.x = x1;
                new_dot.y = y1;
                dotz.push_back(new_dot);
                
                b++;
            }
        } // End of else if.
        else break;
    } // End of for-loop (Read file line by line).
    
    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    ;
}