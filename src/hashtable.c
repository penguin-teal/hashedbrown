#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// If no specific algorithm is specified default to FNV-1A
#if !defined(FNV1A) && !defined(FNV1)
#define FNV1A
#endif

#if defined(FNV1A) || defined(FNV1)

#define SEED    0x811C9DC5
#define PRIME   0x01000193

#endif

#ifndef NOSTDERR
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#else
#define ERR(...)
#endif

typedef struct Hashtable
{
    size_t itemsSize;
    int itemCount;
    struct HashtableItem *items;

    size_t dataSize;
    size_t dataCountBytes;
    void *data;

    unsigned long tableSize;
    int *table;
    unsigned long tableCount;

    size_t overflowSize;
    int *overflow;
    int overflowCount;
} hashtable_T;

struct HashtableItem
{
    int hash;
    int sum;
    size_t keyOffset;
    size_t valueOffset;
    size_t valueMaxSize;
};

static inline int hashChar(char value, int oldHash)
{
#ifdef FNV1A
    return (value ^ oldHash) * PRIME;
#elif FNV1
    return (value * PRIME) ^ oldHash;
#endif
}

static inline int hashString(int tableSize, const char *value, size_t size)
{
    int hash = SEED;

    for(size_t i = 0; i < size; i++)
    {
        hash = hashChar(value[i], hash);
    }

    return (hash & 0x7FFFFFFF) % tableSize;
}

static inline size_t toQWordBoundary(size_t inSize)
{
    return inSize - inSize % sizeof(long int) + sizeof(long int);
}

static inline int sumKey(const char *key, size_t keySize)
{
    int sum = 0;
    for(size_t i = 0; i < keySize; i++) sum += key[i];
    return sum;
}

static size_t pushDataBuffer(hashtable_T *ht, const char *buffer, size_t requestSize, size_t bufferSize)
{
    size_t needed = ht->dataCountBytes + requestSize;
    size_t has = ht->dataSize;
    if(needed > has)
    {
        // Double the buffer unless that isn't enough
        if(has * 2 >= needed) has *= 2;
        else has = needed;

        void *newData = realloc(ht->data, has);
        if(!newData)
        {
            ERR(
                "Failed to allocate %lu B for data in hash table.\n",
                has
            );
            return SIZE_MAX;
        }

        ht->dataSize = has;
        ht->data = newData;
    }

    size_t offset = ht->dataCountBytes;
    memcpy(ht->data + offset, buffer, bufferSize);
    ht->dataCountBytes += requestSize;
    return offset;
}

static int pushItem(hashtable_T *ht, const char *key, size_t keySize, const char *valueBuffer, size_t valueSize, int hash)
{
    size_t needed = (ht->itemCount + 1) * sizeof(struct HashtableItem);
    size_t has = ht->itemsSize;
    if(needed > has)
    {
        // Double the buffer unless that isn't enough
        if(has * 2 >= needed) has *= 2;
        else has = needed;

        void *newItems = realloc(ht->items, has);
        if(!newItems)
        {
            ERR(
                "Failed to allocate %lu B for items in hash table.\n",
                has
            );
            return INT_MAX;
        }

        ht->itemsSize = has;
        ht->items = newItems;
    }

    size_t valueRequestSize = toQWordBoundary(valueSize);
    size_t dataOffset = pushDataBuffer(ht, valueBuffer, valueRequestSize, valueSize);
    if(dataOffset == SIZE_MAX) return false;

    int sum = sumKey(key, keySize);
    size_t keyOffset = pushDataBuffer(ht, key, toQWordBoundary(keySize), keySize);
    if(keyOffset == SIZE_MAX) return false;

    struct HashtableItem kv =
    {
        .hash = hash,
        .sum = sum,
        .keyOffset = keyOffset,
        .valueOffset = dataOffset,
        .valueMaxSize = valueRequestSize
    };
    int offset = ht->itemCount++;
    ht->items[offset] = kv;
    return offset;
}

static inline struct HashtableItem *getItemFromIndex(hashtable_T *ht, int item)
{
    return ht->items + item;
}

static inline char *getItemKey(hashtable_T *ht, int item)
{
    return (char*)ht->data + getItemFromIndex(ht, item)->keyOffset;
}

static inline char *getItemValue(hashtable_T *ht, int item)
{
    return (char*)ht->data + getItemFromIndex(ht, item)->valueOffset;
}

static inline bool collisionInsert(hashtable_T *ht, int item)
{
    size_t needed = (ht->overflowCount + 1) * sizeof(int);
    size_t has = ht->overflowSize;
    if(needed > has)
    {
        // Double the buffer unless that isn't enough
        if(has * 2 >= needed) has *= 2;
        else has = needed;

        int *newOverflow = realloc(ht->overflow, has);
        if(!newOverflow)
        {
            ERR(
                "Failed to allocate %lu B for overflow in hash table.\n",
                has
            );
            return false;
        }

        ht->overflowSize = has;
        ht->overflow = newOverflow;
    }

    ht->overflow[ht->overflowCount++] = item;
    return true;
}

bool htSetBuffer(hashtable_T *ht, const char *key, size_t keySize, const char *value, size_t valueSize)
{
    int hash = hashString(ht->tableSize, key, keySize);
    int item = pushItem(ht, key, keySize, value, valueSize, hash);

    int currentItem = ht->table[hash];
    if(!currentItem)
    {
        if(ht->tableCount == ht->tableSize)
        {
            ERR("Hashtable is full.\n");
            return false;
        }

        ht->table[hash] = item;
        ht->tableCount++;
    }
    else
    {
        char *currentItemKey = getItemKey(ht, currentItem);
        if(!strcmp(currentItemKey, key))
        {
            struct HashtableItem *htItem = getItemFromIndex(ht, currentItem);
            if(valueSize <= htItem->valueMaxSize)
            {
                strncpy(getItemValue(ht, currentItem), value, htItem->valueMaxSize);
            }
            else
            {
                // This is going to just abandon old value memory
                size_t valueRequestSize = toQWordBoundary(valueSize);
                int offset = pushDataBuffer(ht, value, valueRequestSize, valueSize);
                htItem->valueOffset = offset;
                htItem->valueMaxSize = valueRequestSize;
            }
        }
        else
        {
            return collisionInsert(ht, item);
        }
    }

    return true;
}

char *htGetBuffer(hashtable_T *ht, const char *key, size_t keySize)
{
    int hash = hashString(ht->tableSize, key, keySize);
    int sum = sumKey(key, keySize);

    int item = ht->table[hash];
    if(item == 0) return NULL;

    struct HashtableItem *hItem = getItemFromIndex(ht, item);

    if(hItem ->sum == sum) return getItemValue(ht, item);
    else
    {
        int overflowCount = ht->overflowCount;
        for(int i = 0; i < overflowCount; i++)
        {
            int oItem = ht->overflow[i];
            struct HashtableItem *overflowedItem = getItemFromIndex(ht, oItem);
            if(overflowedItem->hash == hash &&
               overflowedItem->sum == sum) return getItemValue(ht, oItem);
        }

        // Nothing was found
        return NULL;
    }
}

bool htTableReset(hashtable_T *ht)
{
    int *newTable = calloc(ht->tableSize, sizeof(int));
    if (!newTable)
    {
        ERR(
            "Failed to allocate %lu B of new zeroed "
            "memory for table in hash table.\n",
            ht->tableSize * sizeof(int)
        );
        return false;
    }
    else
    {
        ht->itemCount = 1;
        ht->dataCountBytes = 0;
        ht->overflowCount = 0;
        ht->tableCount = 0;

        free(ht->table);
        ht->table = newTable;
    }

    return true;
}

void htTableDestroy(hashtable_T *ht)
{
    free(ht->table);
    free(ht->items);
    free(ht->overflow);
    free(ht->data);
    free(ht);
}

hashtable_T *htTableCreate(unsigned long size)
{
    hashtable_T *ht = malloc(sizeof(hashtable_T));
    if(!ht)
    {
        ERR(
            "Failed to allocate initial %lu B of "
            "memory for hash table.",
            sizeof(hashtable_T)
        );
        return NULL;
    }
    
    // We want to use 0 as NULL, so don't let it
    // be a valid item
    ht->itemCount = 1;
    // Arbitrary starting capacity
    ht->itemsSize = toQWordBoundary(size / 6);
    ht->items = malloc(ht->itemsSize);
    if(!ht->items)
    {
        ERR(
            "Failed to allocate initial %lu B of "
            "memory for items in hash table.\n",
            ht->itemsSize
        );
        free(ht);
        return NULL;
    }

    ht->dataCountBytes = 0;
    // Arbitrary starting capacity
    ht->dataSize = toQWordBoundary(size / 2);
    ht->data = malloc(ht->dataSize);
    if (!ht->data)
    {
        ERR(
            "Failed to allocate initial %lu B of "
            "memory for data in hash table.\n",
            ht->dataSize
        );
        free(ht->items);
        free(ht);
        return NULL;
    }

    ht->overflowCount = 0;
    ht->overflowSize = 32;
    ht->overflow = malloc(ht->overflowSize);
    if (!ht->overflow)
    {
        ERR(
            "Failed to allocate initial %lu B of "
            "memory for overflow in hash table.\n",
            ht->overflowSize
        );
        free(ht->items);
        free(ht->data);
        free(ht);
        return NULL;
    }

    ht->tableCount = 0;
    ht->tableSize = size;
    ht->table = calloc(size, sizeof(int));
    if (!ht->table)
    {
        ERR(
            "Failed to allocate initial %lu B of "
            "zeroed memory for table in hash table.\n",
            size * sizeof(int)
        );
        free(ht->items);
        free(ht->data);
        free(ht->overflow);
        free(ht);
        return NULL;
    }

    return ht;
}

bool htDeleteBuffer(hashtable_T *ht, const char *key, size_t keySize)
{
    int hash = hashString(ht->tableSize, key, keySize);
    int sum = sumKey(key, keySize);

    int item = ht->table[hash];
    if(item == 0) return false;

    struct HashtableItem *hItem = getItemFromIndex(ht, item);
    
    if(hItem ->sum == sum)
    {
        ht->table[hash] = 0;
        return true;
    }
    else
    {
        int overflowCount = ht->overflowCount;
        for(int i = 0; i < overflowCount; i++)
        {
            int oItem = ht->overflow[i];
            struct HashtableItem *overflowedItem = getItemFromIndex(ht, oItem);
            if(overflowedItem->hash == hash &&
               overflowedItem->sum == sum)
            {
                ht->overflow[i] = 0;
                return true;
            }
        }

        // Nothing was found
        return false;
    }
}

char *htNextKey(hashtable_T *ht, int *start)
{
    int maxIndex = (int)ht->tableSize;

    int atIndex = *start + 1;

    int *table = ht->table;
    for(; atIndex < maxIndex; atIndex++)
    {
        if(table[atIndex])
        {
            *start = atIndex;
            return getItemKey(ht, table[atIndex]);
        }
    }

    int maxOverflowIndex = maxIndex + ht->overflowSize / sizeof(int);
    int *overflow = ht->overflow;

    for(; atIndex < maxOverflowIndex; atIndex++)
    {
        if(overflow[atIndex - maxIndex])
        {
            *start = atIndex;
            return getItemKey(ht, overflow[atIndex - maxIndex]);
        }
    }

    *start = maxIndex;
    return NULL;
}