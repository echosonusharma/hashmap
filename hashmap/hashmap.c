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
	node->key = malloc(sizeof(char) * (strlen(key) + 1));
	check_null(node->key);
	strcpy_s(node->key, sizeof(node->key), key);
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

	if (current != NULL) {
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node_to_add;
	}
	else {
		map->hashmap[index] = node_to_add;
	}

	map->size++;

	return 0;
}

int hashmap_get(Hashmap* map, char* key, void** value) {
	size_t index = hash(key);
	Node* current = map->hashmap[index];

	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			*value = current->value;
			return 0;
		}
		current = current->next;
	}

	return 1;
}

int hashmap_remove(Hashmap* map, char* key) {
	size_t index = hash(key);
	Node* current = map->hashmap[index];

	if (current == NULL) {
		return 1;
	}

	return 0;
}

typedef struct Person {
	char name[50];
	int age;
} Person;

void main() {
	Hashmap* x = hashmap_create();

	if (hashmap_put(x, "two", "fuck the police") != 0) {
		fprintf(stderr, "error: index already in use");
		exit(1);
	}

	void* value;

	if (hashmap_get(x, "two", &value) != 0) {
		fprintf(stderr, "error: value not found");
		exit(1);
	}
	else {
		printf("%s \n", (char*)value);
	}

	Person y;
	strcpy_s(y.name, sizeof(y.name), "Sam");
	y.age = 25;

	if (hashmap_put(x, "x", &y) != 0) {
		fprintf(stderr, "error: index already in use");
		exit(1);
	}

	void* v;

	if (hashmap_get(x, "x", &v) != 0) {
		fprintf(stderr, "error: value not found");
		exit(1);
	}
	else {
		Person* a = (Person*)v;
		printf("%s \n", a->name);
	}


	printf("%zu \n", hash("hello"));
	printf("%zu \n", hash("asd"));
	printf("%zu \n", hash("hlo"));
	printf("%zu \n", hash("hlo"));
}
