#include "hashedbrown.h"
#include <string.h>

bool htKeyExists(hashtable_t *ht, char *key)
{
    return htGetBuffer(ht, key, strlen(key) + 1) != NULL;
}

bool htDeleteKey(hashtable_t *ht, char *key)
{
    return htDeleteBuffer(ht, key, strlen(key) + 1);
}

char *htGetString(hashtable_t *ht, char *key)
{
    return htGetBuffer(ht, key, strlen(key) + 1);
}

void *htGetPtr(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((void**)buffer);
    else return NULL;
}

int htGetInt(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((int*)buffer);
    else return 0;
}

unsigned int htGetUInt(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned int*)buffer);
    else return 0;
}

int htGetLong(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((long*)buffer);
    else return 0;
}

unsigned int htGetULong(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned long*)buffer);
    else return 0;
}

short htGetShort(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((short*)buffer);
    else return 0;
}

unsigned short htGetUShort(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned short*)buffer);
    else return 0;
}

char htGetChar(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((char*)buffer);
    else return 0;
}

signed char htGetSChar(hashtable_t *ht, char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((signed char*)buffer);
    else return 0;
}

bool htSetString(hashtable_t *ht, char *key, char *value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

bool htSetInt(hashtable_t *ht, char *key, int value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(int));
}

bool htSetUInt(hashtable_t *ht, char *key, unsigned int value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned int));
}

bool htSetLong(hashtable_t *ht, char *key, long value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(long));
}

bool htSetULong(hashtable_t *ht, char *key, unsigned long value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned long));
}

bool htSetShort(hashtable_t *ht, char *key, short value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(short));
}

bool htSetUShort(hashtable_t *ht, char *key, unsigned short value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned short));
}

bool htSetChar(hashtable_t *ht, char *key, char value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(char));
}

bool htSetSChar(hashtable_t *ht, char *key, signed char value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(signed char));
}

bool htSetPtr(hashtable_t *ht, char *key, void* value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)value, sizeof(void*));
}