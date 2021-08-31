#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

#define SMAX 10000000
#define PMAX 3000
char s[SMAX];
char p[PMAX];
int SSIZE = 0;
int PSIZE = 0;

int main() {

	FILE* infp_S = fopen("string.txt", "r");
	FILE* infp_P = fopen("pattern.txt", "r");
	FILE* outfp = fopen("result_naive.txt", "w");

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

	int* result = (int*)malloc(sizeof(int));
	result[0] = 0; //0 index에 패턴이 몇 번 나오는지 저정하기 위해 0으로 초기화한 이 후 서치하며 1씩 더해준다.

	int i, j;
	int index=0;

	for (i = 0; i <= SSIZE - PSIZE; i++) {
		for (j = 0; j < PSIZE; j++) {
			if (s[i + j] != p[j])
				break;
		}
		if (j == PSIZE) {
			index++;
			result = (int*)realloc(result, (index + 1) * sizeof(int));
			++result[0];
			result[index] = i;
		}
	}

	if (outfp) {
		fprintf(outfp, "%d\n", result[0]);
		for (int i = 1; i <= result[0]; i++) {
			fprintf(outfp, "%d ", result[i]);
		}
		fprintf(outfp, "\n");
		cout << "Program complete. Result saved to result_naive.txt" << "\n";
	}

	fclose(infp_S);
	fclose(infp_P);
	fclose(outfp);
	free(result);

	return 0;
}