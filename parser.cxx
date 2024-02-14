#include "parser.hxx"

void Parser::match_adv(TOKEN_TYPE type){
  lexer.next_sym();
  if (type != lexer.getToken().type){
    println("Error at ",lexer.getToken().line,':',lexer.getToken().col);
    printf("Expected %s ! Found %s !\n", token_type_name[type].c_str(), token_type_name[lexer.getToken().type].c_str());
    exit(EXIT_FAILURE);
  }
}

void Parser::match(TOKEN_TYPE type){
  if (type != lexer.getToken().type){
    println("Error at ", lexer.getToken().line, ':', lexer.getToken().col);
    printf("Expected %s ! Found %s !\n", token_type_name[type].c_str(), token_type_name[lexer.getToken().type].c_str());
    exit(EXIT_FAILURE);
  }
}

void Parser::matches(const std::initializer_list<TOKEN_TYPE> &l)
{
  for(auto& t: l){
    if(lexer.getToken().type == t){
      return;
    }
  }
  println("Error at ", lexer.getToken().line, ':', lexer.getToken().col);
  std::string msg = "Expected ";
  for(auto& t: l){
    msg += token_type_name[t] + ' ';
  }
  msg += "! Found " + token_type_name[lexer.getToken().type] + " !";
  println(msg);
  exit(EXIT_FAILURE);
}

void Parser::matches_adv(const std::initializer_list<TOKEN_TYPE> &l)
{
  lexer.next_sym();
  for(auto& t: l){
    if(lexer.getToken().type == t){
      return;
    }
  }
  println("Error at ", lexer.getToken().line, ':', lexer.getToken().col);
  std::string msg = "Expected ";
  for(auto& t: l){
    msg += token_type_name[t] + ' ';
  }
  msg += "! Found " + token_type_name[lexer.getToken().type] + " !";
  println(msg);
  exit(EXIT_FAILURE);
}

Parser::Parser(Lexer l) : lexer(std::move(l)){
  program_name = "";
}

Lexer& Parser::getLexer(){
  return lexer;
}

void Parser::parse(){
  program();
}

void Parser::program(){
  match_adv(PROGRAM_TOKEN);
  match_adv(ID_TOKEN);
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
  block();
  //match(DOT_TOKEN);
}

void Parser::block(){
  declaration_part();
  statement_part();
}

void Parser::declaration_part(){
  bool in = true;
  while(in){
    switch(lexer.getToken().type){
      case LABEL_TOKEN: label_declaration_part(); break;
      case CONST_TOKEN: constant_definition_part(); break;
      case TYPE_TOKEN: type_definition_part(); break;
      case VAR_TOKEN: variable_declaration_part(); break;
      case PROCEDURE_TOKEN: procedure_declaration(); break;
      case FUNCTION_TOKEN: function_declaration(); break;
      default: in = false;
    }
  }
}

void Parser::label_declaration_part(){
  match(LABEL_TOKEN);
  match_adv(NUM_INT_TOKEN);
  while(lexer.next_sym().type == COMMA_TOKEN){
    match_adv(NUM_INT_TOKEN);
  }
  match(SEMI_TOKEN);
  lexer.next_sym();
}

void Parser::constant_definition_part(){
  match(CONST_TOKEN);
  lexer.next_sym();
  match(ID_TOKEN);
  do{
    match_adv(EQ_TOKEN);
    lexer.next_sym();
    constant();
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
}

void Parser::constant(){
  Lexeme& t = lexer.getToken();
  bool neg = (t.type == MINUS_TOKEN);
  if(neg || t.type == PLUS_TOKEN){
    matches_adv({ID_TOKEN,NUM_INT_TOKEN,NUM_REAL_TOKEN});
  }else{
    matches({ID_TOKEN,NUM_INT_TOKEN,NUM_REAL_TOKEN,STRING_LITERAL_TOKEN});
  }
  lexer.next_sym();
}

void Parser::type_definition_part(){
  match(TYPE_TOKEN);
  match_adv(ID_TOKEN);
  do{
    match_adv(EQ_TOKEN);
    lexer.next_sym();
    type();
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
}

void Parser::type(){
  //TODO : distinguish const id and type name
  switch(lexer.getToken().type){
    case PLUS_TOKEN:
    case MINUS_TOKEN:
    // const id
    //case ID_TOKEN:
    case NUM_INT_TOKEN:
    case NUM_REAL_TOKEN:
    case STRING_LITERAL_TOKEN:
    case LP_TOKEN:
      simple_type();
      break;
    case PACKED_TOKEN:
    case ARRAY_TOKEN:
    case RECORD_TOKEN:
    case SET_TOKEN:
    case FILE_TOKEN:
      structured_type();
      break;
    case POINTER_TOKEN: pointer_type();  break;
    case PROCEDURE_TOKEN: procedure_type(); break;
    case FUNCTION_TOKEN: function_type(); break;
    // type name
    case ID_TOKEN: lexer.next_sym();  break;
    default:
      matches({
        PLUS_TOKEN,
        MINUS_TOKEN,
        ID_TOKEN,
        NUM_INT_TOKEN,
        NUM_REAL_TOKEN,
        STRING_LITERAL_TOKEN,
        LP_TOKEN,
        PACKED_TOKEN,
        ARRAY_TOKEN,
        RECORD_TOKEN,
        SET_TOKEN,
        FILE_TOKEN,
        POINTER_TOKEN,
        PROCEDURE_TOKEN,
        FUNCTION_TOKEN
      });
  }
}

void Parser::simple_type(){
  if(lexer.getToken().type == LP_TOKEN){
    enumerated_type();
  }else{
    subrange_type();
  }
}

void Parser::enumerated_type(){
  match(LP_TOKEN);
  lexer.next_sym();
  id_list();
  match(RP_TOKEN);
  lexer.next_sym();
}

void Parser::id_list(){
  match(ID_TOKEN);
  while(lexer.next_sym().type == COMMA_TOKEN){
    match_adv(ID_TOKEN);
  }
}

void Parser::subrange_type(){
  constant();
  match(DOTDOT_TOKEN);
  lexer.next_sym();
  constant();
}

void Parser::structured_type(){
  if(lexer.getToken().type == PACKED_TOKEN){
    lexer.next_sym();
  }
  unpacked_structured_type();
}

void Parser::unpacked_structured_type(){
  // TODO : shouldn't add safeguards (default case) ?
  switch(lexer.getToken().type){
    case ARRAY_TOKEN: array_type(); break;
    case RECORD_TOKEN: record_type(); break;
    case SET_TOKEN: set_type(); break;
    case FILE_TOKEN: file_type(); break;
    default:
      matches({
        ARRAY_TOKEN,
        RECORD_TOKEN,
        SET_TOKEN,
        FILE_TOKEN
      });
  }
}

void Parser::array_type(){
  match(ARRAY_TOKEN);
  match_adv(LB_TOKEN);
  do{
    lexer.next_sym();
    simple_type();
  }while(lexer.getToken().type == COMMA_TOKEN);
  match(RB_TOKEN);
  match_adv(OF_TOKEN);
  lexer.next_sym();
  type();
}

void Parser::record_type(){
  match(RECORD_TOKEN);
  lexer.next_sym();
  field_list();
  match(END_TOKEN);
  lexer.next_sym();
}

void Parser::field_list(){
  switch(lexer.getToken().type){
    case ID_TOKEN:
      fixed_part();
      if(lexer.getToken().type == SEMI_TOKEN){
        lexer.next_sym();
        variant_part();
      }else if(lexer.getToken().type == CASE_TOKEN){
        variant_part();
      }
      break;
    case CASE_TOKEN: variant_part(); break;
    default:
      matches({ID_TOKEN,CASE_TOKEN});
  }
}

void Parser::fixed_part(){
  id_list();
  match(COLON_TOKEN);
  lexer.next_sym();
  type();
  while(lexer.getToken().type == SEMI_TOKEN && lexer.next_sym().type != CASE_TOKEN){
    id_list();
    match(COLON_TOKEN);
    lexer.next_sym();
    type();
  }
}

void Parser::variant_part(){
  //TODO : Differentiate between var id and type name 
  match(CASE_TOKEN);
  if(lexer.next_sym().type == ID_TOKEN){
    match_adv(COLON_TOKEN);
  }
  match_adv(ID_TOKEN);
  match_adv(OF_TOKEN);
  do{
    lexer.next_sym();
    variant();
  }while(lexer.getToken().type == SEMI_TOKEN);
}

void Parser::variant(){
  case_label_list();
  match(COLON_TOKEN);
  match_adv(LP_TOKEN);
  lexer.next_sym();
  field_list();
  match(RP_TOKEN);
  lexer.next_sym();
}

void Parser::case_label_list(){
  constant();
  while(lexer.getToken().type == COMMA_TOKEN){
    lexer.next_sym();
    constant();
  }
}

void Parser::set_type(){
  match(SET_TOKEN);
  match_adv(OF_TOKEN);
  lexer.next_sym();
  type();
}

void Parser::file_type(){
  match(FILE_TOKEN);
  if(lexer.next_sym().type == OF_TOKEN){
    lexer.next_sym();
    type();
  }
}

void Parser::pointer_type(){
  match(POINTER_TOKEN);
  match_adv(ID_TOKEN);
  lexer.next_sym();
}

void Parser::variable_declaration_part(){
  match(VAR_TOKEN);
  lexer.next_sym();
  do{
    id_list();
    match(COLON_TOKEN);
    lexer.next_sym();
    type();
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
  
}

void Parser::procedure_type(){
  match(PROCEDURE_TOKEN);
  lexer.next_sym();
  if(lexer.getToken().type == LP_TOKEN){
    formal_parameter_list();
  }
}

void Parser::formal_parameter_list(){
  match(LP_TOKEN);
  do{
    lexer.next_sym();
    formal_parameter_section();
  }while(lexer.getToken().type == SEMI_TOKEN);
  match(RP_TOKEN);
  lexer.next_sym();
}

void Parser::formal_parameter_section(){
  if(lexer.getToken().type == VAR_TOKEN){
    lexer.next_sym();
  }
  id_list();
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  lexer.next_sym();
}

void Parser::function_type(){
  match(FUNCTION_TOKEN);
  lexer.next_sym();
  if(lexer.getToken().type == LP_TOKEN){
    formal_parameter_list();
  }
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  lexer.next_sym();
}

void Parser::procedure_declaration(){
  match(PROCEDURE_TOKEN);
  match_adv(ID_TOKEN);
  if(lexer.next_sym().type == LP_TOKEN){
    formal_parameter_list();
  }
  match(SEMI_TOKEN);
  lexer.next_sym();
  block();
}

void Parser::function_declaration(){
  match(FUNCTION_TOKEN);
  match_adv(ID_TOKEN);
  if(lexer.next_sym().type == LP_TOKEN){
    formal_parameter_list();
  }
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
  block();
}


void Parser::statement_part(){
  lexer.next_sym();
}