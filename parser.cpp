/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <cstdio>
#include <string.h>
#include "ast.hpp"
#include "lexer.hpp"
#include "symbol.hpp"
extern FILE *yyin;
SymbolTable st;

#line 76 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_and = 258,
    T_bool = 259,
    T_char = 260,
    T_decl = 261,
    T_def = 262,
    T_else = 263,
    T_elsif = 264,
    T_end = 265,
    T_exit = 266,
    T_false = 267,
    T_for = 268,
    T_head = 269,
    T_if = 270,
    T_int = 271,
    T_list = 272,
    T_mod = 273,
    T_new = 274,
    T_not = 275,
    T_nil = 276,
    T_nil2 = 277,
    T_or = 278,
    T_ref = 279,
    T_return = 280,
    T_skip = 281,
    T_tail = 282,
    T_true = 283,
    T_le = 284,
    T_ge = 285,
    T_ne = 286,
    T_assign = 287,
    UMINUS = 288,
    UPLUS = 289,
    T_id = 290,
    T_const = 291,
    T_string = 292,
    T_singlechar = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 51 "parser.y" /* yacc.c:355  */

    char *name;
    int num;
    unsigned char c;

    Expr *expr;
    Atom *atom;
    ExprList *exprlist;
    FunctionCall *call;
    SimpleList *simplelist;
    Simple *simple;
    Stmt *stmt;
    StmtBody *stmtbody;
    TonyType *type;
    VarList *varlist;
    Formal *formal;
    FormalList *formallist;
    Header *header;
    FunctionDeclaration *funcdecl;
    FunctionDefinition *funcdef;
    Elsif *elsif;
    Else *els;
    If *iff;
    For *fo;
 

#line 182 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 199 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   546

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  82
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    36,     2,     2,     2,     2,
      48,    49,    39,    37,    51,    38,     2,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    47,    50,
      34,    33,    35,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    41,    42,
      43,    44,    45,    46
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   109,   109,   119,   123,   124,   125,   126,   130,   131,
     132,   133,   137,   141,   142,   146,   147,   151,   155,   156,
     156,   160,   161,   162,   163,   164,   168,   169,   170,   171,
     172,   176,   180,   181,   185,   189,   190,   194,   195,   199,
     203,   204,   205,   209,   216,   217,   221,   222,   226,   230,
     231,   235,   236,   237,   238,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"and\"", "\"bool\"", "\"char\"",
  "\"decl\"", "\"def\"", "\"else\"", "\"elsif\"", "\"end\"", "\"exit\"",
  "\"false\"", "\"for\"", "\"head\"", "\"if\"", "\"int\"", "\"list\"",
  "\"mod\"", "\"new\"", "\"not\"", "\"nil\"", "\"nil?\"", "\"or\"",
  "\"ref\"", "\"return\"", "\"skip\"", "\"tail\"", "\"true\"", "\"<=\"",
  "\">=\"", "\"<>\"", "\":=\"", "'='", "'<'", "'>'", "'#'", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "UPLUS", "T_id", "T_const", "T_string",
  "T_singlechar", "':'", "'('", "')'", "';'", "','", "'['", "']'",
  "$accept", "Program", "Func_def", "Func_def_dec", "Header", "Func_Decl",
  "Par", "Formal", "Var_Def", "Var_Comma", "$@1", "Type", "Stmt",
  "Stmt_Body", "Stmt_Full", "If_Clause", "Elsif_Clause", "Else_Clause",
  "For_Clause", "Simple", "Simple_List", "Simple_Comma", "Call",
  "Expr_List", "Expr_Comma", "Atom", "Expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    61,    60,    62,    35,    43,    45,    42,
      47,   288,   289,   290,   291,   292,   293,    58,    40,    41,
      59,    44,    91,    93
};
# endif

#define YYPACT_NINF -106

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-106)))

#define YYTABLE_NINF -55

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-55)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       3,     4,    15,  -106,  -106,  -106,  -106,    -9,   -18,    20,
       9,  -106,    22,     1,   490,     8,    21,    41,    22,  -106,
      -1,  -106,    25,     4,   490,   105,   490,   490,    24,  -106,
    -106,  -106,    65,    34,    35,  -106,  -106,  -106,    91,   471,
     471,  -106,    40,  -106,   105,    89,  -106,  -106,  -106,   -21,
      12,  -106,  -106,  -106,    -1,    -1,  -106,    78,  -106,    71,
      73,  -106,    77,    22,   471,  -106,    80,    87,  -106,   471,
     471,  -106,  -106,   471,  -106,    85,   323,   419,   387,   105,
    -106,  -106,   471,   471,    95,  -106,    35,    91,  -106,   471,
     471,    93,   492,   471,   471,  -106,  -106,   229,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   471,   105,  -106,   100,   183,  -106,   419,   129,  -106,
    -106,    71,   206,   252,   375,   276,   299,  -106,   492,  -106,
     442,   506,   506,   506,   506,   506,   506,    -4,     6,     6,
    -106,  -106,   142,  -106,   471,  -106,  -106,  -106,  -106,    91,
    -106,   154,  -106,  -106,   471,   145,   183,   107,  -106,   346,
     108,   151,  -106,   105,   105,   105,  -106,   160,   142,  -106,
    -106,  -106
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,    23,    22,    21,     0,     0,     0,
       0,     1,     0,     0,     7,     0,     0,     0,     0,    10,
      14,    16,     0,     0,     7,     0,     7,     7,     0,    24,
      25,    15,     0,     0,    18,    12,     4,    27,     0,     0,
       0,    40,    51,    52,    33,     0,    29,    30,    26,    42,
       0,     5,     6,     8,    14,    14,    11,     0,    17,    44,
       0,    80,     0,     0,     0,    82,     0,     0,    79,     0,
       0,    56,    57,     0,    54,    55,     0,    28,     0,    33,
      31,     3,     0,     0,     0,    13,    18,     0,    43,     0,
       0,     0,    64,     0,     0,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,    49,    32,    41,     0,     9,
      19,    44,     0,     0,     0,     0,     0,    58,    76,    69,
      77,    74,    75,    71,    70,    72,    73,    78,    65,    66,
      67,    68,    36,    47,     0,    48,    53,    20,    45,     0,
      62,     0,    61,    63,     0,    38,    49,     0,    81,     0,
       0,     0,    50,     0,     0,     0,    34,     0,    36,    37,
      39,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,   171,    52,   150,  -106,    43,    23,   101,    88,
    -106,     0,   -42,  -105,    96,  -106,    10,  -106,  -106,   -34,
      27,    58,   -25,  -106,    39,   -22,     2
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    24,    25,     9,    26,    33,    20,    27,    58,
     147,    22,    44,    45,    80,    46,   155,   161,    47,    48,
      60,    88,    74,   114,   145,    75,    76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    10,    79,    50,    59,     4,     5,   142,     4,     5,
       1,   -54,    17,    49,    99,    11,    50,     6,     7,    49,
       6,     7,    50,    10,    99,    18,     4,     5,     4,     5,
      13,   -54,   107,   108,   109,   110,   111,    79,     6,     7,
       6,     7,    77,    12,    82,   110,   111,     8,    18,    32,
      19,    54,    15,   121,    49,    55,    28,    50,   167,   168,
     169,    16,    49,    91,    83,    50,    92,    14,    34,     4,
       5,    95,    96,    53,    29,    97,    36,    16,    51,    52,
     115,     6,     7,    56,   117,   118,    57,    49,    78,    18,
      50,   122,   123,    16,    30,   125,   126,    84,    85,    81,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    21,    59,    37,    41,    38,    31,
      39,    86,    87,    89,    49,    90,   151,    50,    93,    21,
      40,    41,    98,    21,    42,    94,    43,    83,    49,    49,
      49,    50,    50,    50,   119,   124,   156,    99,    42,   143,
      43,   154,   100,   160,   163,   165,   159,    98,   101,   102,
     103,   166,   104,   105,   106,   107,   108,   109,   110,   111,
     170,     3,    99,    35,   120,   116,   157,   100,   171,   148,
       0,     0,   146,   101,   102,   103,    98,   104,   105,   106,
     107,   108,   109,   110,   111,   162,     0,     0,     0,     0,
       0,    99,     0,     0,     0,     0,   100,   158,     0,    98,
       0,     0,   101,   102,   103,     0,   104,   105,   106,   107,
     108,   109,   110,   111,    99,     0,     0,     0,     0,   100,
       0,     0,    98,     0,   144,   101,   102,   103,     0,   104,
     105,   106,   107,   108,   109,   110,   111,    99,     0,     0,
       0,     0,   100,     0,     0,    98,   149,     0,   101,   102,
     103,     0,   104,   105,   106,   107,   108,   109,   110,   111,
      99,     0,     0,     0,     0,   100,     0,     0,   127,    98,
       0,   101,   102,   103,     0,   104,   105,   106,   107,   108,
     109,   110,   111,     0,    99,     0,     0,     0,     0,   100,
       0,   150,    98,     0,     0,   101,   102,   103,     0,   104,
     105,   106,   107,   108,   109,   110,   111,    99,     0,     0,
       0,     0,   100,     0,     0,   152,    98,     0,   101,   102,
     103,     0,   104,   105,   106,   107,   108,   109,   110,   111,
       0,    99,     0,     0,     0,     0,   100,     0,   153,    98,
       0,     0,   101,   102,   103,     0,   104,   105,   106,   107,
     108,   109,   110,   111,    99,     0,     0,     0,     0,   100,
     112,     0,     0,     0,     0,   101,   102,   103,     0,   104,
     105,   106,   107,   108,   109,   110,   111,    61,     0,    62,
       0,     0,     0,   164,    63,    64,    65,    66,     0,    61,
       0,    62,    67,    68,     0,     0,    63,    64,    65,    66,
       0,     0,    69,    70,    67,    68,     0,     0,    42,    71,
      43,    72,    98,    73,    69,    70,     0,     0,    29,     0,
      42,    71,    43,    72,     0,    73,   113,    99,     0,     0,
       0,     0,   100,     0,     0,    98,     0,     0,   101,   102,
     103,     0,   104,   105,   106,   107,   108,   109,   110,   111,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,   102,   103,     0,   104,   105,   106,   107,   108,
     109,   110,   111,    61,     0,    62,     0,     0,     0,     0,
      63,    64,    65,    66,     4,     5,    23,     1,    67,    68,
       0,     0,     0,     0,     0,     0,     6,     7,    69,    70,
      99,     0,     0,     0,    42,    71,    43,    72,     0,    73,
       0,   101,   102,   103,    99,   104,   105,   106,   107,   108,
     109,   110,   111,     0,     0,   -55,   -55,   -55,     0,   -55,
     -55,   -55,   107,   108,   109,   110,   111
};

static const yytype_int16 yycheck[] =
{
      25,     1,    44,    25,    38,     4,     5,   112,     4,     5,
       7,    32,    12,    38,    18,     0,    38,    16,    17,    44,
      16,    17,    44,    23,    18,    24,     4,     5,     4,     5,
      48,    52,    36,    37,    38,    39,    40,    79,    16,    17,
      16,    17,    40,    52,    32,    39,    40,    43,    24,    50,
      49,    28,    43,    87,    79,    32,    48,    79,   163,   164,
     165,    52,    87,    63,    52,    87,    64,    47,    43,     4,
       5,    69,    70,    49,    53,    73,    24,    52,    26,    27,
      78,    16,    17,    49,    82,    83,    51,   112,    48,    24,
     112,    89,    90,    52,    53,    93,    94,    54,    55,    10,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    13,   149,    11,    26,    13,    18,
      15,    43,    51,    50,   149,    48,   124,   149,    48,    28,
      25,    26,     3,    32,    43,    48,    45,    52,   163,   164,
     165,   163,   164,   165,    49,    52,   144,    18,    43,    49,
      45,     9,    23,     8,    47,    47,   154,     3,    29,    30,
      31,    10,    33,    34,    35,    36,    37,    38,    39,    40,
      10,     0,    18,    23,    86,    79,   149,    23,   168,   121,
      -1,    -1,    53,    29,    30,    31,     3,    33,    34,    35,
      36,    37,    38,    39,    40,   156,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    23,    53,    -1,     3,
      -1,    -1,    29,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    18,    -1,    -1,    -1,    -1,    23,
      -1,    -1,     3,    -1,    51,    29,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,     3,    50,    -1,    29,    30,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    49,     3,
      -1,    29,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    18,    -1,    -1,    -1,    -1,    23,
      -1,    49,     3,    -1,    -1,    29,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    49,     3,    -1,    29,    30,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    49,     3,
      -1,    -1,    29,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    18,    -1,    -1,    -1,    -1,    23,
      47,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    12,    -1,    14,
      -1,    -1,    -1,    47,    19,    20,    21,    22,    -1,    12,
      -1,    14,    27,    28,    -1,    -1,    19,    20,    21,    22,
      -1,    -1,    37,    38,    27,    28,    -1,    -1,    43,    44,
      45,    46,     3,    48,    37,    38,    -1,    -1,    53,    -1,
      43,    44,    45,    46,    -1,    48,    49,    18,    -1,    -1,
      -1,    -1,    23,    -1,    -1,     3,    -1,    -1,    29,    30,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    39,    40,    12,    -1,    14,    -1,    -1,    -1,    -1,
      19,    20,    21,    22,     4,     5,     6,     7,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    16,    17,    37,    38,
      18,    -1,    -1,    -1,    43,    44,    45,    46,    -1,    48,
      -1,    29,    30,    31,    18,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    29,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    39,    40
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    55,    56,     4,     5,    16,    17,    43,    58,
      65,     0,    52,    48,    47,    43,    52,    65,    24,    49,
      61,    62,    65,     6,    56,    57,    59,    62,    48,    53,
      53,    62,    50,    60,    43,    58,    57,    11,    13,    15,
      25,    26,    43,    45,    66,    67,    69,    72,    73,    76,
      79,    57,    57,    49,    61,    61,    49,    51,    63,    73,
      74,    12,    14,    19,    20,    21,    22,    27,    28,    37,
      38,    44,    46,    48,    76,    79,    80,    80,    48,    66,
      68,    10,    32,    52,    60,    60,    43,    51,    75,    50,
      48,    65,    80,    48,    48,    80,    80,    80,     3,    18,
      23,    29,    30,    31,    33,    34,    35,    36,    37,    38,
      39,    40,    47,    49,    77,    80,    68,    80,    80,    49,
      63,    73,    80,    80,    52,    80,    80,    49,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    67,    49,    51,    78,    53,    64,    75,    50,
      49,    80,    49,    49,     9,    70,    80,    74,    53,    80,
       8,    71,    78,    47,    47,    47,    10,    67,    67,    67,
      10,    70
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    56,    57,    57,    57,    57,    58,    58,
      58,    58,    59,    60,    60,    61,    61,    62,    63,    64,
      63,    65,    65,    65,    65,    65,    66,    66,    66,    66,
      66,    67,    68,    68,    69,    70,    70,    71,    71,    72,
      73,    73,    73,    74,    75,    75,    76,    76,    77,    78,
      78,    79,    79,    79,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     6,     2,     2,     2,     0,     4,     6,
       3,     5,     2,     3,     0,     2,     1,     3,     0,     0,
       4,     1,     1,     1,     3,     4,     1,     1,     2,     1,
       1,     2,     2,     0,     7,     5,     0,     3,     0,     9,
       1,     3,     1,     2,     0,     3,     3,     4,     2,     0,
       3,     1,     1,     4,     1,     1,     1,     1,     3,     2,
       2,     4,     4,     4,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     5,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 109 "parser.y" /* yacc.c:1646  */
    {
        (yyvsp[0].funcdef)->sem();
        std::cout << "Semantic analysis done!\n";
        (yyvsp[0].funcdef)->llvm_compile_and_dump();
        delete (yyvsp[0].funcdef);
        //std::cout << *$1;
    }
#line 1477 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 119 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-2].funcdef)->merge((yyvsp[-4].header), (yyvsp[-1].stmtbody)); (yyvsp[-2].funcdef)->reverse(); (yyval.funcdef) = (yyvsp[-2].funcdef);}
#line 1483 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 123 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].funcdef)->append((yyvsp[-1].funcdef)); (yyval.funcdef) = (yyvsp[0].funcdef);}
#line 1489 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 124 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].funcdef)->append((yyvsp[-1].funcdecl)); (yyval.funcdef) = (yyvsp[0].funcdef);}
#line 1495 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 125 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].funcdef)->append((yyvsp[-1].varlist)); (yyval.funcdef) = (yyvsp[0].funcdef);}
#line 1501 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 126 "parser.y" /* yacc.c:1646  */
    {(yyval.funcdef) = new FunctionDefinition();}
#line 1507 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 130 "parser.y" /* yacc.c:1646  */
    {(yyval.header) = new Header((yyvsp[-3].type), new Id(std::string((yyvsp[-2].name))), NULL);}
#line 1513 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 131 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-1].formallist)->append((yyvsp[-2].formal)); (yyvsp[-1].formallist)->reverse(); (yyval.header) = new Header((yyvsp[-5].type), new Id(std::string((yyvsp[-4].name))), (yyvsp[-1].formallist));}
#line 1519 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 132 "parser.y" /* yacc.c:1646  */
    {(yyval.header) = new Header(new Id(std::string((yyvsp[-2].name))),  NULL);}
#line 1525 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 133 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-1].formallist)->append((yyvsp[-2].formal)); (yyvsp[-1].formallist)->reverse(); (yyval.header) = new Header(new Id(std::string((yyvsp[-4].name))), (yyvsp[-1].formallist));}
#line 1531 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 137 "parser.y" /* yacc.c:1646  */
    {(yyval.funcdecl) = new FunctionDeclaration((yyvsp[0].header));}
#line 1537 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 141 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].formallist)->append((yyvsp[-1].formal)); (yyval.formallist) = (yyvsp[0].formallist);}
#line 1543 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 142 "parser.y" /* yacc.c:1646  */
    {(yyval.formallist) = new FormalList();}
#line 1549 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 146 "parser.y" /* yacc.c:1646  */
    {(yyval.formal) = new Formal((yyvsp[0].varlist), true);}
#line 1555 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 147 "parser.y" /* yacc.c:1646  */
    {(yyval.formal) = new Formal((yyvsp[0].varlist), false);}
#line 1561 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 151 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].varlist)->append(new Id(std::string((yyvsp[-1].name)))); (yyvsp[0].varlist)->set_type((yyvsp[-2].type)); (yyvsp[0].varlist)->reverse(); (yyval.varlist) = (yyvsp[0].varlist);}
#line 1567 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 155 "parser.y" /* yacc.c:1646  */
    {(yyval.varlist) = new VarList();}
#line 1573 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 156 "parser.y" /* yacc.c:1646  */
    {}
#line 1579 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 156 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-1].varlist)->append(new Id(std::string((yyvsp[-2].name)))); (yyval.varlist) = (yyvsp[-1].varlist);}
#line 1585 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 160 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = new TonyType(TYPE_int, nullptr);}
#line 1591 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 161 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = new TonyType(TYPE_char, nullptr);}
#line 1597 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 162 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = new TonyType(TYPE_bool, nullptr);}
#line 1603 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 163 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = new TonyType(TYPE_array, (yyvsp[-2].type));}
#line 1609 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 164 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = new TonyType(TYPE_list, (yyvsp[-1].type));}
#line 1615 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 168 "parser.y" /* yacc.c:1646  */
    {(yyval.stmt) = (yyvsp[0].simple);}
#line 1621 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 169 "parser.y" /* yacc.c:1646  */
    {(yyval.stmt) = new Exit();}
#line 1627 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 170 "parser.y" /* yacc.c:1646  */
    {(yyval.stmt) = new Return((yyvsp[0].expr));}
#line 1633 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 171 "parser.y" /* yacc.c:1646  */
    {(yyval.stmt)=(yyvsp[0].iff);}
#line 1639 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 172 "parser.y" /* yacc.c:1646  */
    {(yyval.stmt)=(yyvsp[0].fo);}
#line 1645 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].stmtbody)->append((yyvsp[-1].stmt)); (yyvsp[0].stmtbody)->reverse(); (yyval.stmtbody) = (yyvsp[0].stmtbody);}
#line 1651 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 180 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].stmtbody)->append((yyvsp[-1].stmt)); (yyval.stmtbody) = (yyvsp[0].stmtbody);}
#line 1657 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 181 "parser.y" /* yacc.c:1646  */
    {(yyval.stmtbody) = new StmtBody();}
#line 1663 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 185 "parser.y" /* yacc.c:1646  */
    {(yyval.iff) = new If((yyvsp[-5].expr), (yyvsp[-3].stmtbody), (yyvsp[-2].elsif), (yyvsp[-1].els));}
#line 1669 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 189 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].elsif)->append((yyvsp[-3].expr), (yyvsp[-1].stmtbody)); (yyval.elsif) = (yyvsp[0].elsif);}
#line 1675 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 190 "parser.y" /* yacc.c:1646  */
    {(yyval.elsif) = new Elsif();}
#line 1681 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 194 "parser.y" /* yacc.c:1646  */
    {(yyval.els) = new Else((yyvsp[0].stmtbody));}
#line 1687 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 195 "parser.y" /* yacc.c:1646  */
    {(yyval.els) = new Else();}
#line 1693 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 199 "parser.y" /* yacc.c:1646  */
    {(yyval.fo) = new For((yyvsp[-7].simplelist), (yyvsp[-5].expr), (yyvsp[-3].simplelist), (yyvsp[-1].stmtbody));}
#line 1699 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 203 "parser.y" /* yacc.c:1646  */
    {(yyval.simple) = new Skip();}
#line 1705 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 204 "parser.y" /* yacc.c:1646  */
    {(yyval.simple) = new Assign((yyvsp[-2].atom), (yyvsp[0].expr));}
#line 1711 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 205 "parser.y" /* yacc.c:1646  */
    {(yyval.simple) = (yyvsp[0].call);}
#line 1717 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 209 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].simplelist)->append((yyvsp[-1].simple)); (yyvsp[0].simplelist)->reverse(); (yyval.simplelist) = (yyvsp[0].simplelist);}
#line 1723 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 216 "parser.y" /* yacc.c:1646  */
    {(yyval.simplelist) = new SimpleList();}
#line 1729 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 217 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].simplelist)->append((yyvsp[-1].simple)); (yyval.simplelist) = (yyvsp[0].simplelist);}
#line 1735 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 221 "parser.y" /* yacc.c:1646  */
    {(yyval.call) = new FunctionCall(new Id(std::string((yyvsp[-2].name))));}
#line 1741 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 222 "parser.y" /* yacc.c:1646  */
    {(yyval.call) = new FunctionCall(new Id(std::string((yyvsp[-3].name))), (yyvsp[-1].exprlist));}
#line 1747 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 226 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].exprlist)->append((yyvsp[-1].expr)); (yyvsp[0].exprlist)->reverse(); (yyval.exprlist) = (yyvsp[0].exprlist);}
#line 1753 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 230 "parser.y" /* yacc.c:1646  */
    {(yyval.exprlist) = new ExprList();}
#line 1759 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 231 "parser.y" /* yacc.c:1646  */
    {(yyvsp[0].exprlist)->append((yyvsp[-1].expr)); (yyval.exprlist) = (yyvsp[0].exprlist);}
#line 1765 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 235 "parser.y" /* yacc.c:1646  */
    {(yyval.atom) = new Id(std::string((yyvsp[0].name)));}
#line 1771 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 236 "parser.y" /* yacc.c:1646  */
    {(yyval.atom) = new StringLiteral(std::string((yyvsp[0].name)));}
#line 1777 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 237 "parser.y" /* yacc.c:1646  */
    {(yyval.atom) = new ArrayElement((yyvsp[-3].atom), (yyvsp[-1].expr));}
#line 1783 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 238 "parser.y" /* yacc.c:1646  */
    {(yyval.atom) = (yyvsp[0].call);}
#line 1789 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 242 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = (yyvsp[0].atom);}
#line 1795 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 243 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new IntConst((yyvsp[0].num));}
#line 1801 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new CharConst((yyvsp[0].c));}
#line 1807 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = (yyvsp[-1].expr);}
#line 1813 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 246 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("+"), (yyvsp[0].expr));}
#line 1819 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 247 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("-"), (yyvsp[0].expr));}
#line 1825 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 248 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("nil?"), (yyvsp[-1].expr));}
#line 1831 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 249 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("head"), (yyvsp[-1].expr));}
#line 1837 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 250 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("tail"), (yyvsp[-1].expr));}
#line 1843 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 251 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new UnOp(std::string("not"), (yyvsp[0].expr));}
#line 1849 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("+"), (yyvsp[0].expr));}
#line 1855 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 253 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("-"), (yyvsp[0].expr));}
#line 1861 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 254 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("*"), (yyvsp[0].expr));}
#line 1867 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 255 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("/"), (yyvsp[0].expr));}
#line 1873 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 256 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("mod"), (yyvsp[0].expr));}
#line 1879 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 257 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("="), (yyvsp[0].expr));}
#line 1885 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 258 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("<>"), (yyvsp[0].expr));}
#line 1891 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 259 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("<"), (yyvsp[0].expr));}
#line 1897 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 260 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string(">"), (yyvsp[0].expr));}
#line 1903 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 261 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("<="), (yyvsp[0].expr));}
#line 1909 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 262 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string(">="), (yyvsp[0].expr));}
#line 1915 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 263 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("and"), (yyvsp[0].expr));}
#line 1921 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 264 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("or"), (yyvsp[0].expr));}
#line 1927 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 265 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new BinOp((yyvsp[-2].expr), std::string("#"), (yyvsp[0].expr));}
#line 1933 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 266 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new Boolean(std::string("true"));}
#line 1939 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 267 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new Boolean(std::string("false"));}
#line 1945 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 268 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new New((yyvsp[-3].type), (yyvsp[-1].expr));}
#line 1951 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 269 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new Nil();}
#line 1957 "parser.cpp" /* yacc.c:1646  */
    break;


#line 1961 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 272 "parser.y" /* yacc.c:1906  */


int main(int argc, char **argv){
    if(argc < 2){
        yyerror("No input file provided");
    }
    FILE *pt = fopen(argv[1], "r" );
    if(pt==nullptr){
        yyerror("Input file couldnn't be opened");
    }
    yyin = pt;
    yyparse();    
    fclose(pt);
}
