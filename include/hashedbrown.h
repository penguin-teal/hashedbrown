#ifndef HASHEDBROWN_H

#include <stdbool.h>

typedef struct Hashtable hashtable_T;

/**
 * Gets an arbitrary value buffer from an arbitrary key buffer.
 * All other get functions are wrappers around this function.
 * @remarks Don't rely on this pointer surviving after manipulating the hash table. You should copy it over.
 * @return A pointer to the value buffer.
 */
char *htGetBuffer(hashtable_T *ht, const char *key, unsigned long keySize);

/**
 * Sets an arbitrary key buffer to an arbitrary value buffer.
 * The value buffer is copied into another buffer.
 * All other set functions are wrappers around this function.
 */
bool htSetBuffer(hashtable_T *ht, const char *key, unsigned long keySize, const char *value, unsigned long valueSize);

bool htDeleteBuffer(hashtable_T *ht, const char *key, unsigned long keySize);

bool htDeleteKey(hashtable_T *ht, const char *key);

char *htGetString(hashtable_T *ht, const char *key);

/**
 * Checks if a key exists.
 * Consider just checking if `htGetString` is `NULL` instead, since that is what this function does.
 * @return `true` if the key has a value in the hash table.
 */
bool htKeyExists(hashtable_T *ht, const char *key);

/**
 * Gets the pointer to the string.
 * @remarks Don't rely on this pointer surviving after manipulating the hash table. You should copy the value over (i.e. with `strncpy`).
 * @return Pointer to string, or `NULL` if this key doesn't exist.
 */
char *htGetString(hashtable_T *ht, const char *key);

void *htGetPtr(hashtable_T *ht, const char *key);

int htGetInt(hashtable_T *ht, const char *key);

unsigned int htGetUInt(hashtable_T *ht, const char *key);

int htGetLong(hashtable_T *ht, const char *key);

unsigned int htGetULong(hashtable_T *ht, const char *key);

short htGetShort(hashtable_T *ht, const char *key);

unsigned short htGetUShort(hashtable_T *ht, const char *key);

char htGetChar(hashtable_T *ht, const char *key);

signed char htGetSChar(hashtable_T *ht, const char *key);

bool htSetString(hashtable_T *ht, const char *key, const char *value);

bool htSetInt(hashtable_T *ht, const char *key, int value);

bool htSetUInt(hashtable_T *ht, const char *key, unsigned int value);

bool htSetLong(hashtable_T *ht, const char *key, long value);

bool htSetULong(hashtable_T *ht, const char *key, unsigned long value);

bool htSetShort(hashtable_T *ht, const char *key, short value);

bool htSetUShort(hashtable_T *ht, const char *key, unsigned short value);

bool htSetChar(hashtable_T *ht, const char *key, char value);

bool htSetSChar(hashtable_T *ht, const char *key, signed char value);

bool htSetPtr(hashtable_T *ht, const char *key, const void* value);

/**
 * Allows for iterating over all the keys in the hash table.
 * You can call this in a loop using the same pointer for `start` to iterate over all keys.
 * @param ht The hash table.
 * @param start A pointer to the starting index. For the first call, this should be `0`. This value will be set to the index of the found key.
 * @return The found key, or `NULL` if there are none more. Don't rely on this pointer surviving after table manipulation, copy it over if needed (i.e. with `strncpy`).
 */
char *htNextKey(hashtable_T *ht, int *start);

/**
 * Resets the hashtable, reallocating as little
 * memory as possible.
 * - If any allocation fails, memory is not automatically freed.
 * @return False if a memory allocation error occurred.
 */
bool htTableReset(hashtable_T *ht);

/**
 * Destroys the hashtable, freeing all memory.
 */
void htTableDestroy(hashtable_T *ht);

/**
 * Creates a new hash table in the heap.
 * - Memory is never automatically disposed. Call `htTableDestroy`.
 * @param size The size of the hash table.
 * @return A new hashtable, or `NULL` if a memory allocation failed.
 */
hashtable_T *htTableCreate(unsigned long size);

#define HASHEDBROWN_H
#endif