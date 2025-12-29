#ifndef XMETA_H
#define XMETA_H

// concatenate
#define xconcat(A, B)  _xconcat(A, B)
#define _xconcat(A, B) A##B

// generate unique name for internal macro identifier
#define xuniq(NAME) xconcat(NAME, __LINE__)

// generate unique name for internal macro identifier in nested macros
#define xuniq2(NAME, MACRO_NAME) xconcat(NAME, xconcat(MACRO_NAME, __LINE__))

// static assert for compile time check
#define xstatic_assert(COND, MSG)                                             \
    typedef char xuniq(static_assert)[(COND) ? 1 : -1]

// assign pointer to pointer without type cast
#define xgeneric_assign(A, B)                                                 \
    do {                                                                      \
        void *xuniq(m) = B;                                                   \
        memcpy(&A, &xuniq(m), sizeof(void *));                                \
    } while (0)

#endif
