#include "hashmap.h"
#include <stdio.h>
#include <string.h>

typedef struct Person {
	char name[50];
	int age;
} Person;

int main() {
	Hashmap* map = hashmap_create(10);

	hashmap_put(map, "sam", "altman");
	hashmap_put(map, "sam", "altman1");
	hashmap_put(map, "sam", "altman2");
	hashmap_put(map, "sam", "altman3");
	hashmap_put(map, "sam", "altman4");

	char key[50];
	for (size_t i = 0; i < 10000; i++) {
		snprintf(key, sizeof(key), "sam%zu", i);
		hashmap_put(map, key, "altman");
	}

	char* value = hashmap_get(map, "sam");
	printf("%s \n", value);

	Person y = { .age = 25 };
	strncpy(y.name, "sam", sizeof(y.name));

	hashmap_put(map, "xyz", &y);

	Person* v = hashmap_get(map, "xyz");
	printf("%s - %d\n", v->name, v->age);

	hashmap_remove(map, "two");

	char* d = hashmap_get(map, "two");
	printf("%s \n", d);

	hashmap_print(map);
	hashmap_free(map);

	return EXIT_SUCCESS;
}
