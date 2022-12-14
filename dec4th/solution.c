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

	for (int i = 0, c = 0; i < len && c < 3; i++) {
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

void fully_contained(range_t *r1, range_t *r2, void *data) {
	int *data_p = (int*) data;
	if (r1->end >= r2->end && r1->start <= r2->start) {
		*data_p += 1;
	} else if (r2->end >= r1->end && r2->start <= r1->start) {
		*data_p += 1;
	}
}

int fits_range(range_t *r1, int num, int i) {
  int result = num >= r1->start && num <= r1->end;
  // if (result) {
  // print_range(r1);
  // printf("\t[%d] %d\n", i, num);  
  // }
  return result;
}

void partially_contained(range_t *r1, range_t *r2, void *data) {
  int *data_p = (int*) data;
  if (!((fits_range(r1, r2->start,1) || fits_range(r1, r2->end,2)) &&
    (fits_range(r2, r1->start,3) || fits_range(r2, r2->end,4)))) {
    printf("contained:\n");
    print_range(r1);
    print_range(r2);

    *data_p += 1;
    
    } 
  
}

int main(void) {
	FILE *fp = fopen("./input2.txt", "r");
	if (fp == NULL) return 1;

	int result = 0;

	send_ranges_to_file(fp, &fully_contained, &result);
	printf("PART ONE:\n\t%d\n", result);

  result = 0;
  rewind(fp);

  send_ranges_to_file(fp, &partially_contained, &result);
	printf("PART TWO:\n\t%d\n", result);
}