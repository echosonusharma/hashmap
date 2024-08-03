#ifndef SIPHASH_H
#define SIPHASH_H

#include <stddef.h> // For size_t
#include <stdint.h> // For fixed-width integers

uint64_t siphash(const uint8_t *in, size_t inlen, const uint8_t *k);
uint64_t siphash_nocase(const uint8_t *in, size_t inlen, const uint8_t *k);

#endif // SIPHASH_H
