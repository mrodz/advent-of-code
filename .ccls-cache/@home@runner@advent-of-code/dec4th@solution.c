#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct range_t {
	int start;
	int end;
} range_t;

static int _c = 0;

range_t *from_str(char *str) {
	range_t *result = malloc(sizeof(range_t) * 2);
	if (result == NULL) exit(-1);
	int i = 0;
	size_t len = strlen(str);

	int non_int_at[3];

	for (int i = 0, c = 0; i < len; i++) {
		if (!isdigit(str[i])) {
			non_int_at[c++] = i;
		}
	}

	result[0].start = atoi(&str[0]);
	result[0].end = atoi(&str[non_int_at[0] + 1]);
	
	result[1].start = atoi(&str[non_int_at[1] + 1]);
	result[1].end = atoi(&str[non_int_at[2] + 1]);

	return result;
}

void print_range(range_t *range) {
	printf("Range (start = %d, end = %d)\n", range->start, range->end);
}

int send_ranges_to_file(FILE *fp, void(*part)(range_t*, range_t*, void*), void *result) {
	if (fp == NULL) return 1;

	char destination[20];
	while (fgets(destination, sizeof destination, fp) != NULL) {
		if (feof(fp)) return 0;
		range_t *ranges = from_str(destination);
		_c++;
		(*part)(&ranges[0], &ranges[1], result);
		free(ranges);
	}

	return 0;
}

void duplicate_ranges(range_t *r1, range_t *r2, void *data) {
	int *data_p = (int*) data;
	if (r1->end >= r2->end && r1->start <= r2->start) {
		*data_p += 1;
	} else if (r2->end >= r1->end && r2->start <= r1->start) {
		*data_p += 1;
	}
}

int main(void) {
	FILE *fp = fopen("./input.txt", "r");
	if (fp == NULL) return 1;

	int result = 0;

	send_ranges_to_file(fp, &duplicate_ranges, &result);

	printf("PART ONE:\n\t%d\n", result);
}