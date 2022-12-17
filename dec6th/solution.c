#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../utils/advent.c"

#define FOUND_UNIQUE 0xFEEFEE
// #define DEBUG

int all_uniq(char* array, size_t length) {
	hash_map_t hashmap = init(length);

	int should_not_change = 1;

	for (int i = 0; i < length; i++) {
		// if a key already exists (ie. a duplicate entry), 
		// `should_not_change` will take the old value.
		insert(&hashmap, array[i], 0, &should_not_change);

		if (should_not_change != 1)
			break;
	}

	cleanup(&hashmap);
	return should_not_change;
}

void part_one(char chars[4], void *data)
{
	int *data_p = (int *)data;
#ifdef DEBUG
	printf("[%c, %c, %c, %c]\n", chars[0], chars[1], chars[2], chars[3]);
#endif

	if (all_uniq(chars, 4)) {
		*data_p = FOUND_UNIQUE;
	}
	
}

int send_ranges_from_file(FILE *fp, void (*part)(char three[3], void *), void *result)
{
	if (fp == NULL)
		return 1;

	char destination[5];
	int i = 0;
	while (fgets(destination, sizeof destination, fp) != NULL)
	{
		if (feof(fp))
			return 0;

		(*part)(destination, result);
		i++;

		if (*(int *)result == FOUND_UNIQUE)
		{
			// the +3 makes sure the number returned tells where the marker ENDS
			*(int *)result = i + 3;
			return 0;
		}

		fseek(fp, -3, SEEK_CUR);
	}

	return 0;
}

int main(void)
{
	FILE *fp;

	errno_t code;
	if ((code = fopen_s(&fp, "./input.txt", "r")) != 0)
	{
		printf("File I/O returned %d", code);
		return EXIT_FAILURE;
	};

	int data = 0;

	send_ranges_from_file(fp, &part_one, &data);

	printf("PART ONE:\n\t%d", data);

	fclose(fp);
}