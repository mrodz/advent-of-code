#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

scores_t from_char_s(char *c) {
	switch (*c) {
		case 'X':
			return LOSS;
		case 'Y':
			return TIE;
		case 'Z':
			return WIN;
	}
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

// Returns the player that p1 looses to.
players_t looser_from_player(players_t *p1) {
	switch (*p1) {
		case SCISSORS:
			return ROCK;
		case ROCK:
			return PAPER;
		case PAPER:
			return SCISSORS;
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

uint32_t score_from_round(players_t us, players_t them) {
	if (us == them) return TIE + us;
	
	players_t victor = victor_from_player(&us);

	if (victor == them) return WIN + us;
	
	return LOSS + us;
}

void part_one(char c1, char c2, void *data) {
	int *i_data = (int*) data;
	int score = score_from_round(from_char(&c2), from_char(&c1)); 
	*i_data += score;
}

void part_two(char c1, char c2, void *data) {
	int *data_p = (int*) data;

	players_t us;
	players_t them = from_char(&c1);

	scores_t required_score = from_char_s(&c2);

	switch (required_score) {
		case TIE:
			us = them;
			break;
		case WIN:
			us = looser_from_player(&them);
			break;
		case LOSS:
			us = victor_from_player(&them);
			break;
	}

	uint32_t score = score_from_round(us, them);
	
	*data_p += score;
}

int main() {
	FILE *fp;
	fp = fopen("./input.txt", "r");

	int result = 0;

	send_players_to_fn(fp, &part_one, &result);
	printf("PART ONE:\n\t%d", result);

	result = 0;
	rewind(fp);

	send_players_to_fn(fp, &part_two, &result);
	printf("\nPART TWO:\n\t%d", result);

	fclose(fp);
}