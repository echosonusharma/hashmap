#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 100
#define LOAD_FACTOR 0.75

typedef struct Node {
	char* key;
	void* value;
	struct Node* next;
} Node;

typedef struct Hashmap {
	Node* hashmap[HASHMAP_SIZE];
	size_t size;
} Hashmap;

inline static void check_null(void* value) {
	if (value == NULL) {
		fprintf(stderr, "OUT OF RAM - MEM ALLOCATION FAILED");
		exit(1);
	}
}

static Node* node_create(void* key, void* value) {
	Node* node = malloc(sizeof(Node));
	check_null(node);
	// +1 for null termination char
	node->key = malloc(sizeof(char) * (strlen(key) + 1));
	check_null(node->key);
	strcpy_s(node->key, sizeof(char) * strlen(key) + 1, key);
	node->value = value;
	node->next = NULL;
	return node;
}

static size_t hash(char* key) {
	size_t hash = 0;

	for (size_t i = 0; i < strlen(key); i++) {
		hash += (size_t)((int)key[i] * 10) / 3;
	}

	return hash % HASHMAP_SIZE;
}

Hashmap* hashmap_create() {
	Hashmap* map = malloc(sizeof(Hashmap));
	check_null(map);

	for (size_t i = 0; i < HASHMAP_SIZE; i += 1) {
		map->hashmap[i] = NULL;
	}

	map->size = 0;

	return map;
}

size_t hashmap_size(Hashmap* map) {
	return map->size;
}

int hashmap_put(Hashmap* map, char* key, void* value) {
	size_t index = hash(key);
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
	size_t index = hash(key);
	Node* current = map->hashmap[index];

	while (current != NULL && strcmp(current->key, key) != 0) {
		current = current->next;
	}

	return current == NULL ? NULL : current->value;
}

int hashmap_remove(Hashmap* map, char* key) {
	size_t index = hash(key);
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
	for (size_t i = 0; i < HASHMAP_SIZE; i++) {
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
	for (size_t i = 0; i < HASHMAP_SIZE; i++) {
		Node* current_node = map->hashmap[i];

		if (current_node == NULL) {
			printf("%zu -> %d \n", i, 0);
			continue;
		}

		printf("%zu - %s -> %d ", i, current_node->key, 1);

		while (current_node != NULL && current_node->next != NULL) {
			printf("%d ", 1);
			current_node = current_node->next;
		}

		printf("\n");
	}
}

typedef struct Person {
	char name[50];
	int age;
} Person;

void main() {
	Hashmap* x = hashmap_create();
	printf("current size is - %zu \n", hashmap_size(x));


	if (hashmap_put(x, "two", "fuck the police") != 0) {
		fprintf(stderr, "error: index already in use");
		exit(1);
	}

	if (hashmap_put(x, "two", "fuck the police1") != 0) {
		fprintf(stderr, "error: index already in use");
		exit(1);
	}

	char* value = hashmap_get(x, "two");
	printf("%s \n", value);

	Person y;
	strcpy_s(y.name, sizeof(y.name), "Sam");
	y.age = 25;

	if (hashmap_put(x, "x", &y) != 0) {
		fprintf(stderr, "error: index already in use");
		exit(1);
	}

	Person* v = hashmap_get(x, "x");
	printf("%s - %d\n", v->name, v->age);


	//if (hashmap_remove(x, "two") != 0) {
	//	fprintf(stderr, "error: index already in use");
	//	exit(1);
	//}

	char* d = hashmap_get(x, "two");
	printf("%s \n", d);

	printf("current size is - %zu \n", hashmap_size(x));

	//printf("%zu \n", hash("hello"));
	//printf("%zu \n", hash("asd"));
	//printf("%zu \n", hash("hlo"));
	//printf("%zu \n", hash("hlo"));

	hashmap_print(x);
	hashmap_free(x);
}
