#ifndef __ADVENT_H__INCLUDE__
#	define __ADVENT_H__INCLUDE__

#	include <stdlib.h>
#	include <stdio.h>

#	ifdef __cplusplus
extern "C"
{
#	endif

	typedef struct hash_map_node_t
	{
		struct hash_map_node_t *next;
		int key;
		int value;
	} hash_map_node_t;

	typedef struct hash_map_t
	{
		hash_map_node_t **raw;
		size_t capacity;
		size_t length;
	} hash_map_t;

	typedef void (*foreach_callback)(int index, int key, int value);

	size_t __hash(unsigned int x);

	hash_map_t init(size_t max_capacity);

	hash_map_node_t *pair(int key, int value);

	int *get(hash_map_t *map, int key);

	int qinsert(hash_map_t *map, int key, int value);

	int insert(hash_map_t *map, int key, int value, int *swap);

	void print_hashmap(hash_map_t *map);

	int index_checked(int *array, size_t i, size_t length, int *dest);

	void foreach (hash_map_t *map, foreach_callback cb);

	void cleanup(hash_map_t *map);

#	ifdef __TEST_MEMORY_LEAKS__
	void test_memory_leaks()
	{
		int i = 100000;

		while (i > 0)
		{
			hash_map_t map = init(20);

			insert(&map, 10, 5, NULL);
			insert(&map, 7, 2, NULL);
			insert(&map, 14, 5, NULL);
			insert(&map, 88, -50, NULL);
			insert(&map, 6, 47, NULL);
			insert(&map, 3, 33, NULL);
			insert(&map, -1001, 1, NULL);

			insert(&map, 900, -1, NULL);
			insert(&map, 805, -2, NULL);
			insert(&map, 444, -3, NULL);
			insert(&map, 404, -4, NULL);
			insert(&map, 20, -5, NULL);

			insert(&map, 7, 3, NULL);

#	ifndef LEAK_MEMORY
#		error LEAK_MEMORY is not defined: set to 1 (leak memory) or 0 (cleanup) to change the behavior of this test.
#	elif LEAK_MEMORY == 1
		cleanup(&map);
#	endif
		}
	}
#	endif /* __TEST_MEMORY_LEAKS__ */

#	ifdef __cplusplus
} /* extern "C" */
#	endif

#endif /* __ADVENT_H_INCLUDE__ */