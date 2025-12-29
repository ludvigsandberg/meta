#include <assert.h>

#include <x/set.h>
#include <x/string.h>

int main(int argc, char **argv) {
    {
        xset(xarr(char)) s;
        xset_new(s);
        assert(s.len == 0);
        assert(s.pop == 0);
        xset_free(s);
    }

    {
        xset(xarr(char)) s;
        xset_new(s);

        xarr(char) entry;
        xarr_from_string_literal(entry, "hello", 5);

        xset_set(s, xstring_hash, xstring_eq, entry);
        assert(s.len == 1);

        xset_free(s);
    }

    {
        xset(xarr(char)) s;
        xset_new(s);

        xarr(char) entry;
        xarr_from_string_literal(entry, "test", 4);
        xset_set(s, xstring_hash, xstring_eq, entry);

        bool contains;
        xset_contains(s, xstring_hash, xstring_eq, entry, contains);
        assert(contains);

        xset_free(s);
    }

    {
        xset(xarr(char)) s;
        xset_new(s);

        xarr(char) e1;
        xarr(char) e2;
        xarr(char) e3;

        xarr_from_string_literal(e1, "apple", 5);
        xarr_from_string_literal(e2, "banana", 6);
        xarr_from_string_literal(e3, "cherry", 6);

        xset_set(s, xstring_hash, xstring_eq, e1);
        xset_set(s, xstring_hash, xstring_eq, e2);
        xset_set(s, xstring_hash, xstring_eq, e3);

        assert(s.len == 3);

        bool c1, c2, c3;
        xset_contains(s, xstring_hash, xstring_eq, e1, c1);
        xset_contains(s, xstring_hash, xstring_eq, e2, c2);
        xset_contains(s, xstring_hash, xstring_eq, e3, c3);
        assert(c1 && c2 && c3);

        xset_free(s);
    }

    {
        xset(xarr(char)) s;
        xset_new(s);

        xarr(char) entry;
        xarr_from_string_literal(entry, "dup", 3);

        xset_set(s, xstring_hash, xstring_eq, entry);
        xset_set(s, xstring_hash, xstring_eq, entry);

        assert(s.len == 1);

        xset_free(s);
    }

    {
        xset(xarr(char)) s;
        xset_new(s);

        xarr(char) entry;
        xarr_from_string_literal(entry, "remove_me", 9);

        xset_set(s, xstring_hash, xstring_eq, entry);
        assert(s.len == 1);

        xset_remove(s, xstring_hash, xstring_eq, entry);
        assert(s.len == 0);

        bool contains;
        xset_contains(s, xstring_hash, xstring_eq, entry, contains);
        assert(!contains);

        xset_free(s);
    }

    return 0;
}