# vec

Typesafe vector data structure, written in plain C.


## Usage

To provide type safety, struct name and type (and optionally a function prefix) should be provided before inclusion of the header file:
```c
#define Self VecOfFloat // vector struct name
#define T    float // element type
#define PREFIX vec_of_float // you can also redefine the function prefix to follow your naming style
#include <vec.h> 
// `Self`, `T` and `PREFIX` have been undefined in `vec.h`
```
This setup must only be done a single time. If you need to use the same vec type in multiple header files, you'll need to create a separate header, excllusive for your vec and mark it with either include guards or `#pragma once`. This cannot be done in the library, because the same file `vec.h` is shared between multiple vector types.

After that setup you will be able to use the vec:
```c
// all functions that may fail, return the `Result` representing the error that could occure you can use the `UNROLL` macro, which will return only if result is not equal to `Ok`
VecOfFloat vec;
UNROLL(vec_of_float_init(&vec));

UNROLL(vec_of_float_push(&vec, 0)); 
UNROLL(vec_of_float_push(&vec, 1)); 
UNROLL(vec_of_float_push(&vec, 2)); 

for(float *el_ptr = vec_first(&vec); el_ptr < vec_last(&vec); el_ptr++ ) 
    *el_ptr++;

vec_of_float_uninit(&vec); 
// watch out: using uninitialized vec may cause undefined behaviour
```

See [`test.c`](./test.c) for more examples.


## Performance

At the moment, performance is slightly worse than the `std::vector` from c++, most likely due to a different memory resizing strategy. On the other hand, the resulting code size will be much smaller, as all functions are implemented only once, instead of generating a new one for the every type. This make it suitable for more restricted enviroments (especially when custom allocators will be supported).


## ToDo
 - come up with better name 
 - add support for custom allocators
 - add an ability for reserving vec capacity
 - improve performance (possibly storing store end pointer and data end pointer, instead of `len` and `cap` will help)
