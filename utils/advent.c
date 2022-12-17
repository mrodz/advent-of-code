#include "advent.h"

size_t __hash(unsigned int x)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x;
}

hash_map_t init(size_t max_capacity)
{
	hash_map_t result;

	hash_map_node_t **node_base = (hash_map_node_t **)malloc(sizeof(hash_map_node_t *) * max_capacity); // MUST cleanup.

	result.raw = node_base;
	result.capacity = max_capacity;
	result.length = 0;

	for (int i = 0; i < max_capacity; i++)
	{
		result.raw[i] = NULL;
	}

	return result;
}

hash_map_node_t *pair(int key, int value)
{
	hash_map_node_t *result = (hash_map_node_t *)malloc(sizeof(hash_map_node_t)); // MUST cleanup.

	result->key = key;
	result->value = value;
	result->next = NULL;

	return result;
}

int *get(hash_map_t *map, int key)
{
	size_t maybe_position = __hash(key) % map->capacity;

	if (map->raw[maybe_position] == NULL)
	{
		return NULL;
	}

	hash_map_node_t *child = map->raw[maybe_position];

	while (1)
	{
		if (child == NULL)
		{
			return NULL;
		}

		if (child->key == key)
		{
			// *dest = child->value;
			return &(child->value);
		}

		child = child->next;
	}
}

int qinsert(hash_map_t *map, int key, int value) {
	insert(map, key, value, NULL);
}

int insert(hash_map_t *map, int key, int value, int *swap)
{
	size_t maybe_position = __hash(key) % map->capacity;
	map->length += 1;

	if (map->raw[maybe_position] == NULL)
	{
		map->raw[maybe_position] = pair(key, value);
		return 0;
	}

	hash_map_node_t *child = map->raw[maybe_position];

	while (1)
	{
		if (child->key == key)
		{
			if (swap != NULL)
			{
				*swap = child->value;
			}

			child->value = value;
			return 0;
		}

		if (child->next == NULL)
		{
			child->next = pair(key, value);
			return 0;
		}

		child = child->next;
	}
}

void print_hashmap(hash_map_t *map)
{
	for (int i = 0; i < map->capacity; i++)
	{
		if (map->raw[i] != NULL)
		{

			int key = map->raw[i]->key;
			int value = map->raw[i]->value;

			printf("[%d] { %d -> %d }", i, key, value);

			hash_map_node_t *child = map->raw[i];

			while (child->next != NULL)
			{
				int key = child->next->key;
				int value = child->next->value;

				printf(" + { %d -> %d }", key, value);
				child = child->next;
			}

			printf("\n");
		}
		else
		{
			printf("[%d] ---\n", i);
		}
	}
}


int index_checked(int *array, size_t i, size_t length, int *dest)
{
	if (i >= length)
		return 0;

	*dest = array[i];

	return 1;
}

void foreach (hash_map_t *map, foreach_callback cb)
{
	for (int i = 0, c = 0; i < map->length; i++)
	{
		if (map->raw[i] == NULL)
			continue;

		hash_map_node_t *child = map->raw[i];

		while (child != NULL)
		{
			(*cb)(c++, child->key, child->value);
			child = child->next;
		}
	}
}

void cleanup(hash_map_t *map)
{
	for (int i = 0; i < map->capacity; i++)
	{
		if (map->raw[i] == NULL)
			continue;

		hash_map_node_t *child = map->raw[i];

		while (child != NULL)
		{
			hash_map_node_t *temp = child->next;
			free(child);
			child = temp;
		}
	}

	free(map->raw);
}