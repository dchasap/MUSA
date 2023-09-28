/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf --language=ANSI-C --hash-function-name=fileextensions_hash --struct-type --lookup-function-name=fileextensions_lookup --output=src/driver/cxx-fileextensions.c --initializer-suffix=,SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN ./src/driver/cxx-fileextensions.gperf  */
/* Computed positions: -k'2,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 28 "./src/driver/cxx-fileextensions.gperf"

#include <string.h>
#include "cxx-driver.h"
#line 33 "./src/driver/cxx-fileextensions.gperf"
struct extensions_table_t;

#define TOTAL_KEYWORDS 32
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 82
/* maximum key range = 81, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
fileextensions_hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83,  1, 83, 83, 83, 83, 30, 83,
      83, 20, 83, 10, 83,  0, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 40, 83, 83,
      30, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      35, 83, 40, 35, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 15, 83, 10,
      83, 83, 25, 83, 83, 20, 15, 83,  0, 83,
      83,  5,  5, 83, 40,  0, 83, 15, 83, 83,
       0, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83, 83, 83, 83, 83,
      83, 83, 83, 83, 83, 83
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct extensions_table_t *
fileextensions_lookup (register const char *str, register unsigned int len)
{
  static struct extensions_table_t wordlist[] =
    {
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 61 "./src/driver/cxx-fileextensions.gperf"
      {".s",   SOURCE_LANGUAGE_ASSEMBLER,   SOURCE_KIND_DO_NOT_PROCESS},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 58 "./src/driver/cxx-fileextensions.gperf"
      {".so",  SOURCE_LANGUAGE_LINKER_DATA, SOURCE_KIND_UNKNOWN},
#line 65 "./src/driver/cxx-fileextensions.gperf"
      {".ocl",  SOURCE_SUBLANGUAGE_OPENCL,  SOURCE_KIND_DO_NOT_PROCESS | SOURCE_KIND_DO_NOT_EMBED | SOURCE_KIND_DO_NOT_COMPILE | SOURCE_KIND_DO_NOT_LINK},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 57 "./src/driver/cxx-fileextensions.gperf"
      {".o",   SOURCE_LANGUAGE_LINKER_DATA, SOURCE_KIND_UNKNOWN},
#line 64 "./src/driver/cxx-fileextensions.gperf"
      {".cl",   SOURCE_SUBLANGUAGE_OPENCL,  SOURCE_KIND_DO_NOT_PROCESS | SOURCE_KIND_DO_NOT_EMBED | SOURCE_KIND_DO_NOT_COMPILE | SOURCE_KIND_DO_NOT_LINK},
#line 40 "./src/driver/cxx-fileextensions.gperf"
      {".cxx", SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
#line 43 "./src/driver/cxx-fileextensions.gperf"
      {".c++", SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 39 "./src/driver/cxx-fileextensions.gperf"
      {".cp",  SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
#line 41 "./src/driver/cxx-fileextensions.gperf"
      {".cpp", SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
#line 66 "./src/driver/cxx-fileextensions.gperf"
      {".aocx", SOURCE_SUBLANGUAGE_OPENCL,  SOURCE_KIND_DO_NOT_PROCESS | SOURCE_KIND_DO_NOT_EMBED | SOURCE_KIND_DO_NOT_COMPILE | SOURCE_KIND_DO_NOT_LINK},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 35 "./src/driver/cxx-fileextensions.gperf"
      {".c",   SOURCE_LANGUAGE_C,           SOURCE_KIND_NOT_PREPROCESSED},
#line 38 "./src/driver/cxx-fileextensions.gperf"
      {".cc",  SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
#line 59 "./src/driver/cxx-fileextensions.gperf"
      {".obj", SOURCE_LANGUAGE_LINKER_DATA, SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 63 "./src/driver/cxx-fileextensions.gperf"
      {".cu",  SOURCE_SUBLANGUAGE_CUDA,     SOURCE_KIND_DO_NOT_PROCESS},
#line 49 "./src/driver/cxx-fileextensions.gperf"
      {".f77", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 60 "./src/driver/cxx-fileextensions.gperf"
      {".a",   SOURCE_LANGUAGE_LINKER_DATA, SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 50 "./src/driver/cxx-fileextensions.gperf"
      {".F77", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 53 "./src/driver/cxx-fileextensions.gperf"
      {".f95", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 36 "./src/driver/cxx-fileextensions.gperf"
      {".i",   SOURCE_LANGUAGE_C,           SOURCE_KIND_PREPROCESSED},
#line 37 "./src/driver/cxx-fileextensions.gperf"
      {".ii",  SOURCE_LANGUAGE_CXX,         SOURCE_KIND_PREPROCESSED},
#line 54 "./src/driver/cxx-fileextensions.gperf"
      {".F95", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 55 "./src/driver/cxx-fileextensions.gperf"
      {".f03", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 45 "./src/driver/cxx-fileextensions.gperf"
      {".f",   SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 56 "./src/driver/cxx-fileextensions.gperf"
      {".F03", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 51 "./src/driver/cxx-fileextensions.gperf"
      {".f90", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 46 "./src/driver/cxx-fileextensions.gperf"
      {".F",   SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 52 "./src/driver/cxx-fileextensions.gperf"
      {".F90", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FREE_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 47 "./src/driver/cxx-fileextensions.gperf"
      {".for", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_PREPROCESSED     | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 62 "./src/driver/cxx-fileextensions.gperf"
      {".S",   SOURCE_LANGUAGE_ASSEMBLER,   SOURCE_KIND_DO_NOT_PROCESS},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 48 "./src/driver/cxx-fileextensions.gperf"
      {".FOR", SOURCE_LANGUAGE_FORTRAN,     SOURCE_KIND_NOT_PREPROCESSED | SOURCE_KIND_FIXED_FORM},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 42 "./src/driver/cxx-fileextensions.gperf"
      {".CPP", SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
      {"",SOURCE_LANGUAGE_UNKNOWN,SOURCE_KIND_UNKNOWN},
#line 44 "./src/driver/cxx-fileextensions.gperf"
      {".C",   SOURCE_LANGUAGE_CXX,         SOURCE_KIND_NOT_PREPROCESSED}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = fileextensions_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 67 "./src/driver/cxx-fileextensions.gperf"

