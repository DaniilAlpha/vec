#include <miunte.h>
#include <sys/sysinfo.h>
#include <unistd.h>

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

MiunteResult vec_setup_test() {
    miunte_expect(
        vec_init(&vec, VecOfFloat) == Ok,
        "initialization shoud not fail"
    );

    miunte_pass();
}
MiunteResult vec_teardown_test() {
    vec_uninit(&vec);

    miunte_pass();
}

MiunteResult vec_test_init() {
    miunte_expect(vec_len(&vec) == 0, "new vector is empty");
    miunte_expect(
        vec_cap(&vec) == VEC_MIN_CAP,
        "new vector's capacity is VEC_MIN_CAP"
    );
    miunte_expect(vec_buf(&vec) != NULL, "new vec's buf isn't NULL");
    miunte_expect(
        vec._el_size == sizeof(float),
        "new vec's el_size as miunte_expected"
    );

    miunte_pass();
}

MiunteResult vec_test_access_push_insert_clear() {
    miunte_expect(vec_first(&vec) == NULL, "empty vec has no first and last");
    miunte_expect(vec_last(&vec) == NULL, "empty vec has no first and last");

    miunte_expect(
        vec_push(&vec, 24) == Ok && vec_push(&vec, 25) == Ok,
        "push should not fail"
    );
    miunte_expect(vec_len(&vec) == 2, "after element pushed, len should be 1");
    miunte_expect(
        *vec_first(&vec) == 24 && *vec_last(&vec) == 25,
        "elements should be as pushed"
    );

    int a = 13;
    miunte_expect(vec_insert(&vec, 0, a--) == Ok, "insertion should not fail");
    miunte_expect(
        vec_len(&vec) == 3,
        "after third element inserted, len should be three"
    );
    miunte_expect(
        *vec_first(&vec) == 13.f && a == 12,
        "inserted element should be copied"
    );
    miunte_expect(
        *vec_at(&vec, 1) == 24 && *vec_at(&vec, 2) == 25,
        "after insert old elements are shifted"
    );

    miunte_expect(
        vec_first(&vec) == vec_at(&vec, 0),
        "two-el vec should have first at 0"
    );
    miunte_expect(
        vec_last(&vec) == vec_at(&vec, 2),
        "three-el vec should have last at 2"
    );

    miunte_expect(vec_clear(&vec) == Ok, "clear should not fail");
    miunte_expect(
        vec_len(&vec) == 0 && vec_cap(&vec) == VEC_MIN_CAP,
        "cleared vector should be empty"
    );

    long const ram_start = sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE);
    for (size_t i = 0; i < 10000000; i++) vec_clear(&vec);
    long const ram_end = sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE);
    miunte_expect(
        ram_start - ram_end < 10000000,
        "clear should not give any memory leaks"
    );

    miunte_pass();
}

MiunteResult vec_test_pop_remove() {
    miunte_expect(
        vec_push(&vec, 0) == Ok && vec_push(&vec, 1) == Ok &&
            vec_push(&vec, 2) == Ok && vec_push(&vec, 3) == Ok,
        "push should not fail"
    );

    miunte_expect(
        *vec_at(&vec, 3) == 3,
        "before pop there should be element at 3"
    );
    miunte_expect(vec_pop(&vec) == Ok, "pop should not fail");
    miunte_expect(
        vec_at(&vec, 3) == NULL,
        "after pop there should not be element at 3"
    );

    miunte_expect(
        *vec_at(&vec, 0) == 0,
        "before remove there should be element at 0"
    );
    miunte_expect(vec_remove(&vec, 0) == Ok, "remove should not fail");
    miunte_expect(
        *vec_at(&vec, 0) == 1,
        "after remove old elements are shifted"
    );
    miunte_expect(
        *vec_at(&vec, 1) == 2,
        "after remove old elements are shifted"
    );

    miunte_pass();
}

MiunteResult vec_test_iteration() {
    for (size_t i = 0; i < 10; i++)
        miunte_expect(vec_push(&vec, i) == Ok, "push should not fail");

    miunte_expect(vec_len(&vec) == 10, "vec should have elements before clear");
    miunte_expect(vec_clear(&vec) == Ok, "clear should not fail");
    miunte_expect(
        vec_len(&vec) == 0,
        "vec should not have elements after clear"
    );
    miunte_expect(
        vec_cap(&vec) == VEC_MIN_CAP,
        "vec's capacity should be VEC_MIN_CAP after clear"
    );
    miunte_expect(
        vec._el_size == sizeof(float),
        "vec's el_size should be as miunte_expected after clear"
    );

    miunte_pass();
}

MiunteResult vec_test_capacity() {
    miunte_expect(
        vec_cap(&vec) == VEC_MIN_CAP && vec_len(&vec) == 0,
        "empty vec's capacity should be VEC_MIN_CAP"
    );

    for (int i = 0; i < 100; i++)
        miunte_expect(vec_push(&vec, i) == Ok, "push should not fail");

    miunte_expect(
        vec_cap(&vec) == 128 && vec_len(&vec) == 100,
        "after insertion of 100 elements, capacity should be 128"
    );

    for (int i = 0; i < 50; i++)
        miunte_expect(vec_pop(&vec) == Ok, "pop should not fail");

    miunte_expect(
        vec_cap(&vec) == 64 && vec_len(&vec) == 50,
        "after removal of 50 elements, capacity should be 64"
    );

    for (int i = 0; i < 49; i++)
        miunte_expect(vec_pop(&vec) == Ok, "pop should not fail");

    miunte_expect(vec_pop(&vec) == Ok, "last pop should not fail");

    miunte_expect(
        vec_cap(&vec) == VEC_MIN_CAP && vec_len(&vec) == 0,
        "after removal of all elements, capacity should be VEC_MIN_CAP"
    );

    miunte_pass();
}

MiunteResult vec_test_init_from_array() {
    vec_uninit(&vec);  // just to prevent memory leak

    float const array[] = {-15, 18, 124, 1326, 0};
    miunte_expect(
        vec_init_from_arr(
            &vec,
            VecOfFloat,
            array,
            sizeof(array) / sizeof(*array)
        ) == Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < sizeof(array) / sizeof(*array); i++)
        miunte_expect(
            *vec_at(&vec, i) == array[i],
            "elements of vec init from an array corresponds to the array's elements"
        );

    miunte_pass();
}

MiunteResult vec_test_init_filled() {
    vec_uninit(&vec);  // just to prevent memory leak

    size_t const n = 100;
    float const value = 12345;
    miunte_expect(
        vec_init_filled(&vec, VecOfFloat, value, n) == Ok,
        "initialization should not fail"
    );
    for (size_t i = 0; i < n; i++)
        miunte_expect(
            *vec_at(&vec, i) == value,
            "element of filled initialized vec are equal to fillage"
        );

    miunte_pass();
}

int main() {
    miunte_test(
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
