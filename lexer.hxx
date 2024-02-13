#ifndef LEXER_HXX
#define LEXER_HXX
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <concepts>

#define id_max_len 63


template <typename T>
concept printable = requires(T t){
  std::cout << t;
};

template <printable... Args>
void print(Args &&...args){
  (std::cout << ... << args);
}

template <typename... Args>
void println(Args &&...args)
{
  (std::cout << ... << args) << '\n';
}

typedef long long Int;
typedef long long UInt;

enum LEXER_ERROR{
  LE_INVALID_CHAR,LE_INVALID_NUMBER,LE_LONG_NUMBER,LE_LONG_ID,LE_LONG_STR
};


enum TOKEN_TYPE{
  AND_TOKEN,
  ARRAY_TOKEN,
  BEGIN_TOKEN,
  CASE_TOKEN,
  CHR_TOKEN,
  CONST_TOKEN,
  DIV_TOKEN,
  DO_TOKEN,
  DOWNTO_TOKEN,
  ELSE_TOKEN,
  END_TOKEN,
  FILE_TOKEN,
  FOR_TOKEN,
  FUNCTION_TOKEN,
  GOTO_TOKEN,
  IF_TOKEN,
  IN_TOKEN,
  LABEL_TOKEN,
  MOD_TOKEN,
  NIL_TOKEN,
  NOT_TOKEN,
  OF_TOKEN,
  OR_TOKEN,
  PACKED_TOKEN,
  PROCEDURE_TOKEN,
  PROGRAM_TOKEN,
  RECORD_TOKEN,
  REPEAT_TOKEN,
  SET_TOKEN,
  THEN_TOKEN,
  TO_TOKEN,
  TYPE_TOKEN,
  UNTIL_TOKEN,
  VAR_TOKEN,
  WHILE_TOKEN,
  WITH_TOKEN,
  PLUS_TOKEN,
  MINUS_TOKEN,
  STAR_TOKEN,
  SLASH_TOKEN,
  ASSIGN_TOKEN,
  COMMA_TOKEN,
  SEMI_TOKEN,
  COLON_TOKEN,
  EQ_TOKEN,
  NEQ_TOKEN,
  LT_TOKEN,
  LE_TOKEN,
  GT_TOKEN,
  GE_TOKEN,
  LP_TOKEN,
  RP_TOKEN,
  LB_TOKEN,
  RB_TOKEN,
  READ_TOKEN,
  WRITE_TOKEN,
  POINTER_TOKEN,
  AT_TOKEN,
  DOT_TOKEN,
  DOTDOT_TOKEN,
  LC_TOKEN,
  RC_TOKEN,
  TRUE_TOKEN,
  FALSE_TOKEN,
  ID_TOKEN,
  STRING_LITERAL_TOKEN,
  NUM_INT_TOKEN,
  NUM_REAL_TOKEN,
  EOF_TOKEN,
  ERROR_TOKEN
};

static const std::string token_type_name[] = {
  "AND_TOKEN",
  "ARRAY_TOKEN",
  "BEGIN_TOKEN",
  "CASE_TOKEN",
  "CHR_TOKEN",
  "CONST_TOKEN",
  "DIV_TOKEN",
  "DO_TOKEN",
  "DOWNTO_TOKEN",
  "ELSE_TOKEN",
  "END_TOKEN",
  "FILE_TOKEN",
  "FOR_TOKEN",
  "FUNCTION_TOKEN",
  "GOTO_TOKEN",
  "IF_TOKEN",
  "IN_TOKEN",
  "LABEL_TOKEN",
  "MOD_TOKEN",
  "NIL_TOKEN",
  "NOT_TOKEN",
  "OF_TOKEN",
  "OR_TOKEN",
  "PACKED_TOKEN",
  "PROCEDURE_TOKEN",
  "PROGRAM_TOKEN",
  "RECORD_TOKEN",
  "REPEAT_TOKEN",
  "SET_TOKEN",
  "THEN_TOKEN",
  "TO_TOKEN",
  "TYPE_TOKEN",
  "UNTIL_TOKEN",
  "VAR_TOKEN",
  "WHILE_TOKEN",
  "WITH_TOKEN",
  "PLUS_TOKEN",
  "MINUS_TOKEN",
  "STAR_TOKEN",
  "SLASH_TOKEN",
  "ASSIGN_TOKEN",
  "COMMA_TOKEN",
  "SEMI_TOKEN",
  "COLON_TOKEN",
  "EQ_TOKEN",
  "NEQ_TOKEN",
  "LT_TOKEN",
  "LE_TOKEN",
  "GT_TOKEN",
  "GE_TOKEN",
  "LP_TOKEN",
  "RP_TOKEN",
  "LB_TOKEN",
  "RB_TOKEN",
  "READ_TOKEN",
  "WRITE_TOKEN",
  "POINTER_TOKEN",
  "AT_TOKEN",
  "DOT_TOKEN",
  "DOTDOT_TOKEN",
  "LC_TOKEN",
  "RC_TOKEN",
  "TRUE_TOKEN",
  "FALSE_TOKEN",
  "ID_TOKEN",
  "STRING_LITERAL_TOKEN",
  "NUM_INT_TOKEN",
  "NUM_REAL_TOKEN",
  "EOF_TOKEN",
  "ERROR_TOKEN"
};

struct Lexeme{
  TOKEN_TYPE type;
  union{
    Int ival;
    double dval;
  };
  std::string id;
  size_t col;
  size_t line;
};

class Lexer{
  Lexeme token;
  FILE *file;
  int current_char;
  std::unordered_map<std::string, TOKEN_TYPE> keywords;

  void read_char();
  void read_word();
  void read_string();
  void read_number();
  void lexer_error(LEXER_ERROR le);

public:
  Lexer(std::string filename);
  Lexer(const Lexer& l);
  Lexer(Lexer &&l);

  Lexeme& next_sym();
  Lexeme& getToken();
  char getChar();

  ~Lexer();
};


static const size_t keywords_length = 41;

static const std::string lexer_error_msg[] = {
  "Invalid character",
  "Invalid number",
  "Too big of a number",
  "Too big of an ID",
  "Too big of a string"
};
void show_token(Lexeme *l);

#endif