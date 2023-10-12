# hashedbrown
A fast and versatile hash table library for C to store strings, numbers, or any arbitrary buffers.

## Installation (Linux)

Hashedbrown is a small static library. Link it with `-L` and `-l` or put it into your default lib directory.

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

See `examples/exampletable.c` for examples on how to use the library. It consists of calling a function to create,
one at the end to destroy, and just a single function to get or set any entries.

You can also iterate through all keys and check if a key exists. You can also use any kind of buffer data,
but there are many helper functions to easily work with strings or any number type.