#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(_MSC_VER)    || \
	defined(_WIN32)      || \
	defined(__MINGW32__) || \
	defined(__CYGWIN32__)
#	define LF_LEN 2
#else
#	define LF_LEN 1
#endif

typedef enum players_t {
	ROCK     = 1,
	PAPER    = 2,
	SCISSORS = 3
} players_t;

typedef enum scores_t {
	WIN  = 6,
	LOSS = 0,
	TIE  = 3
} scores_t;

static int _c = 0;

players_t from_char(char *c) {
	switch (*c) {
		case 'A':
		case 'X':
			return ROCK;
		case 'B':
		case 'Y':
			return PAPER;
		case 'C':
		case 'Z':
			return SCISSORS;
	}

	// printf("ERROR! unknown char: '%c' (ASCII = %d)\n", *c, (int) *c);
}

void print_player(players_t player, char *end) {
	switch (player) {
		case ROCK:
			printf("Rock");
			break;
		case PAPER:
			printf("Paper");
			break;
		case SCISSORS:
			printf("Scissors");
			break;
	}

	printf("%c", end != NULL ? *end : '\n');
}

// Returns the player that p1 beats.
players_t victor_from_player(players_t *p1) {
	switch (*p1) {
		case ROCK:
			return SCISSORS;
		case PAPER:
			return ROCK;
		case SCISSORS:
			return PAPER;
	}
}

int invalid_char(char c) {
	return c == 32 || c == 10 ? 1 : 0;
}

/*
Takes a file pointer (FILE *fp) for input.

Input format:
A X\n
B Z\n
...
C Z

Also takes a callback for each line. 

Callback Signature:
void function_name(char player1, char player2, void* return_data);

- player1 will be passed the first character  [A-C].
- player2 will be passed the second character [X-Z].
- return_data is data supplied by the caller that can be changed in the callback.

Lastly, takes a void pointer to data that a callback function can access.
*/
int send_players_to_fn(FILE *fp, void(*part)(char, char, void*), void *result) {
	if (fp == NULL) return 1;

	char destination[5];
	while (fgets(destination, sizeof destination, fp) != NULL) {
		if (feof(fp)) return 0;
		(*part)(destination[0], destination[2], result);
	}

	return 1;
}

uint32_t score_from_round(char *us, char *them) {
	players_t us_unwrapped   = from_char(us);
	players_t them_unwrapped = from_char(them);

	if (us_unwrapped == them_unwrapped) return TIE + us_unwrapped;
	
	players_t victor = victor_from_player(&us_unwrapped);

	if (victor == them_unwrapped) return WIN + us_unwrapped;
	
	return LOSS + us_unwrapped;
}

void part_one(char c1, char c2, void *data) {
	int *i_data = (int*) data;
	int score = score_from_round(&c2, &c1); 
	*i_data += score;
}

int main() {
	FILE *fp;
	fp = fopen("./input.txt", "r");

	int result = 0;

	send_players_to_fn(fp, &part_one, &result);

	printf("%d", result);

	fclose(fp);
}