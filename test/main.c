#include <stdbool.h>

#include <miunte.h>

// unused
#define Self   UnusedVec
#define T      char
#define PREFIX unused_vec
#include <vec.h>

// my vec
#define Self   MyVec
#define T      float
#define PREFIX my_vec
#include <vec.h>

MyVec vec;

MiunteResult vec_setup_test(void) {
    MIUNTE_EXPECT(my_vec_init(&vec) == Ok, "initialization shoud not fail");

    MIUNTE_PASS();
}
MiunteResult vec_teardown_test(void) {
    my_vec_uninit(&vec);

    MIUNTE_PASS();
}

MiunteResult vec_test_init(void) {
    MIUNTE_EXPECT(my_vec_get_len(&vec) == 0, "new vector is empty");
    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == VEC_MIN_CAP,
        "new vector's capacity is VEC_MIN_CAP"
    );
    MIUNTE_EXPECT(my_vec_get_buf(&vec) != NULL, "new vec's buf isn't NULL");
    MIUNTE_EXPECT(
        vec._el_size == sizeof(float),
        "new vec's el_size as MIUNTE_EXPECTed"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_access_push_insert_clear(void) {
    MIUNTE_EXPECT(
        my_vec_first(&vec) == NULL,
        "empty vec has no first and last"
    );
    MIUNTE_EXPECT(my_vec_last(&vec) == NULL, "empty vec has no first and last");

    MIUNTE_EXPECT(
        my_vec_push(&vec, 24) == Ok && my_vec_push(&vec, 25) == Ok,
        "push should not fail"
    );
    MIUNTE_EXPECT(
        my_vec_get_len(&vec) == 2,
        "after element pushed, len should be 1"
    );
    MIUNTE_EXPECT(
        *my_vec_first(&vec) == 24 && *my_vec_last(&vec) == 25,
        "elements should be as pushed"
    );

    int a = 13;
    MIUNTE_EXPECT(
        my_vec_insert(&vec, 0, a--) == Ok,
        "insertion should not fail"
    );
    MIUNTE_EXPECT(
        my_vec_get_len(&vec) == 3,
        "after third element inserted, len should be three"
    );
    MIUNTE_EXPECT(
        *my_vec_first(&vec) == 13.f && a == 12,
        "inserted element should be copied"
    );
    MIUNTE_EXPECT(
        *my_vec_at(&vec, 1) == 24 && *my_vec_at(&vec, 2) == 25,
        "after insert old elements are shifted"
    );

    MIUNTE_EXPECT(
        my_vec_first(&vec) == my_vec_at(&vec, 0),
        "two-el vec should have first at 0"
    );
    MIUNTE_EXPECT(
        my_vec_last(&vec) == my_vec_at(&vec, 2),
        "three-el vec should have last at 2"
    );

    MIUNTE_EXPECT(my_vec_clear(&vec) == Ok, "clear should not fail");
    MIUNTE_EXPECT(
        my_vec_get_len(&vec) == 0 && my_vec_get_cap(&vec) == VEC_MIN_CAP,
        "cleared vector should be empty"
    );

    for (size_t i = 0; i < 1000000; i++) my_vec_clear(&vec);

    // this will be caught by a sanitizer
    MIUNTE_EXPECT(true, "clear should not give any memory leaks");

    MIUNTE_PASS();
}

MiunteResult vec_test_pop_remove(void) {
    MIUNTE_EXPECT(
        my_vec_push(&vec, 0) == Ok && my_vec_push(&vec, 1) == Ok &&
            my_vec_push(&vec, 2) == Ok && my_vec_push(&vec, 3) == Ok,
        "push should not fail"
    );

    MIUNTE_EXPECT(
        *my_vec_at(&vec, 3) == 3,
        "before pop there should be element at 3"
    );
    MIUNTE_EXPECT(my_vec_pop(&vec) == Ok, "pop should not fail");
    MIUNTE_EXPECT(
        my_vec_at(&vec, 3) == NULL,
        "after pop there should not be element at 3"
    );

    MIUNTE_EXPECT(
        *my_vec_at(&vec, 0) == 0,
        "before remove there should be element at 0"
    );
    MIUNTE_EXPECT(my_vec_remove(&vec, 0) == Ok, "remove should not fail");
    MIUNTE_EXPECT(
        *my_vec_at(&vec, 0) == 1,
        "after remove old elements are shifted"
    );
    MIUNTE_EXPECT(
        *my_vec_at(&vec, 1) == 2,
        "after remove old elements are shifted"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_iteration(void) {
    for (size_t i = 0; i < 10; i++)
        MIUNTE_EXPECT(my_vec_push(&vec, i) == Ok, "push should not fail");

    MIUNTE_EXPECT(
        my_vec_get_len(&vec) == 10,
        "vec should have elements before clear"
    );
    MIUNTE_EXPECT(my_vec_clear(&vec) == Ok, "clear should not fail");
    MIUNTE_EXPECT(
        my_vec_get_len(&vec) == 0,
        "vec should not have elements after clear"
    );
    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == VEC_MIN_CAP,
        "vec's capacity should be VEC_MIN_CAP after clear"
    );
    MIUNTE_EXPECT(
        vec._el_size == sizeof(float),
        "vec's el_size should be as MIUNTE_EXPECTed after clear"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_capacity(void) {
    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == VEC_MIN_CAP && my_vec_get_len(&vec) == 0,
        "empty vec's capacity should be VEC_MIN_CAP"
    );

    for (int i = 0; i < 100; i++)
        MIUNTE_EXPECT(my_vec_push(&vec, i) == Ok, "push should not fail");

    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == 128 && my_vec_get_len(&vec) == 100,
        "after insertion of 100 elements, capacity should be 128"
    );

    for (int i = 0; i < 50; i++)
        MIUNTE_EXPECT(my_vec_pop(&vec) == Ok, "pop should not fail");

    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == 64 && my_vec_get_len(&vec) == 50,
        "after removal of 50 elements, capacity should be 64"
    );

    for (int i = 0; i < 49; i++)
        MIUNTE_EXPECT(my_vec_pop(&vec) == Ok, "pop should not fail");

    MIUNTE_EXPECT(my_vec_pop(&vec) == Ok, "last pop should not fail");

    MIUNTE_EXPECT(
        my_vec_get_cap(&vec) == VEC_MIN_CAP && my_vec_get_len(&vec) == 0,
        "after removal of all elements, capacity should be VEC_MIN_CAP"
    );

    MIUNTE_PASS();
}

MiunteResult vec_test_init_from_array(void) {
    my_vec_uninit(&vec);  // just to prevent memory leak

    float const array[] = {-15, 18, 124, 1326, 0};
    MIUNTE_EXPECT(
        my_vec_init_from_arr(&vec, array, sizeof(array) / sizeof(array[0])) ==
            Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); i++)
        MIUNTE_EXPECT(
            *my_vec_at(&vec, i) == array[i],
            "elements of vec init from an array corresponds to the array's elements"
        );

    MIUNTE_PASS();
}

MiunteResult vec_test_init_filled(void) {
    my_vec_uninit(&vec);  // just to prevent memory leak

    size_t const n = 100;
    float const value = 12345;
    MIUNTE_EXPECT(
        my_vec_init_filled(&vec, value, n) == Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < n; i++)
        MIUNTE_EXPECT(
            *my_vec_at(&vec, i) == value,
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
