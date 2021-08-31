#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

#define SMAX 10000000
#define PMAX 3000

char s[SMAX];
char p[PMAX];
int failure[PMAX];
int SSIZE = 0;
int PSIZE = 0;

void fail();

int main() {

	FILE* infp_S = fopen("string.txt", "r");
	FILE* infp_P = fopen("pattern.txt", "r");
	FILE* outfp = fopen("result_kmp.txt", "w");

	if (!infp_S) {
		cout << "The string file does not exist." << "\n";
	}
	if (!infp_P) {
		cout << "The pattern file does not exist." << "\n";
	}

	fgets(s, SMAX, infp_S);
	fgets(p, PMAX, infp_P);

	SSIZE = strlen(s)-1;
	PSIZE = strlen(p)-1;

	fail();

	int* result = (int*)malloc(sizeof(int));
	result[0] = 0; //0 index에 패턴이 몇 번 나오는지 저정하기 위해 0으로 초기화한 이 후 서치하며 1씩 더해준다.

	int index = 0;
	int k = 0, l = 0;
	while (k < SSIZE && l < PSIZE) {
		if (s[k] == p[l]) {
			k++;
			l++;
		}
		if (l == PSIZE) {
			index++;
			result = (int*)realloc(result, (index + 1) * sizeof(int));
			++result[0];
			result[index] = k - l;
			l = failure[l - 1] + 1;
		}
		else if (k < SSIZE && p[l] != s[k]) {
			if (l != 0) {
				l = failure[l - 1] + 1;
			}
			else {
				k++;
			}
		}
	}

	if (outfp) {
		fprintf(outfp, "%d\n", result[0]);
		for (int m = 1; m <= result[0]; m++) {
			fprintf(outfp, "%d ", result[m]);
		}
		fprintf(outfp, "\n");
		cout << "Program complete. Result saved to result_kmp.txt" << "\n";
	}

	fclose(infp_S);
	fclose(infp_P);
	fclose(outfp);
	free(result);

	return 0;
}

void fail() {
	int i = 0, j;
	failure[0] = -1;
	for (j = 1; j < PSIZE; j++) {
		i = failure[j - 1];
		while ((p[j] != p[i + 1] ) && (i >= 0)) {
			i = failure[i];
		}
		if (p[j] == p[i + 1]) failure[j] = i + 1;
		else failure[j] = -1;
	}
}