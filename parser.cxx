#include "parser.hxx"

//GET TYPES

std::shared_ptr<SubrangeType> Parser::get_subrange(Int lower,Int upper){
  std::string name = name_subrange(lower,upper);
  for(auto &i: infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<SubrangeType>(i.types[name]);
    }
  }
  std::shared_ptr<SubrangeType> p = std::make_shared<SubrangeType>();
  p->name = std::move(name);
  p->boundsType = INT_TYPE;
  p->ibounds[0] = lower;
  p->ibounds[1] = upper;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<SubrangeType> Parser::get_subrange(char lower,char upper){
  std::string name = name_subrange(lower,upper);
  for(auto &i: infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<SubrangeType>(i.types[name]);
    }
  }
  std::shared_ptr<SubrangeType> p = std::make_shared<SubrangeType>();
  p->name = std::move(name);
  p->boundsType = CHAR_TYPE;
  p->cbounds[0] = lower;
  p->cbounds[1] = upper;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<EnumType> Parser::get_enum(std::vector<std::string> v){
  std::string name = "_enum";
  for(auto& s: v){
    name += "_" + s + "_";
  }
  for(auto &i: infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<EnumType>(i.types[name]);
    }
  }
  std::shared_ptr<EnumType> p = std::make_shared<EnumType>();
  p->name = std::move(name);
  size_t i = 0;
  for(auto& s:v){
    p->ids[s] = i++;
  }
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<ArrayType> Parser::get_array(
  std::vector<std::shared_ptr<Type>> indexTypes,
  std::shared_ptr<Type> valueType
  ){
    std::string name = "_arr_";
    for(auto& t:indexTypes){
      name += t->name;
    }
    name += valueType->name;
    for(auto &i: infos){
      if(i.types.contains(name)){
        return std::dynamic_pointer_cast<ArrayType>(i.types[name]);
      }
    }
    std::shared_ptr<ArrayType> p = std::make_shared<ArrayType>();
    p->name = std::move(name);
    for(auto& t:indexTypes){
      p->indexTypes.emplace_back(t);
    }
    p->valueType = valueType;
    infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
    return p;
  }

///GET ELEMENTS

Label& Parser::get_label(Int id){
  std::unordered_map<Int, Label>& labels = infos[0].labels;
  if(labels.contains(id)){
    return labels[id];
  }
  println("No label with id (",id,") exists !");
  exit(EXIT_FAILURE);
}

void Parser::quit_if_label_id_used(Int id){
  if(infos[infos.size()-1].labels.contains(id)){
    println("The label id (", id, ") is already taken !");
    exit(EXIT_FAILURE);
  }
}

void Parser::quit_if_id_is_used(const std::string& id){
  for(auto& [_,t] : infos[0].types){
    if(t->type == ENUM_TYPE && std::dynamic_pointer_cast<EnumType>(t)->ids.contains(id)){
      return;
    }
  }
  if(
    infos[0].constants.contains(id) ||
    infos[0].functions.contains(id) ||
    infos[0].types.contains(id) ||
    infos[0].variables.contains(id)
  ){
    println("The id (",id,") is already taken !");
    exit(EXIT_FAILURE);
  }
}

std::shared_ptr<Const> Parser::get_constant(const std::string& id){
  for(auto& i: infos){
    if(i.constants.contains(id)){
      return i.constants[id];
    }
  }
  return std::shared_ptr<Const>();
}

std::shared_ptr<Type> Parser::get_type(const std::string& id){
  for(auto& i: infos){
    if(i.types.contains(id)){
      return i.types[id];
    }
  }
  return std::shared_ptr<Type>();
}

// Helper Methods for parsing

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

//Parser Rule

void Parser::program(){
  match_adv(PROGRAM_TOKEN);
  match_adv(ID_TOKEN);
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
  Info info;
  info.types["int"] = std::make_shared<Type>(INT_TYPE);
  info.types["uint"] = std::make_shared<Type>(UINT_TYPE);
  info.types["real"] = std::make_shared<Type>(REAL_TYPE);
  info.types["boolean"] = std::make_shared<Type>(BOOLEAN_TYPE);
  info.types["char"] = std::make_shared<Type>(CHAR_TYPE);
  infos.emplace_front(std::move(info));
  block();
  println("Labels :");
  for(auto& i:infos[0].labels){
    println(i.first);
  }
  println("Constants :");
  for(auto& i:infos[0].constants){
    print(i.first," = ");
    std::visit([](auto e){std::cout << e << '\n';}, i.second.get()->value);
  }
  println("Types :");
  for(auto& [s,t]:infos[0].types){
    print("Type (",s,") : ");
    if(t->type == ENUM_TYPE){
      auto e = std::dynamic_pointer_cast<EnumType>(t);
      print("( ");
      for(auto &[id,val] : e->ids){
        print(id,"(",val,") ");
      }
      print(")");
    }else if(t->type == SUBRANGE_TYPE){
      auto sub = std::dynamic_pointer_cast<SubrangeType>(t);
      if(sub->boundsType == INT_TYPE){
        print("From ",sub->ibounds[0]," to ",sub->ibounds[1]);
      }else{
        print("From ", sub->cbounds[0], " to ", sub->cbounds[1]);
      }
    }
    println();
  }
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
  std::unordered_map<Int, Label>& labels = infos[0].labels;
  match(LABEL_TOKEN);
  match_adv(NUM_INT_TOKEN);
  Int id = lexer.getToken().ival;
  quit_if_label_id_used(id);
  Label l;
  l.loc = -1;
  labels[id] = l;
  while(lexer.next_sym().type == COMMA_TOKEN){
    match_adv(NUM_INT_TOKEN);
    id = lexer.getToken().ival;
    quit_if_label_id_used(id);
    Label l;
    l.loc = -1;
    labels[id] = l;
  }
  match(SEMI_TOKEN);
  lexer.next_sym();
}

void Parser::constant_definition_part(){
  match(CONST_TOKEN);
  lexer.next_sym();
  match(ID_TOKEN);
  do{
    std::string id = lexer.getToken().id;
    quit_if_id_is_used(id);
    match_adv(EQ_TOKEN);
    lexer.next_sym();
    infos[0].constants[id] = std::make_shared<Const>(constant());
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
}

Const Parser::constant(){
  Lexeme& t = lexer.getToken();
  bool neg = (t.type == MINUS_TOKEN);
  bool pos = (t.type == PLUS_TOKEN);
  if(neg || pos){
    matches_adv({ID_TOKEN,NUM_INT_TOKEN,NUM_REAL_TOKEN});
  }else{
    matches({ID_TOKEN,NUM_INT_TOKEN,NUM_REAL_TOKEN,STRING_LITERAL_TOKEN});
  }
  Const c;
  if(t.type == ID_TOKEN){
    auto tmp = get_constant(t.id);
    if(!tmp){
      println("No constant with id (",t.id,") exists !");
    }
    c = *tmp;
  }else{
    switch(t.type){
      case NUM_INT_TOKEN:
        c.type = INT_TYPE;
        c.value = t.ival;
        break;
      case NUM_REAL_TOKEN:
        c.type = REAL_TYPE;
        c.value = t.dval;
        break;
      case STRING_LITERAL_TOKEN:
        c.type = CONST_STR_TYPE;
        c.value = t.id;
        break;
    }
  }
  if(c.type == CONST_STR_TYPE && (pos || neg)){
    println("Cannot use '+' or '-' with a string !");
    exit(EXIT_FAILURE);
  }
  if(neg){
    if(c.type == INT_TYPE){
      c.value = -std::get<Int>(c.value);
    }else{
      c.value = -std::get<double>(c.value);
    }
  }
  lexer.next_sym();
  return c;
}

void Parser::type_definition_part(){
  match(TYPE_TOKEN);
  match_adv(ID_TOKEN);
  do{
    std::string name = lexer.getToken().id;
    match_adv(EQ_TOKEN);
    lexer.next_sym();
    type();
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
}

std::shared_ptr<Type> Parser::type(){
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
      return simple_type();
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
    case ID_TOKEN: 
      {
        auto tmp1 = get_constant(lexer.getToken().id);
        if(tmp1){
          return simple_type();
        }
        auto tmp2 = get_type(lexer.getToken().id);
        if(tmp2){
          lexer.next_sym();
          return tmp2;
        }
      }
      break;
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
  return std::make_shared<Type>(VOID_TYPE);
}

std::shared_ptr<Type> Parser::simple_type(){
  if(lexer.getToken().type == LP_TOKEN){
    return std::dynamic_pointer_cast<Type>(enumerated_type());
  }else{
    return std::dynamic_pointer_cast<Type>(subrange_type());
  }
}

std::shared_ptr<EnumType> Parser::enumerated_type(){
  match(LP_TOKEN);
  lexer.next_sym();
  std::vector<std::string> ids = id_list();
  match(RP_TOKEN);
  lexer.next_sym();
  return get_enum(std::move(ids));
}

std::vector<std::string> Parser::id_list(){
  std::vector<std::string> v;
  match(ID_TOKEN);
  v.push_back(lexer.getToken().id);
  while(lexer.next_sym().type == COMMA_TOKEN){
    match_adv(ID_TOKEN);
    v.push_back(lexer.getToken().id);
  }
  return v;
}

std::shared_ptr<SubrangeType> Parser::subrange_type(){
  Const lower_bound = constant();
  match(DOTDOT_TOKEN);
  lexer.next_sym();
  Const upper_bound = constant();
  if(lower_bound.type != upper_bound.type){
    println("a subrange is made of 2 constants of the same type !");
    exit(EXIT_FAILURE);
  }
  if(lower_bound.type == INT_TYPE){
    return get_subrange(get<Int>(lower_bound.value),get<Int>(upper_bound.value));
  }
  if(lower_bound.type == CONST_STR_TYPE){
    if(get<std::string>(lower_bound.value).length() != 1 || get<std::string>(upper_bound.value).length() != 1){
      println("a subrange is made of 2 chars not strings !");
      exit(EXIT_FAILURE);
    }
    return get_subrange(get<std::string>(lower_bound.value)[0],get<std::string>(upper_bound.value)[0]);
  }
  println("Cannot create a subrange of something other than chars or ints !");
  exit(EXIT_FAILURE);
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

void Parser::expression(){
  simple_expression();
  switch(lexer.getToken().type){
    case EQ_TOKEN:
    case NEQ_TOKEN:
    case LT_TOKEN:
    case LE_TOKEN:
    case GT_TOKEN:
    case GE_TOKEN:
    case IN_TOKEN:
      lexer.next_sym();
      simple_expression();
      break;
    default: return;
  }
}

void Parser::simple_expression(){
  switch(lexer.getToken().type){
    case PLUS_TOKEN:
    case MINUS_TOKEN:
      lexer.next_sym();
      break;
    default: break;
  }
  term();
  bool in = true;
  while(in){
    switch(lexer.getToken().type){
      case PLUS_TOKEN:
      case MINUS_TOKEN:
      case OR_TOKEN:
        lexer.next_sym();
        term();
        break;
      default: in = false; break;
    }
  }
}

void Parser::term(){
  factor();
  bool in = true;
  while(in){
    switch(lexer.getToken().type){
      case STAR_TOKEN:
      case SLASH_TOKEN:
      case DIV_TOKEN:
      case MOD_TOKEN:
      case AND_TOKEN:
        lexer.next_sym();
        factor();
        break;
      default: in = false; break;
    }
  }
}

void Parser::factor(){
}


void Parser::statement_part(){
  lexer.next_sym();
}