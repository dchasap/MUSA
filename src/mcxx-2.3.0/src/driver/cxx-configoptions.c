/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf --language=ANSI-C --hash-function-name=configoptions_hash --struct-type --lookup-function-name=configoptions_lookup --output=src/driver/cxx-configoptions.c --initializer-suffix=,NULL ./src/driver/cxx-configoptions.gperf  */
/* Computed positions: -k'3,$' */

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

#line 28 "./src/driver/cxx-configoptions.gperf"

#include "cxx-driver.h"
#include "cxx-configfile.h"
#include <string.h>
#line 34 "./src/driver/cxx-configoptions.gperf"
struct configuration_directive_t;

#define TOTAL_KEYWORDS 26
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 28
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 56
/* maximum key range = 52, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
configoptions_hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57,  0, 57, 57,
       5,  5, 57, 57, 57, 57, 57, 57, 57, 30,
       5,  0,  5, 57,  0, 10, 15, 57,  0, 57,
       0,  0, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 57, 57
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct configuration_directive_t *
configoptions_lookup (register const char *str, register unsigned int len)
{
  static struct configuration_directive_t wordlist[] =
    {
      {"",NULL}, {"",NULL}, {"",NULL}, {"",NULL}, {"",NULL},
#line 61 "./src/driver/cxx-configoptions.gperf"
      {"error", config_set_error_message},
      {"",NULL}, {"",NULL}, {"",NULL},
#line 60 "./src/driver/cxx-configoptions.gperf"
      {"target_ar", config_set_target_ar},
      {"",NULL}, {"",NULL},
#line 52 "./src/driver/cxx-configoptions.gperf"
      {"environ", config_set_environment},
#line 50 "./src/driver/cxx-configoptions.gperf"
      {"pragma_prefix", config_add_preprocessor_prefix},
#line 58 "./src/driver/cxx-configoptions.gperf"
      {"target_objcopy", config_set_target_objcopy},
      {"",NULL}, {"",NULL}, {"",NULL},
#line 36 "./src/driver/cxx-configoptions.gperf"
      {"language", config_set_language},
#line 59 "./src/driver/cxx-configoptions.gperf"
      {"target_objdump", config_set_target_objdump},
      {"",NULL},
#line 44 "./src/driver/cxx-configoptions.gperf"
      {"linker_name", config_set_linker_name},
      {"",NULL},
#line 49 "./src/driver/cxx-configoptions.gperf"
      {"codegen_phase", config_set_codegen_phase},
#line 54 "./src/driver/cxx-configoptions.gperf"
      {"target_options", config_set_target_options},
#line 53 "./src/driver/cxx-configoptions.gperf"
      {"fortran_arrayd-descriptor", config_set_fortran_array_descriptor},
#line 51 "./src/driver/cxx-configoptions.gperf"
      {"environment", config_set_environment},
#line 38 "./src/driver/cxx-configoptions.gperf"
      {"preprocessor_name", config_set_preprocessor_name},
#line 45 "./src/driver/cxx-configoptions.gperf"
      {"linker_options_pre", config_set_linker_options_pre},
#line 46 "./src/driver/cxx-configoptions.gperf"
      {"linker_options", config_set_linker_options},
#line 56 "./src/driver/cxx-configoptions.gperf"
      {"fortran_preprocessor_name", config_set_fortran_preprocessor_name},
      {"",NULL},
#line 37 "./src/driver/cxx-configoptions.gperf"
      {"options", config_set_options},
#line 40 "./src/driver/cxx-configoptions.gperf"
      {"prescanner_options", config_set_prescanner_options},
      {"",NULL},
#line 39 "./src/driver/cxx-configoptions.gperf"
      {"preprocessor_options", config_set_preprocessor_options},
      {"",NULL}, {"",NULL},
#line 57 "./src/driver/cxx-configoptions.gperf"
      {"fortran_preprocessor_options", config_set_fortran_preprocessor_options},
#line 47 "./src/driver/cxx-configoptions.gperf"
      {"linker_options_post", config_set_linker_options_post},
      {"",NULL}, {"",NULL},
#line 55 "./src/driver/cxx-configoptions.gperf"
      {"compiler_dto", config_set_compiler_dto},
      {"",NULL},
#line 41 "./src/driver/cxx-configoptions.gperf"
      {"preprocessor_uses_stdout", config_set_preprocessor_uses_stdout},
      {"",NULL}, {"",NULL}, {"",NULL},
#line 42 "./src/driver/cxx-configoptions.gperf"
      {"compiler_name", config_set_compiler_name},
#line 48 "./src/driver/cxx-configoptions.gperf"
      {"compiler_phase", config_add_compiler_phase},
      {"",NULL}, {"",NULL}, {"",NULL}, {"",NULL}, {"",NULL},
      {"",NULL},
#line 43 "./src/driver/cxx-configoptions.gperf"
      {"compiler_options", config_set_compiler_options}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = configoptions_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 62 "./src/driver/cxx-configoptions.gperf"

