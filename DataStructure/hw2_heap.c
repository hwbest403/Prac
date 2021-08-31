#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXSIZE 1000001
#define HEAP_FULL(n) (n==MAXSIZE-1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
	int key;
}element;

element max_heap[MAXSIZE];
element min_heap[MAXSIZE];
element sub_max_heap[MAXSIZE];
element sub_min_heap[MAXSIZE];
element sub, sub2;
element tmp, tmp2;

int n = 0;

int arr[MAXSIZE];
int arr2[MAXSIZE];

void insert_max_heap(element item, int* n);
void insert_min_heap(element item, int* n);
element delete_max_heap(int* n);
element delete_min_heap(int* n);

char s[8];
char* ins = "INSERT";
char* asc = "ASCEND";
char* des = "DESCEND";

int main(int argc, char *argv[]) {

	clock_t start, end;
	double duration;

	start = clock();

	if (argc != 2) {
		printf("Usage: ./hw2 input_filename\n");
		exit(1);
	}

	int a;

	FILE* infp;
	FILE* outfp;
	infp = fopen(argv[1], "r");
	outfp = fopen("hw2_result.txt", "w");

	if (!infp) {
		printf("The input file does not exist.\n");
		exit(1);
	}

	int currsize = 0;
	int currsize2 = 0;
	int size = 0;
	int size2 = 0;

	while (1) {
		if (feof(infp)) {
			break;
		}
		fscanf(infp, "%s %d\n", s, &a);
		if (strcmp(s, ins) == 0) {
			if (a != 0) {
				sub.key = a;
				sub2.key = a;
				insert_max_heap(sub, &currsize);
				insert_min_heap(sub2, &currsize2);
				currsize++;
				currsize2++;
				size = currsize;
				size2 = currsize2;
			}
		}
		else if (strcmp(s, asc) == 0) {
			/*for (int i = 1; i <= currsize2; i++) {
				sub_min_heap[i] = min_heap[i];
			}
			while(size2>0) {
				tmp2= delete_min_heap(&size2);
				arr[size2] = tmp2.key;
				size2--;
			}
			for (int k = currsize2; k >0; k--) {
				if (arr2[k] != 0) {
					fprintf(outfp, "%d ", arr2[k]);
				}
			}
			fprintf(outfp, "\n");*/
			for (int i = 1; i <= currsize; i++) {
				sub_max_heap[i] = max_heap[i];
			}
			while (size > 0) {
				tmp = delete_max_heap(&size);
				arr[size] = tmp.key;
				size--;
			}
			for (int k = 1; k <= currsize; k++) {
				if (arr[k] != 0) {
					fprintf(outfp, "%d ", arr[k]);
				}
			}
			fprintf(outfp, "\n");
		}
		else if (strcmp(s, des) == 0) {
			for (int i = 1; i <= currsize; i++) {
				sub_max_heap[i] = max_heap[i];
			}
			while (size > 0) {
				tmp = delete_max_heap(&size);
				arr[size] = tmp.key;
				size--;
			}
			for (int k = currsize; k >0; k--) {
				if (arr[k] != 0) {
					fprintf(outfp, "%d ", arr[k]);
				}
			}
			fprintf(outfp, "\n");
		}
		else {
			break;
		}
	}

	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("output written to hw2_result.txt.\n");
	printf("running time: %f seconds\n", duration);

	fclose(infp);
	fclose(outfp);
	
	return 0;
}

void insert_max_heap(element item, int* n) {
	int i;
	if (HEAP_FULL(*n)) {
		fprintf(stderr, "The heap is full.\n");
		exit(1);
	}
	i = ++(*n);
	while ((i != 1) && (item.key > max_heap[i / 2].key)) {
		max_heap[i] = max_heap[i / 2];
		i /= 2;
	}
	max_heap[i] = item;
}

void insert_min_heap(element item, int* n) {
	int i;
	if (HEAP_FULL(*n)) {
		fprintf(stderr, "The heap is full.\n");
		exit(1);
	}
	i = ++(*n);
	while ((i != 1) && (item.key < min_heap[i / 2].key)) {
		min_heap[i] = min_heap[i / 2];
		i /= 2;
	}
	min_heap[i] = item;
}

element delete_max_heap(int* n) {
	int parent, child;
	element item, temp;
	if (HEAP_EMPTY(*n)) {
		fprintf(stderr, "The heap is empty");
		exit(1);
	}
	item = sub_max_heap[1];
	temp = sub_max_heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= *n) {
		if ((child < *n) && (sub_max_heap[child].key < sub_max_heap[child + 1].key)) child++;
		if (temp.key >= sub_max_heap[child].key) break;
		sub_max_heap[parent] = sub_max_heap[child];
		parent = child;
		child *= 2;
	}
	sub_max_heap[parent] = temp;
	return item;
}

element delete_min_heap(int* n) {
	int parent, child;
	element item, temp;
	if (HEAP_EMPTY(*n)) {
		fprintf(stderr, "The heap is empty");
		exit(1);
	}
	item = sub_min_heap[1];
	temp = sub_min_heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= *n) {
		if ((child < *n) && (sub_min_heap[child].key > sub_min_heap[child + 1].key)) child++;
		if (temp.key <= sub_min_heap[child].key) break;
		sub_min_heap[parent] = sub_min_heap[child];
		parent = child;
		child *= 2;
	}
	sub_min_heap[parent] = temp;
	return item;
}
