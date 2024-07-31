#include <stdbool.h>

#include <miunte.h>

// for chars
#define Self VecOfChar
#define T    char
#include <vec.h>

#define Self VecOfChar
#define T    char
#include <vec_impl.h>

// for floats
#define Self VecOfFloat
#define T    float
#include <vec.h>

#define Self VecOfFloat
#define T    float
#include <vec_impl.h>

VecOfFloat vec;

MiunteResult vec_setup_test(void) {
    MIUNTE_EXPECT(
        vec_init(&vec, VecOfFloat) == Ok,
        "initialization shoud not fail"
    );

    MIUNTE_PASS();
}
MiunteResult vec_teardown_test(void) {
    vec_uninit(&vec);

    MIUNTE_PASS();
}

MiunteResult vec_test_init(void) {
    MIUNTE_EXPECT(vec_len(&vec) == 0, "new vector is empty");
    MIUNTE_EXPECT(
        vec_cap(&vec) == VEC_MIN_CAP,
        "new vector's capacity is VEC_MIN_CAP"
    );
    MIUNTE_EXPECT(vec_buf(&vec) != NULL, "new vec's buf isn't NULL");
    MIUNTE_EXPECT(
        vec.__vtbl->el_size == sizeof(float),
        "new vec's el_size as MIUNTE_EXPECTed"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_access_push_insert_clear(void) {
    MIUNTE_EXPECT(vec_first(&vec) == NULL, "empty vec has no first and last");
    MIUNTE_EXPECT(vec_last(&vec) == NULL, "empty vec has no first and last");

    MIUNTE_EXPECT(
        vec_push(&vec, 24) == Ok && vec_push(&vec, 25) == Ok,
        "push should not fail"
    );
    MIUNTE_EXPECT(vec_len(&vec) == 2, "after element pushed, len should be 1");
    MIUNTE_EXPECT(
        *vec_first(&vec) == 24 && *vec_last(&vec) == 25,
        "elements should be as pushed"
    );

    int a = 13;
    MIUNTE_EXPECT(vec_insert(&vec, 0, a--) == Ok, "insertion should not fail");
    MIUNTE_EXPECT(
        vec_len(&vec) == 3,
        "after third element inserted, len should be three"
    );
    MIUNTE_EXPECT(
        *vec_first(&vec) == 13.f && a == 12,
        "inserted element should be copied"
    );
    MIUNTE_EXPECT(
        *vec_at(&vec, 1) == 24 && *vec_at(&vec, 2) == 25,
        "after insert old elements are shifted"
    );

    MIUNTE_EXPECT(
        vec_first(&vec) == vec_at(&vec, 0),
        "two-el vec should have first at 0"
    );
    MIUNTE_EXPECT(
        vec_last(&vec) == vec_at(&vec, 2),
        "three-el vec should have last at 2"
    );

    MIUNTE_EXPECT(vec_clear(&vec) == Ok, "clear should not fail");
    MIUNTE_EXPECT(
        vec_len(&vec) == 0 && vec_cap(&vec) == VEC_MIN_CAP,
        "cleared vector should be empty"
    );

    for (size_t i = 0; i < 10000000; i++) vec_clear(&vec);

    // this will be caught by a sanitizer
    MIUNTE_EXPECT(true, "clear should not give any memory leaks");

    MIUNTE_PASS();
}

MiunteResult vec_test_pop_remove(void) {
    MIUNTE_EXPECT(
        vec_push(&vec, 0) == Ok && vec_push(&vec, 1) == Ok &&
            vec_push(&vec, 2) == Ok && vec_push(&vec, 3) == Ok,
        "push should not fail"
    );

    MIUNTE_EXPECT(
        *vec_at(&vec, 3) == 3,
        "before pop there should be element at 3"
    );
    MIUNTE_EXPECT(vec_pop(&vec) == Ok, "pop should not fail");
    MIUNTE_EXPECT(
        vec_at(&vec, 3) == NULL,
        "after pop there should not be element at 3"
    );

    MIUNTE_EXPECT(
        *vec_at(&vec, 0) == 0,
        "before remove there should be element at 0"
    );
    MIUNTE_EXPECT(vec_remove(&vec, 0) == Ok, "remove should not fail");
    MIUNTE_EXPECT(
        *vec_at(&vec, 0) == 1,
        "after remove old elements are shifted"
    );
    MIUNTE_EXPECT(
        *vec_at(&vec, 1) == 2,
        "after remove old elements are shifted"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_iteration(void) {
    for (size_t i = 0; i < 10; i++)
        MIUNTE_EXPECT(vec_push(&vec, i) == Ok, "push should not fail");

    MIUNTE_EXPECT(vec_len(&vec) == 10, "vec should have elements before clear");
    MIUNTE_EXPECT(vec_clear(&vec) == Ok, "clear should not fail");
    MIUNTE_EXPECT(
        vec_len(&vec) == 0,
        "vec should not have elements after clear"
    );
    MIUNTE_EXPECT(
        vec_cap(&vec) == VEC_MIN_CAP,
        "vec's capacity should be VEC_MIN_CAP after clear"
    );
    MIUNTE_EXPECT(
        vec.__vtbl->el_size == sizeof(float),
        "vec's el_size should be as MIUNTE_EXPECTed after clear"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_capacity(void) {
    MIUNTE_EXPECT(
        vec_cap(&vec) == VEC_MIN_CAP && vec_len(&vec) == 0,
        "empty vec's capacity should be VEC_MIN_CAP"
    );

    for (int i = 0; i < 100; i++)
        MIUNTE_EXPECT(vec_push(&vec, i) == Ok, "push should not fail");

    MIUNTE_EXPECT(
        vec_cap(&vec) == 128 && vec_len(&vec) == 100,
        "after insertion of 100 elements, capacity should be 128"
    );

    for (int i = 0; i < 50; i++)
        MIUNTE_EXPECT(vec_pop(&vec) == Ok, "pop should not fail");

    MIUNTE_EXPECT(
        vec_cap(&vec) == 64 && vec_len(&vec) == 50,
        "after removal of 50 elements, capacity should be 64"
    );

    for (int i = 0; i < 49; i++)
        MIUNTE_EXPECT(vec_pop(&vec) == Ok, "pop should not fail");

    MIUNTE_EXPECT(vec_pop(&vec) == Ok, "last pop should not fail");

    MIUNTE_EXPECT(
        vec_cap(&vec) == VEC_MIN_CAP && vec_len(&vec) == 0,
        "after removal of all elements, capacity should be VEC_MIN_CAP"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_init_from_array(void) {
    vec_uninit(&vec);  // just to prevent memory leak

    float const array[] = {-15, 18, 124, 1326, 0};
    MIUNTE_EXPECT(
        vec_init_from_arr(
            &vec,
            VecOfFloat,
            array,
            sizeof(array) / sizeof(array[0])
        ) == Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); i++)
        MIUNTE_EXPECT(
            *vec_at(&vec, i) == array[i],
            "elements of vec init from an array corresponds to the array's elements"
        );

    MIUNTE_PASS();
}

MiunteResult vec_test_init_filled(void) {
    vec_uninit(&vec);  // just to prevent memory leak

    size_t const n = 100;
    float const value = 12345;
    MIUNTE_EXPECT(
        vec_init_filled(&vec, VecOfFloat, value, n) == Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < n; i++)
        MIUNTE_EXPECT(
            *vec_at(&vec, i) == value,
            "element of filled initialized vec are equal to fillage"
        );

    MIUNTE_PASS();
}

int main(void) {
    MIUNTE_RUN(
        vec_setup_test,
        vec_teardown_test,
        {
            vec_test_init,
            vec_test_access_push_insert_clear,
            vec_test_pop_remove,
            vec_test_iteration,
            vec_test_capacity,
            vec_test_init_from_array,
            vec_test_init_filled,
        }
    );
}
