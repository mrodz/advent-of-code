#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct range_t
{
	int start;
	int end;
} range_t;

range_t *from_str(char *str)
{
	range_t *result = malloc(sizeof(range_t) * 2);
	if (result == NULL)
		exit(EXIT_FAILURE);
	int i = 0;
	size_t len = strlen(str);

	int non_int_at[3];

	for (int i = 0, c = 0; i < len && c < 3; i++)
	{
		if (!isdigit(str[i]))
		{
			non_int_at[c++] = i;
		}
	}

	// This code takes advantage of the fact that atoi() will exit
	// early when it encounters a non-digit. Thus, since we know the positions
	// of the non-digits, we can move around them accordingly.
	result[0].start = atoi(&str[0]);
	result[0].end = atoi(&str[non_int_at[0] + 1]);

	result[1].start = atoi(&str[non_int_at[1] + 1]);
	result[1].end = atoi(&str[non_int_at[2] + 1]);

	return result;
}

void print_range(range_t *range)
{
	printf("Range (start = %d, end = %d)\n", range->start, range->end);
}

int send_ranges_from_file(FILE *fp, void (*part)(range_t *, range_t *, void *), void *result)
{
	if (fp == NULL)
		return 1;

	char destination[20];
	while (fgets(destination, sizeof destination, fp) != NULL)
	{
		if (feof(fp))
			return 0;
		range_t *ranges = from_str(destination);
		(*part)(&ranges[0], &ranges[1], result);
		free(ranges);
	}

	return 0;
}

void fully_contained(range_t *r1, range_t *r2, void *data)
{
	int *data_p = (int *)data;

	int r2_in_r1 = r1->end >= r2->end && r1->start <= r2->start;
	int r1_in_r2 = r2->end >= r1->end && r2->start <= r1->start;

 	if (r2_in_r1 || r1_in_r2)
	{
		*data_p += 1;
	}
}

static inline int fits_range(range_t *r1, int num)
{
	int result = num >= r1->start && num <= r1->end;
	return result;
}

void partially_contained(range_t *r1, range_t *r2, void *data)
{
	int *data_p = (int *)data;

	if (fits_range(r1, r2->start) || fits_range(r1, r2->end) || fits_range(r2, r1->start) || fits_range(r2, r1->end))
	{
		*data_p += 1;
	}

	return;
}

int main(void)
{
	FILE *fp;

	errno_t error_code = fopen_s(&fp, "./input.txt", "r");

	if (error_code != 0)
	{
		printf("Error! Failed to open file in r mode!\n");
		exit(EXIT_FAILURE);
	}

	if (fp == NULL)
		return 1;

	int result = 0;

	send_ranges_to_file(fp, &fully_contained, &result);
	printf("PART ONE:\n\t%d\n", result);

	result = 0;
	rewind(fp);

	send_ranges_to_file(fp, &partially_contained, &result);
	printf("PART TWO:\n\t%d\n", result);
}