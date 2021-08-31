#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#define Maxversize 10000
#define Maxedsize 50000000

typedef struct _edge {
	int Sver, Ever;
	int weight;
}edge;
//edge를 구성하는 요소인 vertex 두개를 저장할 공간과, edge의 가중치를 저장할 weight를 구성한다.

typedef struct _vertice {
	int parent;
	int rank;
}vertice;
//vetice를 구성할 때 disjoint set을 이용하기 위해 parent와 rank의 정보를 저장할 공간을 만들어준다.
//이후 disjoint set은 minheap을 이용하여 완성한다.

edge e[Maxedsize];
edge etmp[Maxedsize];
vertice v[Maxversize];

void makeset(int a);
int find(int a);
void disjointset(int a, int b);
void useminheap(int idx, int size);
void makeminheap(int size);
void sortedge(int size);

int main(int argc, char* argv[]) {

	clock_t start, end;
	double duration;
	start = clock();

	if (argc != 2) {
		cout << "Usage: ./hw3 input_filename\n" << endl;
		exit(1);
	}

	FILE* infp;
	FILE* outfp;

	infp = fopen(argv[1], "r");
	outfp = fopen("hw3_result.txt", "w");

	if (!infp) {
		cout << "The input file does not exit." << endl;
	}

	int edgesize;
	int verticesize;

	for (int i = 0; i < Maxedsize + 2; i++) {
		if (feof(infp)) {
			break;
		}
		if (i == 0) {
			fscanf(infp, "%d", &verticesize);
		}
		else if (i == 1) {
			fscanf(infp, "%d", &edgesize);
		}
		else if (i > 1) {
			fscanf(infp, "%d %d %d", &e[i - 2].Sver, &e[i - 2].Ever, &e[i - 2].weight);
		}
	}

	//kruskal's algorithm을 진행한다. edge의 가중치로 정렬된 것들을 가중치가 낮은 것부터 차례로 선택한다.
	//이때 사이클이 발생하지 않도록 하는 algorithm을 따라 선택한다. 이를 통해 minimum spanning tree를 구성한다.
	int count = 0;
	int totalweight = 0;

	for (int i = 0; i < verticesize; i++) {
		makeset(i);
	}

	sortedge(edgesize); //edge 가중치 순 정렬

	for (int i = 0; i < edgesize; i++) {
		//사이클이 발생하지 않도록 parent가 다를 시 union한다.
		if (find(e[i].Sver) != find(e[i].Ever)) {

			disjointset(e[i].Sver, e[i].Ever);

			count++;
			totalweight = totalweight + e[i].weight;

			fprintf(outfp, "%d %d %d\n", e[i].Sver, e[i].Ever, e[i].weight);
		}
	}
	fprintf(outfp, "%d\n", totalweight);
	if (count == verticesize - 1) {
		fprintf(outfp, "CONNECTED\n");
	}
	else {
		fprintf(outfp, "DISCONNECTED\n");
	}


	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "output written to hw3_result.txt." << endl;
	cout << "running time: " << duration << " seconds" << endl;

	fclose(infp);
	fclose(outfp);

	return 0;
}

void makeset(int a)
{
	v[a].parent = a;
	v[a].rank = 0;
}
//input.txt.에서 첫줄에서 받은 vertice의 개수에 따라 makedisjointset을 반복하여 각각의 vertice를 각각 자기 자신을 parent로 가르키는 set으로 만들어준다.

int find(int a)
{
	if (v[a].parent != a) {
		v[a].parent = find(v[a].parent);
	}
	return v[a].parent;
}
//값을 통해 set이 가르키는 parent에 접근한다.

void disjointset(int a, int b)
{
	a = find(a);
	b = find(b);

	if (v[a].rank > v[b].rank) {
		v[b].parent = a;
	}
	else {
		v[a].parent = b;
		if (v[a].rank == v[b].rank) {
			v[b].rank = v[b].rank + 1;
		}
	}
}
//기본적으로 rank가 0으로 설정되어 있는 set들을 결합하여 disjoint set을 만들기 위한 함수이다.
//rank가 같으면 rank를 1증가시키며 set을 결합한다.
//rank를 비교하여 rank가 작은 쪽을 rank가 큰쪽의 subtree로 결합한다.

void useminheap(int idx, int size)
{
	edge tmp;
	int left = idx * 2 + 1;
	int right = idx * 2 + 2;
	int minvalue = idx;

	if (left < size && e[left].weight < e[minvalue].weight) {
		minvalue = left;
	}

	if (right < size && e[right].weight < e[minvalue].weight) {
		minvalue = right;
	}
	
	if (minvalue != idx) {
		tmp = e[minvalue];
		e[minvalue] = e[idx];
		e[idx] = tmp;

		useminheap(minvalue, size);
	}
}
//minheap을 위한 연산.

void makeminheap(int size)
{
	for (int i = size / 2; i >= 0; i--) {
		useminheap(i, size);
	}
}
//입력되는 size값에 대한 minheap을 useminheap연산을 이용해 만든다.
//이때, minheap을 구성하는 이유는 kruskal's algorithm을 이용하기 위해 가중치가 적은 edge부터 선택하기 위함이다.

void sortedge(int size)
{
	edge tmp;

	makeminheap(--size);

	for (int i = size; i > 0 ; i--) {
		tmp = e[i];
		e[i] = e[0];
		e[0] = tmp;

		useminheap(0, i);
	}

	for (int i = 0; i <= size; i++) {
		etmp[i] = e[i];
	}

	for (int i = 0; i <= size; i++) {
		e[i] = etmp[size - i];
	}

}
//kruskal's algortihm을 적용하기 위해 edge의 가중치가 낮은 순서로 정렬이 필요하다. 따라서 minheap을 통해 정렬을 수행한다.
