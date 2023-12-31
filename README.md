[![Build Status](https://github.com/penguin-teal/hashedbrown/actions/workflows/build.yml/badge.svg)](https://github.com/penguin-teal/hashedbrown/actions/workflows/build.yml)
# hashedbrown
A fast and versatile hash table library for C to store strings, numbers, or any arbitrary buffers.

## Build

Build library into `bin/libhashedbrown.a`:
```shell
make
```
Build example program into `bin/example`:
```shell
make examples
```

## How To

The entire library revolves around a `hashtable_T*` pointer to a struct
that is passed along with each function.

The table only stores copies of whatever you pass in (as opposing to pointing
to the inputs directly) to guarantee lifetime. It dynamically allocates memory
by itself, meaning it must be destroyed when done with.

See `examples/exampletable.c` for examples on how to use the library.
It consists of calling a function to create,
one at the end to destroy, and just a single function to get or set any entries.

You can also iterate through all keys and check if a key exists. You can also use any kind of buffer data,
but there are many helper functions to easily work with strings or any number type.
