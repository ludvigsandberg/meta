#ifndef XSTRING_H
#define XSTRING_H

#include <stdint.h>
#include <stdbool.h>

#include <x/arr.h>

#if !defined(__SIZEOF_SIZE_T__) || __SIZEOF_SIZE_T__ == 8
#define _XZEROES "\x00\x00\x00\x00\x00\x00"
#elif __SIZEOF_SIZE_T__ == 16
#define _XZEROES "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#elif __SIZEOF_SIZE_T__ == 4
#define _XZEROES "\x00\x00"
#else
#error "unknown platform"
#endif

#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define _xliteral(LO, HI) xconcat(_XHEX_, LO) xconcat(_XHEX_, HI) _XZEROES
#else
#define _xliteral(LO, HI) xconcat(_XHEX_, HI) xconcat(_XHEX_, LO) _XZEROES
#endif

// embed string literal in array without allocating memory
// eg xarr_from_string_literal_ex(str, "hello", 5, 0)
#define xarr_from_string_literal2(A, L, LO, HI)                               \
    do {                                                                      \
        xstatic_assert(sizeof(L) - 1 == (((HI) << 8) | (LO)),                 \
                       "invalid length");                                     \
        xgeneric_assign((A), (_xliteral(LO, HI) _xliteral(LO, HI) L) +        \
                                 2 * sizeof(size_t));                         \
    } while (0)

// embed string literal in array without allocating memory
// eg xarr_from_string_literal(str, "hello", 5)
// note, this macro only supports length up to 255
// for longer string literals use xarr_from_string_literal2
#define xarr_from_string_literal(A, L, LO)                                    \
    xarr_from_string_literal2(A, L, LO, 0)

// hash function for a string using the xarr api
uint64_t xstring_hash(const xarr(char) * key) {
    uint64_t hash = 14695981039346656037ul;

    for (size_t i = 0; i < xalen(*key); ++i) {
        hash ^= (uint64_t)(uint8_t)(*key)[i];
        hash *= 1099511628211ul;
    }

    return hash;
}

// equal function for a string using the xarr api
bool xstring_eq(const xarr(char) * a, const xarr(char) * b) {
    return xalen(*a) == xalen(*b) && memcmp(*a, *b, xalen(*a)) == 0;
}

// hash function for a null terminated string
uint64_t xstring_hash_nt(const char **key) {
    uint64_t hash = 14695981039346656037ul;

    for (size_t i = 0; i < strlen(*key); ++i) {
        hash ^= (uint64_t)(uint8_t)(*key)[i];
        hash *= 1099511628211ul;
    }

    return hash;
}

// equal function for a null terminated string
bool xstring_eq_nt(const char **a, const char **b) {
    return strlen(*a) == strlen(*b) && memcmp(*a, *b, strlen(*a)) == 0;
}

#define _XHEX_0   "\x00"
#define _XHEX_1   "\x01"
#define _XHEX_2   "\x02"
#define _XHEX_3   "\x03"
#define _XHEX_4   "\x04"
#define _XHEX_5   "\x05"
#define _XHEX_6   "\x06"
#define _XHEX_7   "\x07"
#define _XHEX_8   "\x08"
#define _XHEX_9   "\x09"
#define _XHEX_10  "\x0a"
#define _XHEX_11  "\x0b"
#define _XHEX_12  "\x0c"
#define _XHEX_13  "\x0d"
#define _XHEX_14  "\x0e"
#define _XHEX_15  "\x0f"
#define _XHEX_16  "\x10"
#define _XHEX_17  "\x11"
#define _XHEX_18  "\x12"
#define _XHEX_19  "\x13"
#define _XHEX_20  "\x14"
#define _XHEX_21  "\x15"
#define _XHEX_22  "\x16"
#define _XHEX_23  "\x17"
#define _XHEX_24  "\x18"
#define _XHEX_25  "\x19"
#define _XHEX_26  "\x1a"
#define _XHEX_27  "\x1b"
#define _XHEX_28  "\x1c"
#define _XHEX_29  "\x1d"
#define _XHEX_30  "\x1e"
#define _XHEX_31  "\x1f"
#define _XHEX_32  "\x20"
#define _XHEX_33  "\x21"
#define _XHEX_34  "\x22"
#define _XHEX_35  "\x23"
#define _XHEX_36  "\x24"
#define _XHEX_37  "\x25"
#define _XHEX_38  "\x26"
#define _XHEX_39  "\x27"
#define _XHEX_40  "\x28"
#define _XHEX_41  "\x29"
#define _XHEX_42  "\x2a"
#define _XHEX_43  "\x2b"
#define _XHEX_44  "\x2c"
#define _XHEX_45  "\x2d"
#define _XHEX_46  "\x2e"
#define _XHEX_47  "\x2f"
#define _XHEX_48  "\x30"
#define _XHEX_49  "\x31"
#define _XHEX_50  "\x32"
#define _XHEX_51  "\x33"
#define _XHEX_52  "\x34"
#define _XHEX_53  "\x35"
#define _XHEX_54  "\x36"
#define _XHEX_55  "\x37"
#define _XHEX_56  "\x38"
#define _XHEX_57  "\x39"
#define _XHEX_58  "\x3a"
#define _XHEX_59  "\x3b"
#define _XHEX_60  "\x3c"
#define _XHEX_61  "\x3d"
#define _XHEX_62  "\x3e"
#define _XHEX_63  "\x3f"
#define _XHEX_64  "\x40"
#define _XHEX_65  "\x41"
#define _XHEX_66  "\x42"
#define _XHEX_67  "\x43"
#define _XHEX_68  "\x44"
#define _XHEX_69  "\x45"
#define _XHEX_70  "\x46"
#define _XHEX_71  "\x47"
#define _XHEX_72  "\x48"
#define _XHEX_73  "\x49"
#define _XHEX_74  "\x4a"
#define _XHEX_75  "\x4b"
#define _XHEX_76  "\x4c"
#define _XHEX_77  "\x4d"
#define _XHEX_78  "\x4e"
#define _XHEX_79  "\x4f"
#define _XHEX_80  "\x50"
#define _XHEX_81  "\x51"
#define _XHEX_82  "\x52"
#define _XHEX_83  "\x53"
#define _XHEX_84  "\x54"
#define _XHEX_85  "\x55"
#define _XHEX_86  "\x56"
#define _XHEX_87  "\x57"
#define _XHEX_88  "\x58"
#define _XHEX_89  "\x59"
#define _XHEX_90  "\x5a"
#define _XHEX_91  "\x5b"
#define _XHEX_92  "\x5c"
#define _XHEX_93  "\x5d"
#define _XHEX_94  "\x5e"
#define _XHEX_95  "\x5f"
#define _XHEX_96  "\x60"
#define _XHEX_97  "\x61"
#define _XHEX_98  "\x62"
#define _XHEX_99  "\x63"
#define _XHEX_100 "\x64"
#define _XHEX_101 "\x65"
#define _XHEX_102 "\x66"
#define _XHEX_103 "\x67"
#define _XHEX_104 "\x68"
#define _XHEX_105 "\x69"
#define _XHEX_106 "\x6a"
#define _XHEX_107 "\x6b"
#define _XHEX_108 "\x6c"
#define _XHEX_109 "\x6d"
#define _XHEX_110 "\x6e"
#define _XHEX_111 "\x6f"
#define _XHEX_112 "\x70"
#define _XHEX_113 "\x71"
#define _XHEX_114 "\x72"
#define _XHEX_115 "\x73"
#define _XHEX_116 "\x74"
#define _XHEX_117 "\x75"
#define _XHEX_118 "\x76"
#define _XHEX_119 "\x77"
#define _XHEX_120 "\x78"
#define _XHEX_121 "\x79"
#define _XHEX_122 "\x7a"
#define _XHEX_123 "\x7b"
#define _XHEX_124 "\x7c"
#define _XHEX_125 "\x7d"
#define _XHEX_126 "\x7e"
#define _XHEX_127 "\x7f"
#define _XHEX_128 "\x80"
#define _XHEX_129 "\x81"
#define _XHEX_130 "\x82"
#define _XHEX_131 "\x83"
#define _XHEX_132 "\x84"
#define _XHEX_133 "\x85"
#define _XHEX_134 "\x86"
#define _XHEX_135 "\x87"
#define _XHEX_136 "\x88"
#define _XHEX_137 "\x89"
#define _XHEX_138 "\x8a"
#define _XHEX_139 "\x8b"
#define _XHEX_140 "\x8c"
#define _XHEX_141 "\x8d"
#define _XHEX_142 "\x8e"
#define _XHEX_143 "\x8f"
#define _XHEX_144 "\x90"
#define _XHEX_145 "\x91"
#define _XHEX_146 "\x92"
#define _XHEX_147 "\x93"
#define _XHEX_148 "\x94"
#define _XHEX_149 "\x95"
#define _XHEX_150 "\x96"
#define _XHEX_151 "\x97"
#define _XHEX_152 "\x98"
#define _XHEX_153 "\x99"
#define _XHEX_154 "\x9a"
#define _XHEX_155 "\x9b"
#define _XHEX_156 "\x9c"
#define _XHEX_157 "\x9d"
#define _XHEX_158 "\x9e"
#define _XHEX_159 "\x9f"
#define _XHEX_160 "\xa0"
#define _XHEX_161 "\xa1"
#define _XHEX_162 "\xa2"
#define _XHEX_163 "\xa3"
#define _XHEX_164 "\xa4"
#define _XHEX_165 "\xa5"
#define _XHEX_166 "\xa6"
#define _XHEX_167 "\xa7"
#define _XHEX_168 "\xa8"
#define _XHEX_169 "\xa9"
#define _XHEX_170 "\xaa"
#define _XHEX_171 "\xab"
#define _XHEX_172 "\xac"
#define _XHEX_173 "\xad"
#define _XHEX_174 "\xae"
#define _XHEX_175 "\xaf"
#define _XHEX_176 "\xb0"
#define _XHEX_177 "\xb1"
#define _XHEX_178 "\xb2"
#define _XHEX_179 "\xb3"
#define _XHEX_180 "\xb4"
#define _XHEX_181 "\xb5"
#define _XHEX_182 "\xb6"
#define _XHEX_183 "\xb7"
#define _XHEX_184 "\xb8"
#define _XHEX_185 "\xb9"
#define _XHEX_186 "\xba"
#define _XHEX_187 "\xbb"
#define _XHEX_188 "\xbc"
#define _XHEX_189 "\xbd"
#define _XHEX_190 "\xbe"
#define _XHEX_191 "\xbf"
#define _XHEX_192 "\xc0"
#define _XHEX_193 "\xc1"
#define _XHEX_194 "\xc2"
#define _XHEX_195 "\xc3"
#define _XHEX_196 "\xc4"
#define _XHEX_197 "\xc5"
#define _XHEX_198 "\xc6"
#define _XHEX_199 "\xc7"
#define _XHEX_200 "\xc8"
#define _XHEX_201 "\xc9"
#define _XHEX_202 "\xca"
#define _XHEX_203 "\xcb"
#define _XHEX_204 "\xcc"
#define _XHEX_205 "\xcd"
#define _XHEX_206 "\xce"
#define _XHEX_207 "\xcf"
#define _XHEX_208 "\xd0"
#define _XHEX_209 "\xd1"
#define _XHEX_210 "\xd2"
#define _XHEX_211 "\xd3"
#define _XHEX_212 "\xd4"
#define _XHEX_213 "\xd5"
#define _XHEX_214 "\xd6"
#define _XHEX_215 "\xd7"
#define _XHEX_216 "\xd8"
#define _XHEX_217 "\xd9"
#define _XHEX_218 "\xda"
#define _XHEX_219 "\xdb"
#define _XHEX_220 "\xdc"
#define _XHEX_221 "\xdd"
#define _XHEX_222 "\xde"
#define _XHEX_223 "\xdf"
#define _XHEX_224 "\xe0"
#define _XHEX_225 "\xe1"
#define _XHEX_226 "\xe2"
#define _XHEX_227 "\xe3"
#define _XHEX_228 "\xe4"
#define _XHEX_229 "\xe5"
#define _XHEX_230 "\xe6"
#define _XHEX_231 "\xe7"
#define _XHEX_232 "\xe8"
#define _XHEX_233 "\xe9"
#define _XHEX_234 "\xea"
#define _XHEX_235 "\xeb"
#define _XHEX_236 "\xec"
#define _XHEX_237 "\xed"
#define _XHEX_238 "\xee"
#define _XHEX_239 "\xef"
#define _XHEX_240 "\xf0"
#define _XHEX_241 "\xf1"
#define _XHEX_242 "\xf2"
#define _XHEX_243 "\xf3"
#define _XHEX_244 "\xf4"
#define _XHEX_245 "\xf5"
#define _XHEX_246 "\xf6"
#define _XHEX_247 "\xf7"
#define _XHEX_248 "\xf8"
#define _XHEX_249 "\xf9"
#define _XHEX_250 "\xfa"
#define _XHEX_251 "\xfb"
#define _XHEX_252 "\xfc"
#define _XHEX_253 "\xfd"
#define _XHEX_254 "\xfe"
#define _XHEX_255 "\xff"

#endif
