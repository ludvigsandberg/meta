/* -------------------------------------------------------------------------
   meta.h - C99 container library by Ludvig Sandberg, 2025.
   ------------------------------------------------------------------------- */

#ifndef META_H
#define META_H

#define concat2(A, B) A##B
#define concat(A, B)  concat2(A, B)

/** @brief Generate a unique name for internal macro identifiers. */
#define uniq(NAME) concat(NAME, __LINE__)

/** @brief Static assertion for compile-time checks. */
#ifdef __GNUC__
#define static_assert(COND, MSG)                                              \
    __attribute__((unused)) typedef char uniq(static_assert)[(COND) ? 1 : -1]
#else
#define static_assert(COND, MSG)
#endif

/** @brief Assign pointer to pointer without type cast. */
#define generic_assign(A, B)                                                  \
    do {                                                                      \
        void *uniq(m) = B;                                                    \
        memcpy(&A, &uniq(m), sizeof(void *));                                 \
    } while (0)

/** @brief Iterate over a range of values. */
#define foreach(N, I) for (size_t I = 0; I < (N); I++)

/* -------------------------------------------------------------------------
   Generic dynamic array.
   Metadata about array length and capacity is stored at
   indices -2 and -1 and array elements begin like normal at index 0.
   ------------------------------------------------------------------------- */

#define arr(T) T *

/** @brief Get the length of an array. */
#define alen(A) ((size_t *)(A))[-2]
/** @brief Get the capacity of an array. */
#define acap(A) ((size_t *)(A))[-1]

/** @brief Iterate over the elements of an array. */
#define arr_foreach(A, I) for (size_t I = 0; I < alen(A); I++)

/** @brief Create a new empty array. */
#define arr_new(A)                                                            \
    generic_assign((A), (size_t *)(calloc(2, sizeof(size_t))) + 2);

/** @brief Create a new array with a specific length. */
#define arr_new_n(A, N)                                                       \
    do {                                                                      \
        generic_assign(                                                       \
            (A),                                                              \
            (size_t *)malloc(2 * sizeof(size_t) + (N) * sizeof((A)[0])) + 2); \
        alen(A) = acap(A) = (N);                                              \
    } while (0);

/** @brief Create a new array with all elements initialized to zero. */
#define arr_new_n_zero(A, N)                                                  \
    do {                                                                      \
        generic_assign((A), (size_t *)calloc(1, 2 * sizeof(size_t) +          \
                                                    (N) * sizeof((A)[0])) +   \
                                2);                                           \
        alen(A) = acap(A) = (N);                                              \
    } while (0);

/** @brief Create a new array with a specific length. */
#define arr_new_reserve(A, N)                                                 \
    do {                                                                      \
        generic_assign(                                                       \
            (A),                                                              \
            (size_t *)malloc(2 * sizeof(size_t) + (N) * sizeof((A)[0])) + 2); \
        alen(A) = 0;                                                          \
        acap(A) = (N);                                                        \
    } while (0);

/** @brief Create a new array with a specific length with all elements
 * initialized to zero. */
#define arr_new_reserve_zero(A, N)                                            \
    do {                                                                      \
        generic_assign((A), (size_t *)calloc(1, 2 * sizeof(size_t) +          \
                                                    (N) * sizeof((A)[0])) +   \
                                2);                                           \
        alen(A) = 0;                                                          \
        acap(A) = (N);                                                        \
    } while (0);

#if !defined(__SIZEOF_SIZE_T__) || __SIZEOF_SIZE_T__ == 8
#define ZEROES "\x00\x00\x00\x00\x00\x00"
#elif __SIZEOF_SIZE_T__ == 16
#define ZEROES "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#elif __SIZEOF_SIZE_T__ == 4
#define ZEROES "\x00\x00"
#else
#define ZEROES
#endif

#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define literal(LO, HI) concat(HEX_, LO) concat(HEX_, HI) ZEROES
#else
#define literal(LO, HI) concat(HEX_, HI) concat(HEX_, LO) ZEROES
#endif

/** @brief Embed a string literal and its metadata in an array without using
 * heap memory. */
#define arr_from_string_literal(A, L, LO, HI)                                 \
    do {                                                                      \
        static_assert(sizeof(L) - 1 == (((HI) << 8) | (LO)),                  \
                      "Invalid length.");                                     \
        generic_assign((A), (literal(LO, HI) literal(LO, HI) L) +             \
                                2 * sizeof(size_t));                          \
    } while (0)

/** @brief Free the memory allocated for an array. */
#define arr_free(A) free(((size_t *)A) - 2);

/** @brief Resize an array to a new length. */
#define arr_resize(A, N)                                                      \
    do {                                                                      \
        generic_assign((A), (size_t *)(realloc((size_t *)(A) - 2,             \
                                               2 * sizeof(size_t) +           \
                                                   (N) * sizeof((A)[0]))) +   \
                                2);                                           \
        alen(A) = acap(A) = (N);                                              \
    } while (0)

/** @brief Insert N uninitialized elements into an array at a specific index.
 */
#define arr_insert_raw_n(A, I, N)                                             \
    do {                                                                      \
        assert((I) <= alen(A));                                               \
                                                                              \
        if (alen(A) + (N) > acap(A)) {                                        \
            acap(A) = (alen(A) + (N)) * 2;                                    \
                                                                              \
            generic_assign(                                                   \
                (A), (size_t *)(realloc((size_t *)(A) - 2,                    \
                                        2 * sizeof(size_t) +                  \
                                            acap(A) * sizeof((A)[0]))) +      \
                         2);                                                  \
        }                                                                     \
                                                                              \
        if ((I) != alen(A)) {                                                 \
            memmove((A) + (I) + (N), (A) + (I),                               \
                    (alen(A) - (I)) * sizeof((A)[0]));                        \
        }                                                                     \
                                                                              \
        alen(A) += (N);                                                       \
    } while (0)

/** @brief Insert a single uninitialized element into an array at a specific
 * index.
 */
#define arr_insert_raw(A, I) arr_insert_raw_n(A, I, 1)

/** @brief Append N uninitialized elements to the end of an array. */
#define arr_append_n_raw(A, N)                                                \
    do {                                                                      \
        /* Capture I before alen(A) mutates. */                               \
        size_t uniq(i) = alen(A);                                             \
        arr_insert_raw_n(A, uniq(i), N);                                      \
    } while (0)

/** @brief Append a single uninitialized element to the end of an array. */
#define arr_append_raw(A) arr_append_n_raw(A, 1)

/** @brief Insert N elements into an array at a specific index. */
#define arr_insert_n(A, I, N, B)                                              \
    do {                                                                      \
        arr_insert_raw_n(A, I, N);                                            \
                                                                              \
        for (size_t uniq(j) = 0; uniq(j) < (N); ++uniq(j)) {                  \
            (A)[(I) + uniq(j)] = (B)[uniq(j)];                                \
        }                                                                     \
    } while (0)

/** @brief Insert a single element into an array at a specific index. */
#define arr_insert(A, I, E) arr_insert_n(A, I, 1, &(E))

/** @brief Append N elements to the end of an array. */
#define arr_append_n(A, N, B)                                                 \
    do {                                                                      \
        /* Capture I before alen(A) mutates. */                               \
        size_t uniq(i) = alen(A);                                             \
        arr_insert_n(A, uniq(i), N, B);                                       \
    } while (0)

/** @brief Append a single element to the end of an array. */
#define arr_append(A, E) arr_append_n(A, 1, &(E))

/** @brief Remove N elements from an array at a specific index. */
#define arr_remove_n(A, I, N)                                                 \
    do {                                                                      \
        assert((I) < alen(A));                                                \
        assert((I) + (N) <= alen(A));                                         \
                                                                              \
        if ((I) + (N) < alen(A)) {                                            \
            memmove((A) + (I), (A) + (I) + (N),                               \
                    (alen(A) - (I) - (N)) * sizeof((A)[0]));                  \
        }                                                                     \
                                                                              \
        alen(A) -= (N);                                                       \
    } while (0)

/** @brief Remove a single element from an array at a specific index. */
#define arr_remove(A, I) arr_remove_n(A, I, 1)

/** @brief Remove an element from an array at a specific pointer. */
#define arr_remove_ptr(A, P) arr_remove(A, (size_t)((P) - (A)))

/* -------------------------------------------------------------------------
   Generic FIFO queue (ring buffer).
   ------------------------------------------------------------------------- */

#define queue(T)                                                              \
    struct {                                                                  \
        arr(T) buf;                                                           \
        size_t head; /* Head index. */                                        \
        size_t tail; /* Tail index. */                                        \
    }

#define queue_empty(Q) ((Q).head == (Q).tail)

#define queue_len(Q) (((Q).tail + alen((Q).buf) - (Q).head) % alen((Q).buf))

#define queue_new(Q, CAP)                                                     \
    do {                                                                      \
        arr_new_n((Q).buf, CAP);                                              \
        (Q).head = 0;                                                         \
        (Q).tail = 0;                                                         \
    } while (0)

#define queue_free(Q) arr_free((Q).buf)

#define queue_push(Q, ELEM)                                                   \
    do {                                                                      \
        if (((Q).tail + 1) % alen((Q).buf) == (Q).head) {                     \
            arr_append((Q).buf, ELEM);                                        \
        }                                                                     \
        (Q).buf[(Q).tail] = ELEM;                                             \
        (Q).tail          = ((Q).tail + 1) % alen((Q).buf);                   \
    } while (0)

/** NOTE: Always check if queue is empty before pop. */
#define queue_pop(Q)                                                          \
    (Q).buf[((Q).head = ((Q).head + 1) % alen((Q).buf)) == 0                  \
                ? alen((Q).buf) - 1                                           \
                : (Q).head - 1]

/* -------------------------------------------------------------------------
   Generic hash map.
   ------------------------------------------------------------------------- */

#define map(K, V)                                                             \
    struct {                                                                  \
        arr(arr(struct {                                                      \
            K key;                                                            \
            V val;                                                            \
        })) bkts;                                                             \
                                                                              \
        size_t len;      /* Number of entries. */                             \
        size_t pop_bkts; /* Number of populated buckets. */                   \
    }

/** @brief Iterate over all key-value pairs in a hash map. */
#define map_foreach(M, K, V)                                                  \
    arr_foreach((M).bkts, uniq(i)) arr_foreach(                               \
        (M).bkts[uniq(i)],                                                    \
        uniq(j)) for (bool uniq(once) = true;                                 \
                      uniq(                                                   \
                          once);) for ((K) =                                  \
                                           &((M).bkts[uniq(i)][uniq(j)].key), \
                                       (V) =                                  \
                                           &((M).bkts[uniq(i)][uniq(j)].val); \
                                       uniq(once); uniq(once) = false)

/** @brief Create a new hash map with a specific initial capacity. */
#define map_new_reserve(M, N)                                                 \
    do {                                                                      \
        arr_new_n((M).bkts, N);                                               \
        (M).len = (M).pop_bkts = 0;                                           \
                                                                              \
        arr_foreach((M).bkts, uniq(i)) {                                      \
            arr_new_reserve((M).bkts[uniq(i)], 2);                            \
        }                                                                     \
    } while (0)

/** @brief Create a new hash map with a default initial capacity. */
#define map_new(M) map_new_reserve(M, 16)

/** @brief Free the memory allocated for a hash map. */
#define map_free(M)                                                           \
    do {                                                                      \
        arr_foreach((M).bkts, uniq(i)) {                                      \
            arr_free((M).bkts[uniq(i)]);                                      \
        }                                                                     \
        arr_free((M).bkts);                                                   \
    } while (0)

/** @brief Get a value from a hash map, returning NULL if not found. */
#define map_get_maybe_null(M, HASH, EQ, K, V)                                 \
    do {                                                                      \
        (V)               = NULL;                                             \
        size_t uniq(hash) = HASH(&(K)) % acap((M).bkts);                      \
                                                                              \
        arr_foreach((M).bkts[uniq(hash)], uniq(i)) {                          \
            if (EQ(&(M).bkts[uniq(hash)][uniq(i)].key, &(K))) {               \
                (V) = &(M).bkts[uniq(hash)][uniq(i)].val;                     \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

/** @brief Get a value from a hash map, it must exist. */
#define map_get(M, HASH, EQ, K, V)                                            \
    do {                                                                      \
        map_get_maybe_null(M, HASH, EQ, K, V);                                \
        assert(V);                                                            \
    } while (0)

/** @brief Check if a key exists in a hash map. */
#define map_contains(M, HASH, EQ, K, BOOL)                                    \
    do {                                                                      \
        void *uniq(v);                                                        \
        map_get_maybe_null(M, HASH, EQ, K, uniq(v));                          \
        (BOOL) = uniq(v) != NULL;                                             \
    } while (0)

/** @brief Internal macro for setting a value in a hash map without rehashing.
 */
#define map_set_no_rehash(M, CAP, HASH, EQ, K, V)                             \
    do {                                                                      \
        size_t uniq(hash) = HASH(&(K)) % (CAP);                               \
        /* If two keys within a bucket match the value will be replaced.      \
           Otherwise append new entry to bucket. */                           \
        bool uniq(replaced) = false;                                          \
                                                                              \
        foreach (alen((M).bkts[uniq(hash)]), uniq(k)) {                       \
            if (EQ(&(M).bkts[uniq(hash)][uniq(k)].key, &(K))) {               \
                (M).bkts[uniq(hash)][uniq(k)].val = V;                        \
                uniq(replaced)                    = true;                     \
                break;                                                        \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (!uniq(replaced)) {                                                \
            (M).len++;                                                        \
                                                                              \
            if (alen((M).bkts[uniq(hash)]) == 0) {                            \
                (M).pop_bkts++;                                               \
            }                                                                 \
                                                                              \
            arr_append_raw((M).bkts[uniq(hash)]);                             \
            ((M).bkts[uniq(hash)])[alen((M).bkts[uniq(hash)]) - 1].key = K;   \
            ((M).bkts[uniq(hash)])[alen((M).bkts[uniq(hash)]) - 1].val = V;   \
        }                                                                     \
    } while (0)

/** @brief Set a key-value pair in a hash map. Rehash if needed. */
#define map_set(M, HASH, EQ, K, V)                                            \
    do {                                                                      \
        /* Grow array if needed. */                                           \
        if ((M).pop_bkts == acap((M).bkts) / 2) {                             \
            (M).len = (M).pop_bkts = 0;                                       \
                                                                              \
            size_t uniq(old_cap) = acap((M).bkts);                            \
            size_t uniq(new_cap) = acap((M).bkts) * 2;                        \
                                                                              \
            /* Double number of buckets. Also allocate a 3rd region to        \
               temporarily store the old data while re-hashing. */            \
            arr_resize((M).bkts, uniq(new_cap) + uniq(old_cap));              \
                                                                              \
            /* Move old data to the 3rd region. */                            \
            memcpy((M).bkts + uniq(new_cap), (M).bkts,                        \
                   (uniq(old_cap)) * sizeof((M).bkts[0]));                    \
            memset((M).bkts, 0x0, uniq(old_cap) * sizeof((M).bkts[0]));       \
                                                                              \
            /* Initialize new buckets. */                                     \
            foreach (uniq(new_cap), uniq(bkt)) {                              \
                arr_new_reserve((M).bkts[uniq(bkt)], 2);                      \
            }                                                                 \
                                                                              \
            /* Re-hash. */                                                    \
            foreach (uniq(old_cap), uniq(bkt)) {                              \
                arr_foreach(((M).bkts + uniq(new_cap))[uniq(bkt)], uniq(j)) { \
                    map_set_no_rehash(                                        \
                        M, uniq(new_cap), HASH, EQ,                           \
                        ((M).bkts + uniq(new_cap))[uniq(bkt)][uniq(j)].key,   \
                        ((M).bkts + uniq(new_cap))[uniq(bkt)][uniq(j)].val);  \
                }                                                             \
                                                                              \
                /* Free old bucket after re-hash. */                          \
                arr_free(((M).bkts + uniq(new_cap))[uniq(bkt)]);              \
            }                                                                 \
                                                                              \
            /* Shrink array to new capacity. */                               \
            acap((M).bkts) = uniq(new_cap);                                   \
        }                                                                     \
                                                                              \
        /* Insert new value. */                                               \
        map_set_no_rehash(M, acap((M).bkts), HASH, EQ, K, V);                 \
    } while (0)

/** @brief Remove a key-value pair from a hash map. */
#define map_remove(M, HASH, EQ, K)                                            \
    do {                                                                      \
        size_t uniq(hash) = HASH(&(K)) % acap((M).bkts);                      \
                                                                              \
        arr_foreach((M).bkts[uniq(hash)], uniq(j)) {                          \
            if (EQ(&(M).bkts[uniq(hash)][uniq(j)].key, &(K))) {               \
                arr_remove((M).bkts[uniq(hash)], uniq(j));                    \
                                                                              \
                (M).len--;                                                    \
                                                                              \
                if (alen((M).bkts[uniq(hash)]) == 0) {                        \
                    (M).pop_bkts--;                                           \
                }                                                             \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

#define HEX_0   "\x00"
#define HEX_1   "\x01"
#define HEX_2   "\x02"
#define HEX_3   "\x03"
#define HEX_4   "\x04"
#define HEX_5   "\x05"
#define HEX_6   "\x06"
#define HEX_7   "\x07"
#define HEX_8   "\x08"
#define HEX_9   "\x09"
#define HEX_10  "\x0a"
#define HEX_11  "\x0b"
#define HEX_12  "\x0c"
#define HEX_13  "\x0d"
#define HEX_14  "\x0e"
#define HEX_15  "\x0f"
#define HEX_16  "\x10"
#define HEX_17  "\x11"
#define HEX_18  "\x12"
#define HEX_19  "\x13"
#define HEX_20  "\x14"
#define HEX_21  "\x15"
#define HEX_22  "\x16"
#define HEX_23  "\x17"
#define HEX_24  "\x18"
#define HEX_25  "\x19"
#define HEX_26  "\x1a"
#define HEX_27  "\x1b"
#define HEX_28  "\x1c"
#define HEX_29  "\x1d"
#define HEX_30  "\x1e"
#define HEX_31  "\x1f"
#define HEX_32  "\x20"
#define HEX_33  "\x21"
#define HEX_34  "\x22"
#define HEX_35  "\x23"
#define HEX_36  "\x24"
#define HEX_37  "\x25"
#define HEX_38  "\x26"
#define HEX_39  "\x27"
#define HEX_40  "\x28"
#define HEX_41  "\x29"
#define HEX_42  "\x2a"
#define HEX_43  "\x2b"
#define HEX_44  "\x2c"
#define HEX_45  "\x2d"
#define HEX_46  "\x2e"
#define HEX_47  "\x2f"
#define HEX_48  "\x30"
#define HEX_49  "\x31"
#define HEX_50  "\x32"
#define HEX_51  "\x33"
#define HEX_52  "\x34"
#define HEX_53  "\x35"
#define HEX_54  "\x36"
#define HEX_55  "\x37"
#define HEX_56  "\x38"
#define HEX_57  "\x39"
#define HEX_58  "\x3a"
#define HEX_59  "\x3b"
#define HEX_60  "\x3c"
#define HEX_61  "\x3d"
#define HEX_62  "\x3e"
#define HEX_63  "\x3f"
#define HEX_64  "\x40"
#define HEX_65  "\x41"
#define HEX_66  "\x42"
#define HEX_67  "\x43"
#define HEX_68  "\x44"
#define HEX_69  "\x45"
#define HEX_70  "\x46"
#define HEX_71  "\x47"
#define HEX_72  "\x48"
#define HEX_73  "\x49"
#define HEX_74  "\x4a"
#define HEX_75  "\x4b"
#define HEX_76  "\x4c"
#define HEX_77  "\x4d"
#define HEX_78  "\x4e"
#define HEX_79  "\x4f"
#define HEX_80  "\x50"
#define HEX_81  "\x51"
#define HEX_82  "\x52"
#define HEX_83  "\x53"
#define HEX_84  "\x54"
#define HEX_85  "\x55"
#define HEX_86  "\x56"
#define HEX_87  "\x57"
#define HEX_88  "\x58"
#define HEX_89  "\x59"
#define HEX_90  "\x5a"
#define HEX_91  "\x5b"
#define HEX_92  "\x5c"
#define HEX_93  "\x5d"
#define HEX_94  "\x5e"
#define HEX_95  "\x5f"
#define HEX_96  "\x60"
#define HEX_97  "\x61"
#define HEX_98  "\x62"
#define HEX_99  "\x63"
#define HEX_100 "\x64"
#define HEX_101 "\x65"
#define HEX_102 "\x66"
#define HEX_103 "\x67"
#define HEX_104 "\x68"
#define HEX_105 "\x69"
#define HEX_106 "\x6a"
#define HEX_107 "\x6b"
#define HEX_108 "\x6c"
#define HEX_109 "\x6d"
#define HEX_110 "\x6e"
#define HEX_111 "\x6f"
#define HEX_112 "\x70"
#define HEX_113 "\x71"
#define HEX_114 "\x72"
#define HEX_115 "\x73"
#define HEX_116 "\x74"
#define HEX_117 "\x75"
#define HEX_118 "\x76"
#define HEX_119 "\x77"
#define HEX_120 "\x78"
#define HEX_121 "\x79"
#define HEX_122 "\x7a"
#define HEX_123 "\x7b"
#define HEX_124 "\x7c"
#define HEX_125 "\x7d"
#define HEX_126 "\x7e"
#define HEX_127 "\x7f"
#define HEX_128 "\x80"
#define HEX_129 "\x81"
#define HEX_130 "\x82"
#define HEX_131 "\x83"
#define HEX_132 "\x84"
#define HEX_133 "\x85"
#define HEX_134 "\x86"
#define HEX_135 "\x87"
#define HEX_136 "\x88"
#define HEX_137 "\x89"
#define HEX_138 "\x8a"
#define HEX_139 "\x8b"
#define HEX_140 "\x8c"
#define HEX_141 "\x8d"
#define HEX_142 "\x8e"
#define HEX_143 "\x8f"
#define HEX_144 "\x90"
#define HEX_145 "\x91"
#define HEX_146 "\x92"
#define HEX_147 "\x93"
#define HEX_148 "\x94"
#define HEX_149 "\x95"
#define HEX_150 "\x96"
#define HEX_151 "\x97"
#define HEX_152 "\x98"
#define HEX_153 "\x99"
#define HEX_154 "\x9a"
#define HEX_155 "\x9b"
#define HEX_156 "\x9c"
#define HEX_157 "\x9d"
#define HEX_158 "\x9e"
#define HEX_159 "\x9f"
#define HEX_160 "\xa0"
#define HEX_161 "\xa1"
#define HEX_162 "\xa2"
#define HEX_163 "\xa3"
#define HEX_164 "\xa4"
#define HEX_165 "\xa5"
#define HEX_166 "\xa6"
#define HEX_167 "\xa7"
#define HEX_168 "\xa8"
#define HEX_169 "\xa9"
#define HEX_170 "\xaa"
#define HEX_171 "\xab"
#define HEX_172 "\xac"
#define HEX_173 "\xad"
#define HEX_174 "\xae"
#define HEX_175 "\xaf"
#define HEX_176 "\xb0"
#define HEX_177 "\xb1"
#define HEX_178 "\xb2"
#define HEX_179 "\xb3"
#define HEX_180 "\xb4"
#define HEX_181 "\xb5"
#define HEX_182 "\xb6"
#define HEX_183 "\xb7"
#define HEX_184 "\xb8"
#define HEX_185 "\xb9"
#define HEX_186 "\xba"
#define HEX_187 "\xbb"
#define HEX_188 "\xbc"
#define HEX_189 "\xbd"
#define HEX_190 "\xbe"
#define HEX_191 "\xbf"
#define HEX_192 "\xc0"
#define HEX_193 "\xc1"
#define HEX_194 "\xc2"
#define HEX_195 "\xc3"
#define HEX_196 "\xc4"
#define HEX_197 "\xc5"
#define HEX_198 "\xc6"
#define HEX_199 "\xc7"
#define HEX_200 "\xc8"
#define HEX_201 "\xc9"
#define HEX_202 "\xca"
#define HEX_203 "\xcb"
#define HEX_204 "\xcc"
#define HEX_205 "\xcd"
#define HEX_206 "\xce"
#define HEX_207 "\xcf"
#define HEX_208 "\xd0"
#define HEX_209 "\xd1"
#define HEX_210 "\xd2"
#define HEX_211 "\xd3"
#define HEX_212 "\xd4"
#define HEX_213 "\xd5"
#define HEX_214 "\xd6"
#define HEX_215 "\xd7"
#define HEX_216 "\xd8"
#define HEX_217 "\xd9"
#define HEX_218 "\xda"
#define HEX_219 "\xdb"
#define HEX_220 "\xdc"
#define HEX_221 "\xdd"
#define HEX_222 "\xde"
#define HEX_223 "\xdf"
#define HEX_224 "\xe0"
#define HEX_225 "\xe1"
#define HEX_226 "\xe2"
#define HEX_227 "\xe3"
#define HEX_228 "\xe4"
#define HEX_229 "\xe5"
#define HEX_230 "\xe6"
#define HEX_231 "\xe7"
#define HEX_232 "\xe8"
#define HEX_233 "\xe9"
#define HEX_234 "\xea"
#define HEX_235 "\xeb"
#define HEX_236 "\xec"
#define HEX_237 "\xed"
#define HEX_238 "\xee"
#define HEX_239 "\xef"
#define HEX_240 "\xf0"
#define HEX_241 "\xf1"
#define HEX_242 "\xf2"
#define HEX_243 "\xf3"
#define HEX_244 "\xf4"
#define HEX_245 "\xf5"
#define HEX_246 "\xf6"
#define HEX_247 "\xf7"
#define HEX_248 "\xf8"
#define HEX_249 "\xf9"
#define HEX_250 "\xfa"
#define HEX_251 "\xfb"
#define HEX_252 "\xfc"
#define HEX_253 "\xfd"
#define HEX_254 "\xfe"
#define HEX_255 "\xff"

#endif