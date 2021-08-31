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
//edge�� �����ϴ� ����� vertex �ΰ��� ������ ������, edge�� ����ġ�� ������ weight�� �����Ѵ�.

typedef struct _vertice {
	int parent;
	int rank;
}vertice;
//vetice�� ������ �� disjoint set�� �̿��ϱ� ���� parent�� rank�� ������ ������ ������ ������ش�.
//���� disjoint set�� minheap�� �̿��Ͽ� �ϼ��Ѵ�.

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

	//kruskal's algorithm�� �����Ѵ�. edge�� ����ġ�� ���ĵ� �͵��� ����ġ�� ���� �ͺ��� ���ʷ� �����Ѵ�.
	//�̶� ����Ŭ�� �߻����� �ʵ��� �ϴ� algorithm�� ���� �����Ѵ�. �̸� ���� minimum spanning tree�� �����Ѵ�.
	int count = 0;
	int totalweight = 0;

	for (int i = 0; i < verticesize; i++) {
		makeset(i);
	}

	sortedge(edgesize); //edge ����ġ �� ����

	for (int i = 0; i < edgesize; i++) {
		//����Ŭ�� �߻����� �ʵ��� parent�� �ٸ� �� union�Ѵ�.
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
//input.txt.���� ù�ٿ��� ���� vertice�� ������ ���� makedisjointset�� �ݺ��Ͽ� ������ vertice�� ���� �ڱ� �ڽ��� parent�� ����Ű�� set���� ������ش�.

int find(int a)
{
	if (v[a].parent != a) {
		v[a].parent = find(v[a].parent);
	}
	return v[a].parent;
}
//���� ���� set�� ����Ű�� parent�� �����Ѵ�.

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
//�⺻������ rank�� 0���� �����Ǿ� �ִ� set���� �����Ͽ� disjoint set�� ����� ���� �Լ��̴�.
//rank�� ������ rank�� 1������Ű�� set�� �����Ѵ�.
//rank�� ���Ͽ� rank�� ���� ���� rank�� ū���� subtree�� �����Ѵ�.

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
//minheap�� ���� ����.

void makeminheap(int size)
{
	for (int i = size / 2; i >= 0; i--) {
		useminheap(i, size);
	}
}
//�ԷµǴ� size���� ���� minheap�� useminheap������ �̿��� �����.
//�̶�, minheap�� �����ϴ� ������ kruskal's algorithm�� �̿��ϱ� ���� ����ġ�� ���� edge���� �����ϱ� �����̴�.

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
//kruskal's algortihm�� �����ϱ� ���� edge�� ����ġ�� ���� ������ ������ �ʿ��ϴ�. ���� minheap�� ���� ������ �����Ѵ�.
