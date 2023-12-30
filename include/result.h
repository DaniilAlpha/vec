#ifndef RESULT_H
#define RESULT_H

typedef enum Result {
  Ok = 0,
  OutOfMemErr,
  RangeErr,
} Result;

static const char *const result_msgs[] = {
  [Ok] = "ok!",
  [OutOfMemErr] = "out of memory",
  [RangeErr] = "out of range",
};

/* #define result_match(result, ...) \
   { \
     const Result error = (result); \
     switch (error) \
   } \
  ((void)0) */
#define unroll(result)                                                         \
  {                                                                            \
    const Result error = (result);                                             \
    switch (error) {                                                           \
    case Ok: break;                                                            \
    default: return error;                                                     \
    }                                                                          \
  }                                                                            \
  ((void)0)

#endif