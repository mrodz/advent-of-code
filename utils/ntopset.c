#include <stdlib.h>
#include <stdio.h>

typedef struct ntopset_item
{
	int n;
	void *item;
	struct ntopset_item *next;
	struct ntopset_item *prev;
} ntopset_item;

typedef struct ntopset_t
{
	size_t size;
	size_t length;
	ntopset_item *items;
	// ntopset_item *end_ptr;
} ntopset_t;

ntopset_t new_ntopset(size_t n)
{
	ntopset_t result;
	// ntopset_item *items = (ntopset_item *)malloc(sizeof(ntopset_item) * n); // MUST be freed
	result.items = NULL;
	// result.end_ptr = NULL;
	result.size = n;
	result.length = 0;

	// for (int i = 0; i < n; i++)
	// {
	// 	items[i] = NULL;
	// }

	return result;
}

ntopset_item *nth(ntopset_item *item, size_t n)
{
	int i = 0;
	ntopset_item *result = item;

	for (int i = 0; i < n; i++)
	{
		result = result->next;
	}

	return result;
}

ntopset_item *new_item(int n, void *item)
{
	ntopset_item *result = (ntopset_item *)malloc(sizeof(ntopset_item));
	result->n = n;
	result->item = item;
	result->next = NULL;
	result->prev = NULL;
	return result;
}

void insert_between(ntopset_item *new, ntopset_item *first, ntopset_item *second, size_t *counter)
{
	new->next = second;
	first->next = new;
	*counter += 1;
}

void add_item(ntopset_t *set, ntopset_item *item)
{
	if (set->items == NULL)
	{
		set->items = item;
		// set->end_ptr = item;
		set->length += 1;
		return;
	}

	// ntopset_item *middle = nth(set->items, set->length / 2);

	// if (middle != NULL)
	// {
	// if (middle->n == item->n)
	// {
	// 	// middle -> item -> middle.next
	// 	insert_between(item, middle, &middle->next, &set->length);
	// 	// item->next = middle->next;
	// 	// middle->next = item;
	// 	// set->length += 1;
	// 	return;
	// }

	ntopset_item *prev = set->items;

	// if (middle->n > item->n)
	// { // item is less than the middle.
	// 	prev = set->items;
	// } else {
	// 	prev =
	// }

	for (int i = 0; i < set->length; i++)
	{
		if (item->n > prev->n && item->n < prev->next->n)
		{ // item fits between the previous and next.
			insert_between(item, prev, prev->next, &set->length);
			// item->next = prev->next;
			// prev->next = item;

			return;
		}

		prev = prev->next;
	}

	printf("<obj at %p> with value %d WAS NOT ADDED\n", item, item->n);
	// }
	// else
	// { // item is greater than the middle.
	// 	size_t last_index = set->length - 1;
	// 	ntopset_item *last = &set->items[last_index];

	// 	for (int i = last_index; i >= 0; i--) {

	// 	}
	// 	// for (int i = set->length; i )
	// }
	// }
	// else
	// {
	// 	printf("middle is null (shouldn't happen)\n");
	// }
}

void print_items(ntopset_t *set)
{
	ntopset_item *item = set->items;
	for (int i = 0; i < set->length; i++)
	{
		printf("<obj at %p> with value %d\n", item, item->n);
		item = item->next;
	}
}

int main(void)
{
	ntopset_t set = new_ntopset(3);

	add_item(&set, new_item(10, "A"));
	add_item(&set, new_item(20, "B"));
	add_item(&set, new_item(300, "C"));
	add_item(&set, new_item(-2, "D"));

	print_items(&set);
}