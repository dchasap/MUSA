/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf --language=ANSI-C --hash-function-name=fortran_keywords_hash --struct-type --initializer-suffix=,0 --lookup-function-name=fortran_keywords_lookup --output=src/frontend/fortran/fortran03-keywords.c --ignore-case --compare-strncmp ./src/frontend/fortran/fortran03-keywords.gperf  */
/* Computed positions: -k'1-5' */

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

#line 28 "./src/frontend/fortran/fortran03-keywords.gperf"

#include <string.h>
#include "fortran03-scanner.h"
#include "fortran03-lexer.h"
#include "fortran03-utils.h"
#include "fortran03-parser-internal.h"
#line 36 "./src/frontend/fortran/fortran03-keywords.gperf"
struct fortran_keyword_tag;

#define TOTAL_KEYWORDS 192
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 531
/* maximum key range = 529, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static unsigned char gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_STRNCMP
#define GPERF_CASE_STRNCMP 1
static int
gperf_case_strncmp (register const char *s1, register const char *s2, register unsigned int n)
{
  for (; n > 0;)
    {
      unsigned char c1 = gperf_downcase[(unsigned char)*s1++];
      unsigned char c2 = gperf_downcase[(unsigned char)*s2++];
      if (c1 != 0 && c1 == c2)
        {
          n--;
          continue;
        }
      return (int)c1 - (int)c2;
    }
  return 0;
}
#endif

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
fortran_keywords_hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532,   0, 150,  20, 140,   5,
       60,  18,  89,  10, 532,  85,   0, 170,  20,  15,
       55,  30,  65,   0,  40, 180,   0,  50,  50, 200,
        0, 532, 532, 532, 532, 532, 532,   0, 150,  20,
      140,   5,  60,  18,  89,  10, 532,  85,   0, 170,
       20,  15,  55,  30,  65,   0,  40, 180,   0,  50,
       50, 200,   0, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532, 532, 532, 532, 532,
      532, 532, 532, 532, 532, 532
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct fortran_keyword_tag *
fortran_keywords_lookup (register const char *str, register unsigned int len)
{
  static struct fortran_keyword_tag wordlist[] =
    {
      {"",0}, {"",0}, {"",0},
#line 43 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"all", TOKEN_ALL},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 200 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"save", TOKEN_SAVE},
      {"",0}, {"",0},
#line 147 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"is", TOKEN_IS},
      {"",0},
#line 86 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"else", TOKEN_ELSE},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 206 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"size", TOKEN_SIZE},
      {"",0},
#line 57 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"c", TOKEN_C},
      {"",0}, {"",0},
#line 58 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"call", TOKEN_CALL},
#line 61 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"class", TOKEN_CLASS},
#line 87 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"elseif", TOKEN_ELSEIF},
      {"",0},
#line 149 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"len", TOKEN_LEN},
#line 59 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"case", TOKEN_CASE},
      {"",0}, {"",0},
#line 137 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"in", TOKEN_IN},
      {"",0},
#line 47 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"assign", TOKEN_ASSIGN},
#line 129 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"go", TOKEN_GO},
#line 201 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"select", TOKEN_SELECT},
      {"",0},
#line 48 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"assignment", TOKEN_ASSIGNMENT},
      {"",0},
#line 202 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"selectcase", TOKEN_SELECTCASE},
      {"",0}, {"",0},
#line 45 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"allocate", TOKEN_ALLOCATE},
#line 49 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"associate", TOKEN_ASSOCIATE},
#line 62 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"close", TOKEN_CLOSE},
#line 44 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"allocatable", TOKEN_ALLOCATABLE},
#line 46 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"allstop", TOKEN_ALLSTOP},
      {"",0}, {"",0}, {"",0},
#line 39 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"access", TOKEN_ACCESS},
#line 205 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"sign", TOKEN_SIGN},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 217 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"to", TOKEN_TO},
#line 144 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"iolength", TOKEN_IOLENGTH},
#line 175 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pass", TOKEN_PASS},
      {"",0}, {"",0}, {"",0},
#line 225 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"volatile", TOKEN_VOLATILE},
#line 161 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"none", TOKEN_NONE},
      {"",0}, {"",0},
#line 70 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"convert", TOKEN_CONVERT},
      {"",0},
#line 88 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"elsewhere", TOKEN_ELSEWHERE},
#line 151 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"logical", TOKEN_LOGICAL},
#line 146 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"iostat", TOKEN_IOSTAT},
#line 132 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"if", TOKEN_IF},
#line 180 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pos", TOKEN_POS},
#line 192 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"real", TOKEN_REAL},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 119 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"file", TOKEN_FILE},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 208 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"stat", TOKEN_STAT},
      {"",0},
#line 224 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"vector", TOKEN_VECTOR},
      {"",0},
#line 111 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"eor", TOKEN_EOR},
      {"",0}, {"",0},
#line 41 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"action", TOKEN_ACTION},
#line 130 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"goto", TOKEN_GOTO},
#line 193 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"rec", TOKEN_REC},
#line 194 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"recl", TOKEN_RECL},
#line 120 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"final", TOKEN_FINAL},
#line 162 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"nopass", TOKEN_NOPASS},
      {"",0}, {"",0},
#line 166 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"open", TOKEN_OPEN},
#line 140 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"integer", TOKEN_INTEGER},
#line 141 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"intent", TOKEN_INTENT},
      {"",0},
#line 67 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"contains", TOKEN_CONTAINS},
#line 226 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"wait", TOKEN_WAIT},
      {"",0},
#line 168 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"opened", TOKEN_OPENED},
      {"",0}, {"",0},
#line 116 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"exit", TOKEN_EXIT},
#line 115 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"exist", TOKEN_EXIST},
      {"",0}, {"",0},
#line 69 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"continue", TOKEN_CONTINUE},
#line 210 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"stop", TOKEN_STOP},
#line 68 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"contiguous", TOKEN_CONTIGUOUS},
#line 211 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"stream", TOKEN_STREAM},
      {"",0}, {"",0}, {"",0},
#line 128 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"generic", TOKEN_GENERIC},
#line 167 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"opencl", TOKEN_OPENCL},
      {"",0}, {"",0},
#line 150 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"lock", TOKEN_LOCK},
#line 178 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pixel", TOKEN_PIXEL},
      {"",0},
#line 117 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"extends", TOKEN_EXTENDS},
#line 181 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"position", TOKEN_POSITION},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 215 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"target", TOKEN_TARGET},
      {"",0}, {"",0},
#line 184 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"private", TOKEN_PRIVATE},
#line 113 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"err", TOKEN_ERR},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 170 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"optional", TOKEN_OPTIONAL},
      {"",0}, {"",0},
#line 123 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"forall", TOKEN_FORALL},
#line 179 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pointer", TOKEN_POINTER},
#line 169 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"operator", TOKEN_OPERATOR},
#line 142 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"interface", TOKEN_INTERFACE},
      {"",0}, {"",0},
#line 131 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"id", TOKEN_ID},
#line 71 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"critical", TOKEN_CRITICAL},
#line 143 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"intrinsic", TOKEN_INTRINSIC},
#line 74 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"deallocate", TOKEN_DEALLOCATE},
#line 198 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"rewind", TOKEN_REWIND},
#line 81 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"do", TOKEN_DO},
#line 216 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"then", TOKEN_THEN},
#line 228 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"while", TOKEN_WHILE},
      {"",0},
#line 172 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"overridable", TOKEN_OVERRIDABLE},
      {"",0}, {"",0},
#line 182 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"precision", TOKEN_PRECISION},
      {"",0}, {"",0},
#line 42 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"advance", TOKEN_ADVANCE},
#line 90 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"end", TOKEN_END},
#line 185 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"procedure", TOKEN_PROCEDURE},
      {"",0}, {"",0}, {"",0},
#line 118 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"external", TOKEN_EXTERNAL},
      {"",0},
#line 229 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"write", TOKEN_WRITE},
      {"",0},
#line 91 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endassociate", TOKEN_ENDASSOCIATE},
      {"",0},
#line 103 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endselect", TOKEN_ENDSELECT},
      {"",0}, {"",0}, {"",0},
#line 60 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"character", TOKEN_CHARACTER},
#line 73 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"data", TOKEN_DATA},
      {"",0}, {"",0},
#line 159 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"nextrec", TOKEN_NEXTREC},
#line 222 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"use", TOKEN_USE},
#line 187 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"protected", TOKEN_PROTECTED},
#line 223 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"value", TOKEN_VALUE},
      {"",0}, {"",0},
#line 160 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"nml", TOKEN_NML},
#line 85 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"elemental", TOKEN_ELEMENTAL},
#line 183 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"print", TOKEN_PRINT},
      {"",0}, {"",0},
#line 173 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pad", TOKEN_PAD},
#line 155 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"name", TOKEN_NAME},
      {"",0}, {"",0}, {"",0},
#line 157 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"namelist", TOKEN_NAMELIST},
      {"",0}, {"",0}, {"",0},
#line 99 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endinterface", TOKEN_ENDINTERFACE},
#line 89 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"encoding", TOKEN_ENCODING},
#line 133 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"images", TOKEN_IMAGES},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 190 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"read", TOKEN_READ},
      {"",0}, {"",0},
#line 163 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"nullify", TOKEN_NULLIFY},
#line 145 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"iomsg", TOKEN_IOMSG},
#line 227 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"where", TOKEN_WHERE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 186 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"program", TOKEN_PROGRAM},
      {"",0}, {"",0},
#line 203 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"sequence", TOKEN_SEQUENCE},
      {"",0},
#line 204 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"sequential", TOKEN_SEQUENTIAL},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 112 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"equivalence", TOKEN_EQUIVALENCE},
#line 177 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pending", TOKEN_PENDING},
#line 171 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"out", TOKEN_OUT},
#line 165 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"only", TOKEN_ONLY},
#line 98 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endif", TOKEN_ENDIF},
#line 221 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"unlock", TOKEN_UNLOCK},
#line 96 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endfile", TOKEN_ENDFILE},
      {"",0},
#line 214 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"sync", TOKEN_SYNC},
#line 176 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pause", TOKEN_PAUSE},
#line 80 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"direct", TOKEN_DIRECT},
      {"",0},
#line 40 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"acquired", TOKEN_ACQUIRED},
      {"",0},
#line 72 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"cycle", TOKEN_CYCLE},
      {"",0},
#line 50 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"asynchronous", TOKEN_ASYNCHRONOUS},
#line 134 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"implicit", TOKEN_IMPLICIT},
#line 220 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"unit", TOKEN_UNIT},
      {"",0},
#line 196 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"result", TOKEN_RESULT},
#line 139 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"inquire", TOKEN_INQUIRE},
      {"",0},
#line 148 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"kind", TOKEN_KIND},
#line 53 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"blank", TOKEN_BLANK},
#line 94 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endcritical", TOKEN_ENDCRITICAL},
      {"",0},
#line 38 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"abstract", TOKEN_ABSTRACT},
#line 51 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"backspace", TOKEN_BACKSPACE},
#line 66 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"concurrent", TOKEN_CONCURRENT},
#line 209 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"status", TOKEN_STATUS},
#line 65 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"complex", TOKEN_COMPLEX},
      {"",0},
#line 191 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"readwrite", TOKEN_READWRITE},
#line 138 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"inout", TOKEN_INOUT},
      {"",0}, {"",0},
#line 122 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"fmt", TOKEN_FMT},
      {"",0},
#line 54 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"block", TOKEN_BLOCK},
      {"",0}, {"",0}, {"",0},
#line 55 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"blockdata", TOKEN_BLOCKDATA},
      {"",0}, {"",0},
#line 158 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"newunit", TOKEN_NEWUNIT},
#line 77 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"deferred", TOKEN_DEFERRED},
      {"",0}, {"",0},
#line 207 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"source", TOKEN_SOURCE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0},
#line 102 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endprogram", TOKEN_ENDPROGRAM},
      {"",0},
#line 101 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endprocedure", TOKEN_ENDPROCEDURE},
      {"",0},
#line 174 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"parameter", TOKEN_PARAMETER},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 218 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"type", TOKEN_TYPE},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 189 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"pure", TOKEN_PURE},
      {"",0},
#line 114 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"errmsg", TOKEN_ERRMSG},
#line 107 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endwhere", TOKEN_ENDWHERE},
      {"",0},
#line 124 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"form", TOKEN_FORM},
      {"",0},
#line 125 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"format", TOKEN_FORMAT},
      {"",0}, {"",0},
#line 126 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"formatted", TOKEN_FORMATTED},
      {"",0},
#line 135 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"import", TOKEN_IMPORT},
      {"",0},
#line 92 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endblock", TOKEN_ENDBLOCK},
#line 52 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"bind", TOKEN_BIND},
#line 95 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"enddo", TOKEN_ENDDO},
      {"",0},
#line 93 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endblockdata", TOKEN_ENDBLOCKDATA},
#line 127 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"function", TOKEN_FUNCTION},
#line 154 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"mold", TOKEN_MOLD},
#line 78 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"delim", TOKEN_DELIM},
      {"",0}, {"",0}, {"",0},
#line 121 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"flush", TOKEN_FLUSH},
#line 108 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"entry", TOKEN_ENTRY},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 156 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"named", TOKEN_NAMED},
      {"",0}, {"",0}, {"",0},
#line 195 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"recursive", TOKEN_RECURSIVE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 219 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"unformatted", TOKEN_UNFORMATTED},
#line 75 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"decimal", TOKEN_DECIMAL},
      {"",0},
#line 79 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"dimension", TOKEN_DIMENSION},
      {"",0}, {"",0},
#line 104 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endsubmodule", TOKEN_ENDSUBMODULE},
#line 105 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endsubroutine", TOKEN_ENDSUBROUTINE},
#line 100 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endmodule", TOKEN_ENDMODULE},
      {"",0},
#line 197 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"return", TOKEN_RETURN},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 63 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"codimension", TOKEN_CODIMENSION},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 109 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"enum", TOKEN_ENUM},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0},
#line 110 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"enumerator", TOKEN_ENUMERATOR},
      {"",0},
#line 76 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"default", TOKEN_DEFAULT},
      {"",0}, {"",0}, {"",0},
#line 64 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"common", TOKEN_COMMON},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 188 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"public", TOKEN_PUBLIC},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0},
#line 106 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endtype", TOKEN_ENDTYPE},
      {"",0}, {"",0}, {"",0},
#line 97 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"endfunction", TOKEN_ENDFUNCTION},
      {"",0}, {"",0}, {"",0},
#line 213 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"subroutine", TOKEN_SUBROUTINE},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 199 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"round", TOKEN_ROUND},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 152 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"memory", TOKEN_MEMORY},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0},
#line 56 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"buffered", TOKEN_BUFFERED},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0},
#line 136 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"impure", TOKEN_IMPURE},
      {"",0}, {"",0}, {"",0}, {"",0},
#line 82 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"double", TOKEN_DOUBLE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 83 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"doublecomplex", TOKEN_DOUBLECOMPLEX},
      {"",0},
#line 84 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"doubleprecision", TOKEN_DOUBLEPRECISION},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0},
#line 153 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"module", TOKEN_MODULE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 212 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"submodule", TOKEN_SUBMODULE},
      {"",0}, {"",0}, {"",0}, {"",0}, {"",0}, {"",0},
#line 164 "./src/frontend/fortran/fortran03-keywords.gperf"
      {"number", TOKEN_NUMBER}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = fortran_keywords_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strncmp (str, s, len) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
#line 230 "./src/frontend/fortran/fortran03-keywords.gperf"

