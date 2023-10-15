#include "hashedbrown.h"
#include <string.h>

bool htKeyExists(hashtable_T *ht, const char *key)
{
    return htGetBuffer(ht, key, strlen(key) + 1) != NULL;
}

bool htDeleteKey(hashtable_T *ht, const char *key)
{
    return htDeleteBuffer(ht, key, strlen(key) + 1);
}

char *htGetString(hashtable_T *ht, const char *key)
{
    return htGetBuffer(ht, key, strlen(key) + 1);
}

void *htGetPtr(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((void**)buffer);
    else return NULL;
}

int htGetInt(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((int*)buffer);
    else return 0;
}

unsigned int htGetUInt(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned int*)buffer);
    else return 0;
}

int htGetLong(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((long*)buffer);
    else return 0;
}

unsigned int htGetULong(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned long*)buffer);
    else return 0;
}

short htGetShort(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((short*)buffer);
    else return 0;
}

unsigned short htGetUShort(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((unsigned short*)buffer);
    else return 0;
}

char htGetChar(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((char*)buffer);
    else return 0;
}

signed char htGetSChar(hashtable_T *ht, const char *key)
{
    char *buffer = htGetBuffer(ht, key, strlen(key) + 1);
    if(buffer) return *((signed char*)buffer);
    else return 0;
}

bool htSetString(hashtable_T *ht, const char *key, const char *value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

bool htSetInt(hashtable_T *ht, const char *key, int value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(int));
}

bool htSetUInt(hashtable_T *ht, const char *key, unsigned int value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned int));
}

bool htSetLong(hashtable_T *ht, const char *key, long value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(long));
}

bool htSetULong(hashtable_T *ht, const char *key, unsigned long value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned long));
}

bool htSetShort(hashtable_T *ht, const char *key, short value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(short));
}

bool htSetUShort(hashtable_T *ht, const char *key, unsigned short value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(unsigned short));
}

bool htSetChar(hashtable_T *ht, const char *key, char value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(char));
}

bool htSetSChar(hashtable_T *ht, const char *key, signed char value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)&value, sizeof(signed char));
}

bool htSetPtr(hashtable_T *ht, const char *key, const void* value)
{
    return htSetBuffer(ht, key, strlen(key) + 1, (char*)value, sizeof(void*));
}