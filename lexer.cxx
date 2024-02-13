#include "lexer.hxx"

Lexer::Lexer(std::string filename)
{
  file = fopen(filename.c_str(), "r");
  if (!file)
  {
    std::cout << "Unable to open file " << filename << " !\n";
    exit(EXIT_FAILURE);
  }
  keywords["and"] = AND_TOKEN;
  keywords["array"] = ARRAY_TOKEN;
  keywords["begin"] = BEGIN_TOKEN;
  keywords["case"] = CASE_TOKEN;
  keywords["const"] = CONST_TOKEN;
  keywords["div"] = DIV_TOKEN;
  keywords["do"] = DO_TOKEN;
  keywords["downto"] = DOWNTO_TOKEN;
  keywords["else"] = ELSE_TOKEN;
  keywords["end"] = END_TOKEN;
  keywords["file"] = FILE_TOKEN;
  keywords["for"] = FOR_TOKEN;
  keywords["function"] = FUNCTION_TOKEN;
  keywords["goto"] = GOTO_TOKEN;
  keywords["if"] = IF_TOKEN;
  keywords["in"] = IN_TOKEN;
  keywords["label"] = LABEL_TOKEN;
  keywords["mod"] = MOD_TOKEN;
  keywords["nil"] = NIL_TOKEN;
  keywords["not"] = NOT_TOKEN;
  keywords["of"] = OF_TOKEN;
  keywords["or"] = OR_TOKEN;
  keywords["packed"] = PACKED_TOKEN;
  keywords["procedure"] = PROCEDURE_TOKEN;
  keywords["program"] = PROGRAM_TOKEN;
  keywords["record"] = RECORD_TOKEN;
  keywords["repeat"] = REPEAT_TOKEN;
  keywords["read"] = READ_TOKEN;
  keywords["write"] = WRITE_TOKEN;
  keywords["set"] = SET_TOKEN;
  keywords["then"] = THEN_TOKEN;
  keywords["to"] = TO_TOKEN;
  keywords["type"] = TYPE_TOKEN;
  keywords["until"] = UNTIL_TOKEN;
  keywords["var"] = VAR_TOKEN;
  keywords["while"] = WHILE_TOKEN;
  keywords["with"] = WITH_TOKEN;
  keywords["true"] = TRUE_TOKEN;
  keywords["false"] = FALSE_TOKEN;
  read_char();
}

Lexer::Lexer(const Lexer &l){
  current_char = l.current_char;
  file = l.file;
  token = l.token;
  keywords = l.keywords;
}

Lexer::Lexer(Lexer &&l){
  current_char = l.current_char;
  file = l.file;
  token = std::move(l.token);
  keywords = std::move(l.keywords);
}

Lexeme& Lexer::getToken(){
  return token;
}

char Lexer::getChar(){
  return current_char;
}

Lexer::~Lexer(){
  if(file){
    fclose(file);
    file = nullptr;
  }
}

void Lexer::lexer_error(LEXER_ERROR le){
  printf("Lexer Error %lu:%lu (%d) : %s !",token.line,token.col,le,lexer_error_msg[le].c_str());
  exit(-1);
}

void Lexer::read_char(){
  current_char = fgetc(file);
  if(current_char == '\n'){
    ++token.line;
    token.col = 0;
  }else{
    ++token.col;
  }
}

void show_token(Lexeme* l){
  std::cout << token_type_name[l->type];
}

void Lexer::read_number(){
  do{
    token.id += current_char;
    read_char();
  }while(isdigit(current_char));
  if(current_char != '.' && current_char != 'e' && current_char != 'E'){
    if (token.id.length() > id_max_len){
      lexer_error(LE_LONG_NUMBER);
    }
    token.ival = atoll(token.id.c_str());
    token.type = NUM_INT_TOKEN;
    return;
  }
  if(current_char == '.'){
    token.id += '.';
    read_char();
    if(!isdigit(current_char)){
      lexer_error(LE_INVALID_NUMBER);
    }
    do{
      token.id += current_char;
      read_char();
    }while(isdigit(current_char));
  }
  if(current_char == 'e' || current_char == 'E'){
    token.id += current_char;
    read_char();
    if(current_char == '+' || current_char == '-'){
      token.id += current_char;
      read_char();
    }
    if(!isdigit(current_char)){
      lexer_error(LE_INVALID_NUMBER);
    }
    do{
      token.id += current_char;
      read_char();
    }while(isdigit(current_char));
  }
  if(token.id.length() > id_max_len){
    lexer_error(LE_LONG_NUMBER);
  }
  token.dval = atof(token.id.c_str());
  token.type = NUM_REAL_TOKEN;
}

void Lexer::read_word(){
  const char diff = 'a' - 'A';
  do{
    token.id += (current_char <= 'Z' ? current_char + diff : current_char);
    read_char();
  }while(isalnum(current_char));
  if(token.id.length() > id_max_len){
    lexer_error(LE_LONG_ID);
  }
  if(keywords.contains(token.id)){
    token.type = keywords[token.id];
    return;
  }
  token.type = ID_TOKEN;
}

void Lexer::read_string(){
  const char q = current_char;
  read_char();
  while(current_char != EOF){
    if(current_char == q){
      read_char();
      if(current_char == q){
        token.id += q;
      }else{
        break;
      }
    }else{
      token.id += current_char;
    }
    read_char();
  }
  token.type = STRING_LITERAL_TOKEN;
}

Lexeme& Lexer::next_sym(){
  token.id = "";
  while(isspace(current_char)){
    read_char();
  }
  if(current_char == EOF){
    token.type = EOF_TOKEN;
    token.id = token_type_name[token.type];
    return token;
  }
  if(current_char == '{'){
    read_char();
    if(current_char == '*'){
      read_char();
      while(current_char != EOF){
        if(current_char == '*'){
          read_char();
          if(current_char == '}'){
            read_char();
            return next_sym();
          }
        }
        read_char();
      }
    }
    token.type = LC_TOKEN;
    return token;
  }
  if(isalpha(current_char)){
    read_word();
    return token;
  }if(isdigit(current_char)){
    read_number();
    return token;
  }if(current_char == '\''){
    read_string();
    return token;
  }else{
    token.id = current_char;
    switch(current_char){
      case '+':
      token.type = PLUS_TOKEN;
      break;
      case '-':
      token.type = MINUS_TOKEN;
      break;
      case '*':
      token.type = STAR_TOKEN;
      break;
      case '/':
      token.type = SLASH_TOKEN;
      break;
      case ':':
        read_char();
        if(current_char != '='){
          token.type = COLON_TOKEN;
          return token;
        }
        token.id += '=';
        token.type = ASSIGN_TOKEN;
        break;
      case ',':
      token.type = COMMA_TOKEN;
      break;
      case ';':
      token.type = SEMI_TOKEN;
      break;
      case '=':
      token.type = EQ_TOKEN;
      break;
      case '<':
        read_char();
        if(current_char == '>'){
          token.id += current_char;
          token.type = NEQ_TOKEN;
          return token;
        }
        if(current_char == '='){
          token.id += current_char;
          token.type = LE_TOKEN;
          return token;
        }
        token.type = LT_TOKEN;
        return token;
      case '>':
        read_char();
        if(current_char != '='){
          token.type = GT_TOKEN;
          return token;
        }
        token.id += current_char;
        token.type = GE_TOKEN;
        break;
      case '(':
      token.type = LP_TOKEN;
      break;
      case ')':
      token.type = RP_TOKEN;
      break;
      case '[':
      token.type = LB_TOKEN;
      break;
      case ']':
      token.type = RB_TOKEN;
      break;
      case '}':
      token.type = RC_TOKEN;
      break;
      case '^':
      token.type = POINTER_TOKEN;
      break;
      case '.':
        read_char();
        if(current_char != '.'){
          token.type = DOT_TOKEN;
          return token;
        }
        token.id += current_char;
        token.type = DOTDOT_TOKEN;
        break;
      default:
        token.type = ERROR_TOKEN;
        lexer_error(LE_INVALID_CHAR);
    }
    read_char();
    return token;
  }
}