#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <hashedbrown.h>

int main()
{
    // Create hash table
    hashtable_T *table = htTableCreate(5000);

    // Check if failed (will have returned NULL)
    if(!table) return 1;

    // Set some values
    htSetString(table, "testkey", "This is the value.");
    htSetString(table, "MYKEY", "a value");

    // Get the strings and print them
    printf("----Get & Set----\n");
    printf("testkey: %s\n", htGetString(table, "testkey"));
    printf("MYKEY: %s\n", htGetString(table, "MYKEY"));

    // The pointers are fine for immediate use,
    // however after manipulating the table they may become invalid.
    // If we want to keep the string for a bit we should strncpy:
    char testkeyValue[512];
    strncpy(testkeyValue, htGetString(table, "testkey"), sizeof testkeyValue);

    // If I wanted to get a new table and don't need this one,
    // I can reuse a lot of allocated memory to not waste time:
    htTableReset(table);

    // You can also set other types
    // There is no way to "tell" what type is in a value since it is all just buffers
    // Because of this (and because this is really how you should do things anyway) only
    // put one type of data in a single table. This example just shows off different
    // types. For example, have one table that is just strings, one that is just ushorts, etc.
    htSetUShort(table, "my ushort", 32);

    // Overwrite a key
    htSetUShort(table, "my ushort", 97);

    // This will get the value as a ushort. It will trust
    // that the value actually is a ushort, even if in reality
    // you set it to be a string or something.
    // This returns 0 if the value doesn't exist.
    unsigned short b = htGetUShort(table, "my ushort");
    printf("my ushort: %hu\n", b);
    
    int *a = malloc(32);

    htSetPtr(table, "my pointer", a);

    // Delete a key
    htDeleteKey(table, "my pointer");

    // Just freeing the variable we malloced
    free(a);

    htSetUShort(table, "hello", 2);
    htSetUShort(table, "hello34894934", 93);

    printf("----Iterate------\n");
    // Iterate over every key
    int keyIndex = 0;
    char *key;
    do
    {
        key = htNextKey(table, &keyIndex);

        if(key) printf("Found (key: value) pair = (\"%s\": %u)\n", key, htGetUShort(table, key));
    }
    while(key);

    // This frees all the allocated memory
    htTableDestroy(table);

    return 0;
}