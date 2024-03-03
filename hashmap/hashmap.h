#ifndef HASHMAP_H
#define HASHMAP_H

// prevent name mangling
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	typedef struct Node {
		char* key;
		void* value;
		struct Node* next;
	} Node;

	typedef struct Hashmap {
		Node** hashmap;
		size_t size;
		size_t capacity;
	} Hashmap;

	static Node* node_create(void* key, void* value) {
		Node* node = (Node*)malloc(sizeof(Node));
		if (node == NULL) {
			return NULL;
		}
		// +1 is for null termination char
		node->key = (char*)malloc(sizeof(char) * (strlen((char*)key) + 1));
		if (node->key == NULL) {
			free(node);
			return NULL;
		}
		strcpy_s(node->key, sizeof(char) * strlen((char*)key) + 1, (char*)key);
		node->value = value;
		node->next = NULL;
		return node;
	}

	static inline size_t calculateIndex(size_t capacity, size_t hash) {
		return hash & (capacity - 1);
	}

	// http://www.cse.yorku.ca/~oz/hash.html
	static size_t hash(Hashmap* map, char* key) {
		unsigned long h = 5381;
		int c;

		while (c = *key++)
			h = ((h << 5) + h) + c; /* hash * 33 + c */

		return calculateIndex(map->capacity, h);
	}

	Hashmap* hashmap_create(size_t initialCapacity) {
		Hashmap* map = (Hashmap*)malloc(sizeof(Hashmap));
		if (map == NULL) {
			return NULL;
		}
		map->capacity = initialCapacity;
		map->hashmap = (Node**)calloc(map->capacity, sizeof(Node*));
		if (map->hashmap == NULL) {
			free(map);
			return NULL;
		}
		map->size = 0;

		return map;
	}

	int hashmap_put(Hashmap* map, char* key, void* value) {
		size_t index = hash(map, key);
		Node* current = map->hashmap[index];
		Node* node_to_add = node_create(key, value);

		if (current == NULL) {
			map->hashmap[index] = node_to_add;
			map->size += 1;
			return 0;
		}

		while (current != NULL) {
			if (strcmp(current->key, key) == 0) {
				current->value = value;
				free(node_to_add);
				return 0;
			}

			if (current->next == NULL) {
				current->next = node_to_add;
				map->size += 1;
				return 0;
			}

			current = current->next;
		}

		return 1;
	}

	void* hashmap_get(Hashmap* map, char* key) {
		size_t index = hash(map, key);
		Node* current = map->hashmap[index];

		while (current != NULL && strcmp(current->key, key) != 0) {
			current = current->next;
		}

		return current == NULL ? NULL : current->value;
	}

	int hashmap_remove(Hashmap* map, char* key) {
		size_t index = hash(map, key);
		Node* current = map->hashmap[index];
		Node* node_to_remove;

		if (current == NULL) {
			return 1;
		}

		if (strcmp(current->key, key) == 0) {
			map->hashmap[index] = current->next;
			node_to_remove = current;
		}
		else {
			while (current->next != NULL && strcmp(current->next->key, key) != 0) {
				current = current->next;
			}
			if (current->next == NULL) {
				return 1;
			}

			node_to_remove = current->next;
			current->next = node_to_remove->next;
		}

		free(node_to_remove->key);
		free(node_to_remove);

		map->size -= 1;

		return 0;
	}

	void hashmap_free(Hashmap* map) {
		for (size_t i = 0; i < map->capacity; i++) {
			Node* current_node = map->hashmap[i];

			while (current_node != NULL) {
				Node* next_node = current_node->next;
				free(current_node->key);
				free(current_node);
				current_node = next_node;
			}
		}

		free(map);
	}

	void hashmap_print(Hashmap* map) {
		size_t unused_blocks = 0;
		size_t collisions = 0;

		for (size_t i = 0; i < map->capacity; i++) {
			Node* current_node = map->hashmap[i];

			if (current_node == NULL) {
				unused_blocks += 1;
				continue;
			}

			while (current_node != NULL && current_node->next != NULL) {
				collisions += 1;
				current_node = current_node->next;
			}
		}

		printf("\n");
		printf("TOTAL BLOCKS : %zu \n", map->capacity);
		printf("UNUSED BLOCKS : %zu \n", unused_blocks);
		printf("ITEMS STORED : %zu \n", map->size);
		printf("COLLISIONS : %zu (%.2f%%) \n", collisions, ((double)collisions / (double)map->size) * 100);
		printf("\n");
	}

#ifdef __cplusplus
}
#endif

#endif // HASHMAP_H