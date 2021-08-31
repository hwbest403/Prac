#include "tetris.h"

static struct sigaction act, oact;

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate);

void makeTree(RecNode* root) {
	int i;
	RecNode** c = root->c;

	for (i = 0; i < CHILDREN_MAX; i++) {
		c[i] = (RecNode*)malloc(sizeof(RecNode));
		c[i]->lv = root->lv + 1;
		c[i]->f = (char(*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
		if (c[i]->lv < VISIBLE_BLOCKS) {
			makeTree(c[i]);
		}
	}
}

void deleteTree(RecNode* root) {
	int i;
	RecNode** c = root->c;
	for (i = 0; i < CHILDREN_MAX; i++) {
		if (c[i]->lv < VISIBLE_BLOCKS) {
			deleteTree(c[i]);
		}
		free(c[i]->f);
		free(c[i]);
	}
}

void makemodifiedTree(ModifiedNode* root) {
	int i;
	ModifiedNode** c = root->c;
	for (i = 0; i < CHILDREN_MAX; i++) {
		c[i] = (ModifiedNode*)malloc(sizeof(ModifiedNode));
		c[i]->lv = root->lv + 1;
		if (c[i]->lv < VISIBLE_BLOCKS) {
			makemodifiedTree(c[i]);
		}
	}
}

void deletemodifiedTree(ModifiedNode* root) {
	int i;
	ModifiedNode** c = root->c;
	for (i = 0; i < CHILDREN_MAX; i++) {
		if (c[i]->lv < VISIBLE_BLOCKS) {
			deletemodifiedTree(c[i]);
		}
		free(c[i]);
	}
}

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	//recRoot = (RecNode*)malloc(sizeof(RecNode));
	//recRoot->lv = 0;
	//recRoot->score = 0;
	//recRoot->f = field;
	//makeTree(recRoot);
	ModRoot = (ModifiedNode*)malloc(sizeof(ModifiedNode));
	ModRoot->lv = 0;
	ModRoot->score = 0;
	for (int i = 0; i < WIDTH; i++) {
		ModRoot->f[i] = HEIGHT;
	}
	makemodifiedTree(ModRoot);
	
	while(!exit){
		clear();
		createRankList(); //랭크 리스트를 만들어준다.
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		default: break;
		}
	}

	endwin();
	system("clear");
	//deleteTree(recRoot);
	deletemodifiedTree(ModRoot);
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2] = rand() % 7; //두번째 next block을 생성한다.
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	//recommend(recRoot);
	Modifiedrecommend(ModRoot);
	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate); //drawblock을 drawblockwithfeature로 바꿔준다.
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	DrawBox(10, WIDTH + 10, 4, 8);//nextblock[2]를 보여줄 공간을 만든다.

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(17,WIDTH+13);
	printw("SCORE");
	DrawBox(18,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(19,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for (i = 0; i < 4; i++) {
		move(11 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}//두 번째 next block을 그린다.
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}
	}
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}//drawblock과 drawshadow, drawrecommend를 호출하여 블럭과 그림자, 추천위치를 같이 보여주도록한다.

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				if ((f[blockY + i][blockX + j] == 1) || ((blockY + i) >= HEIGHT) || ((blockX + j) < 0) || ((blockX + j) >= WIDTH)) {
					return 0;
				}
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int pRotate, pblockX, pblockY;
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	pRotate = blockRotate;
	pblockX = blockX;
	pblockY = blockY;
	switch (command) {
	case KEY_UP:
		pRotate = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		pblockY--;
		break;
	case KEY_RIGHT:
		pblockX--;
		break;
	case KEY_LEFT:
		pblockX++;
		break;
	default:
		break;
	}
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][pRotate][i][j] == 1) {
				f[pblockY + i][pblockX + j] = 0;
			}
		}
	}
	DrawField();
	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);//그림자 생성.
	move(HEIGHT + 1, WIDTH + 1);
}

void BlockDown(int sig){
	// user code
	timed_out = 0;
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == 1) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		if (blockY == -1) {
			gameOver = 1;
		}
		score = score + AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score = score + DeleteLine(field);
		PrintScore(score);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7; //2번째를 추가했기에 1번째 2번째를 순서대로 가져오도록 한다.
		recommend(recRoot);
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		DrawNextBlock(nextBlock);

	}
	//강의자료 p26-27의 플로우차트를 참고한다.
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int score2;
	int touched = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if ((blockY + i + 1 < HEIGHT)) {
					if (f[blockY + i + 1][blockX + j] == 1) {
						touched++;
					}
				}
				else if (((blockY + i + 1) == HEIGHT)) {
					touched++;
				}//닿는 면적에 따라 touched를 증가시킨다.
			}
		}
	}
	score2 = touched * 10;
	return score2;
	//Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	int score1=0;
	int del = 0;
	int flag;
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	for (int i = 0; i < HEIGHT; i++) {
		flag = 1;
		for (int j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) {
				flag = 0;
				break;
			}
		}
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
		if (flag == 1) {
			del++;
			for (int k = i - 1; k >= 0; k--) {
				for (int j = 0; j < WIDTH; j++) {
					f[k + 1][j] = f[k][j];
				}
			}
			score1 = score1 + del * del * 100;
		}
	}
	move(HEIGHT + 1, HEIGHT + 1);
	DrawField();
	return score1;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	while (1) {
		if ((CheckToMove(field, nextBlock[0], blockRotate, y, x)) == 1) {
			y++;
			continue;
		}
		else {
			break;
		}
	}
	y = y - 1;
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList(){
	// user code
	char rankname[NAMELEN];
	int rankscore;
	_Node* newnode, * tmp;

	FILE* outfp = fopen("rank.txt", "r");
	if (outfp == NULL) {
		return;
	}//rank.txt를 연다.

	fscanf(outfp, "%d", &score_number);

	ranklist = (_Node*)malloc(sizeof(_Node));
	ranklist->link = NULL;
	tmp = ranklist; //첫 노드, headnode

	for (int i = 0; i < score_number; i++) {
		newnode = (_Node*)malloc(sizeof(_Node));
		fscanf(outfp, "%s %d", (newnode->name), &(newnode->score));
		ranklist->link = newnode;
		newnode->link = NULL;
		ranklist = ranklist->link;
	}//ranksize만큼 노드를 생성하여 linked list로 연결한다.

	ranklist = tmp;
	fclose(outfp);
}

void rank(){
	// user code
	int X=1, Y=score_number, ch, i, j;
	char name[NAMELEN];
	_Node* search = NULL;
	clear();

	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	ch = wgetch(stdscr);

	if (ch == '1') { //x부터 y까지 rank를 출력한다.
		echo();//입력하는 값이 화면에 출력되지 않는다.
		printw("X: ");
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();
		printw("       name     |   score   \n");
		printw("----------------------------\n");

		if (X > Y || score_number==0 || X>score_number) {
			mvprintw(8, 0, "search failure: no rank in the list\n");
		}
		else {
			search = ranklist;
			for (int i = 0; i < X; i++) {
				search = search->link;
			}
			for (int i = 0; i <= (score_number - X); i++) {
				printw("%-16s| %d\n", search->name, search->score);
				search = search->link;
			}
		}
	}

	else if (ch == '2') { //과제에서 특정 사람의 점수를 찾는 경우
		printw("input the name: ");
		echo();
		getstr(name);
		noecho();
		printw("       name     |   score   \n");
		printw("----------------------------\n");

		_Node* searchname;
		searchname = ranklist->link;
		char* tmpname[NAMELEN];
		int target = 0;
		while (searchname != NULL) {
			if (strcmp(searchname->name, name) == 0) {
				target = target + 1;
				printw("%-16s| %d\n", searchname->name, searchname->score);
			}
			searchname = searchname->link;
		}
		if (target == 0) {
			printw("\nsearch failure: no name in the list\n");
		}
	}

	else if (ch == '3') { //과제에서 특정 랭크를 지우는 경우

		printw("input the rank: ");
		echo();
		scanw("%d", &X);
		noecho();

		_Node* tmp1, * tmp2;
		tmp1 = ranklist->link;
		tmp2 = ranklist;
		if (X > score_number) {
			printw("\nsearch failure: the rank not in the list\n");
		}
		else {
			for (int count = 1; count < X; count++) {
				tmp1 = tmp1->link;
				tmp2 = tmp2->link;
			}
			tmp2->link = tmp1->link;
			free(tmp1);
			newranksize = score_number - 1;
			writeRankFile();
			printw("\nresult: the rank deleted\n");
		}
	}
	getch();
}

void writeRankFile(){
	// user code
	FILE* infp;
	_Node* tmp, * tmp2;

	infp = fopen("rank.txt", "w");
	newranksize = 0;
	tmp2 = ranklist->link;
	while ((tmp2->link) != NULL) {
		newranksize = newranksize + 1;
		tmp2 = tmp2->link;
	}
	newranksize = newranksize + 1;
	fprintf(infp, "%d\n", newranksize);

	tmp = ranklist->link;
	while ((tmp->link) != NULL) {
		fprintf(infp, "%s %d\n", (tmp->name), (tmp->score));
		tmp = tmp->link;
	}
	fprintf(infp, "%s %d", (tmp->name), (tmp->score));

	fclose(infp);

	tmp = ranklist->link;
	while ((tmp->link) != NULL) {
		_Node* delete;
		delete = tmp;
		tmp = tmp->link;
		free(delete);
	}
	free(tmp);
	free(ranklist);
}

void newRank(int score){
	// user code
	char newname[NAMELEN];
	clear();
	printw("your name: ");
	echo();
	getstr(newname);
	if (strlen(newname) == 0) {
		strcpy(newname, "N/A");
	}
	noecho();
	_Node* newnode, * tmp1, * tmp2;
	newnode = (_Node*)malloc(sizeof(_Node));
	strcpy(newnode->name, newname);
	newnode->score = score;
	if (ranklist->link == NULL) {
		ranklist->link = newnode;
		newnode->link = NULL;
		writeRankFile();
		return;
	}
	tmp1 = ranklist->link;
	tmp2 = ranklist;
	while (1) {
		if ((tmp1->score) > score) {
			if ((tmp1->link) == NULL) {
				tmp1->link = newnode;
				newnode->link = NULL;
				break;
			}
			else {
				tmp1 = tmp1->link;
				tmp2 = tmp2->link;
				continue;
			}
		}
		else {
			newnode->link = tmp1;
			tmp2->link = newnode;
			break;
		}
	}
	writeRankFile();
}

int ModifiedDeleteLine(char f[HEIGHT][WIDTH], int x) {
	int score1 = 0;
	int del = 0;
	int full, tmp;
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		full = 1;
		for (j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) {
				full = 0;
				break;
			}
		}
		if (full == 1) {
			tmp = 1;
			for (j = 0; j < WIDTH; j++) {
				if ((field[i][j] == 0) && ((j < x) || (j > (x + 3)))) {
					tmp = 0;
					break;
				}
			}
			if (tmp == 1) {
				del++;
				for (int k = i - 1; k >= 0; k--) {
					for (int j = 0; j < WIDTH; j++) {
						f[k + 1][j] = f[k][j];
					}
				}
			}
			else continue;
		}
	}
	score1 += del * del * 100;
	move(HEIGHT + 1, HEIGHT + 1);
	DrawField();
	return score1;

}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	while (1) {
		if ((CheckToMove(field, nextBlock[0], blockRotate, y, x)) == 1) {
			y++;
			continue;
		}
		else {
			break;
		}
	}
	y = y - 1;
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	int rotate;
	int index = root->lv + 1;
	int childindex = 0;
	int left, right;
	int X, Y;
	int recX, recY, recR;
	int tmpScore;
	int flag = 0;
	(root->c)[childindex]->lv = index;

	for (rotate = 0; rotate < NUM_OF_ROTATE; rotate++) {
		left = 3;
		for (int i = 0; i < 4; i++) {
			int j;
			for (j = 0; j < 4; j++) {
				if (block[nextBlock[index - 1]][rotate][i][j] == 1) {
					break;
				}
			}
			if (j < left) {
				left = j;
			}
		}
		left = 0 - left;

		right = 0;
		for (int i = 0; i < 4; i++) {
			int j;
			for (j = 3; j > -1; j--) {
				if (block[nextBlock[index - 1]][rotate][i][j] == 1) {
					break;
				}
			}
			if (j > right) {
				right = j;
			}
		}
		right = WIDTH - 1 - right;

		for (X = left; X <= right; X++) {
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					(root->c)[childindex]->f[i][j] = root->f[i][j];
				}
			}
			int currPoint = 0;
			Y = 0;
			if (CheckToMove((root->c)[childindex]->f, nextBlock[index - 1], rotate, Y, X) == 0) continue;
			while (CheckToMove((root->c)[childindex]->f, nextBlock[index - 1], rotate, Y, X) == 1) {
				Y++;
			}
			Y = Y - 1;

			currPoint = currPoint + AddBlockToField((root->c)[childindex]->f, nextBlock[index - 1], rotate, Y, X);
			currPoint = currPoint + DeleteLine((root->c)[childindex]->f);
			(root->c)[childindex]->score = currPoint + (root->score);
			if (index < VISIBLE_BLOCKS) {
				tmpScore = recommend((root->c)[childindex]);
			}
			else {
				tmpScore = (root->c)[childindex]->score;
			}
			if (tmpScore > max) {
				flag = 1;
				max = tmpScore;
				recX = X;
				recY = Y;
				recR = rotate;
			}
			childindex++;
		}
	}
	if (root->lv == 0 && flag == 1) {
		recommendX = recX;
		recommendY = recY;
		recommendR = recR;
	}

	return max;
}

int Modifiedrecommend(ModifiedNode* root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	// user code
	int index = root->lv + 1;
	int childindex = 0;
	int left, right;
	int rotate;
	int X = 0, Y = 0;
	int tmpScore = 0, recX = 0, recY = 0, recR = 0;
	(root->c)[childindex]->lv = index;
	int flag = 0;

	for (rotate = 0; rotate < NUM_OF_ROTATE; rotate++) {
		left = 3;
		for (int i = 0; i < 4; i++) {
			int j;
			for (j = 0; j < 4; j++) {
				if (block[nextBlock[index - 1]][rotate][i][j] == 1) {
					break;
				}
			}
			if (j < left) {
				left = j;
			}
		}
		left = 0 - left;

		right = 0;
		for (int i = 0; i < 4; i++) {
			int j;
			for (j = 3; j > -1; j--) {
				if (block[nextBlock[index - 1]][rotate][i][j] == 1) {
					break;
				}
			}
			if (j > right) {
				right = j;
			}
		}
		right = WIDTH - 1 - right;

		int k;
		for (X = left; X <= right; X++) {
			char modified[HEIGHT][WIDTH];
			if (index == 1) {
				for (int i = 0; i < WIDTH; i++) {
					for (int j = 0; j < HEIGHT; j++) {
						modified[j][i] = 0;
						if (field[j][i] == 1) {
							for (int k = j; k < HEIGHT; k++) {
								modified[k][i] = 1;
							}
							break;
						}
					}
				}
			}
			else {
				for (int i = 0; i < WIDTH; i++) {
					k = root->f[i];
					for (int j = 0; j < HEIGHT; j++) {
						if (j < k) {
							modified[j][i] = 0;
						}
						else {
							modified[j][i] = 1;
						}
					}
				}
			}
			int currPoint = 0;
			Y = 0;
			if (CheckToMove(modified, nextBlock[index - 1], rotate, Y, X) == 0) continue;
			while (CheckToMove(modified, nextBlock[index - 1], rotate, Y, X) == 1) {
				Y++;
			}
			Y = Y - 1;

			currPoint = currPoint + AddBlockToField(modified, nextBlock[index - 1], rotate, Y, X);
			currPoint = currPoint + ModifiedDeleteLine(modified, X);
			//	currPoint=currPoint+DeleteLine(modif);
			for (int i = 0; i < WIDTH; i++) {
				(root->c)[childindex]->f[i] = HEIGHT;
				for (int j = 0; j < HEIGHT; j++) {
					if (modified[j][i] == 1) {
						(root->c)[childindex]->f[i] = j;
						break;
					}
				}
			}
			(root->c)[childindex]->score = currPoint + (root->score);
			if (index < VISIBLE_BLOCKS) {
				tmpScore = Modifiedrecommend((root->c)[childindex]);
			}
			else {
				tmpScore = (root->c)[childindex]->score;
			}
			if (tmpScore > max) {
				flag = 1;
				max = tmpScore;
				recX = X;
				recY = Y;
				recR = rotate;
			}
			childindex++;
		}
	}
	if (root->lv == 0 && flag == 1) {
		recommendX = recX;
		recommendY = recY;
		recommendR = recR;
	}

	return max;
}

void recommendedPlay(){
	// user code
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		blockX = WIDTH / 2 - 2;
		blockY = -1;
		blockRotate = 0;
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
		move(HEIGHT + 1, HEIGHT + 1);
		blockX = recommendX;
		blockY = recommendY;
		blockRotate = recommendR;
		if (CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX) == 0) gameOver = 1;

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}
