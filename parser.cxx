#include "parser.hxx"

//GET TYPES

std::shared_ptr<SubrangeType> Parser::get_subrange(Int lower,Int upper){
  if(lower >= upper){
    println("For subrange types (l..u), we should have l < u !");
    exit(EXIT_FAILURE);
  }
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
  p->amount = upper - lower + 1;
  p->size = 1;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<SubrangeType> Parser::get_subrange(char lower,char upper){
  if(lower >= upper){
    println("For subrange types (l..u), we should have l < u !");
    exit(EXIT_FAILURE);
  }
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
  p->amount = upper - lower + 1;
  p->size = 1;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<EnumType> Parser::get_enum(std::vector<std::string>&& v){
  std::string name = "_enum";
  for (auto &s : v){
    name += "_" + s + "_";
  }
  for (auto &i : infos){
    if (i.types.contains(name)){
      return std::dynamic_pointer_cast<EnumType>(i.types[name]);
    }
  }
  std::shared_ptr<EnumType> p = std::make_shared<EnumType>();
  p->name = std::move(name);
  size_t i = 0;
  for(auto& s:v){
    if(p->ids.contains(s)){
      println("Enum with element declared twice (",s,") !");
      exit(EXIT_FAILURE);
    }
    p->ids[s] = i++;
  }
  p->amount = (uint)v.size();
  p->size = 1;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<ArrayType> Parser::get_array(
  const std::vector<std::shared_ptr<Type>>& indexTypes,
  std::shared_ptr<Type> valueType
  ){
    std::string name = "_arr_";
    for (auto &t : indexTypes){
      name += t->name;
    }
    name += valueType->name;
    for (auto &i : infos){
      if (i.types.contains(name)){
        return std::dynamic_pointer_cast<ArrayType>(i.types[name]);
      }
    }
    std::shared_ptr<ArrayType> p = std::make_shared<ArrayType>();
    p->name = std::move(name);
    p->amount = 1;
    for(auto& t:indexTypes){
      p->amount *= t->amount;
      p->indexTypes.emplace_back(t);
    }
    p->valueType = valueType;
    p->size = p->valueType->size;
    infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
    return p;
  }

std::shared_ptr<SetType> Parser::get_set(std::shared_ptr<Type> valueType){
  std::string name = "_set_" + valueType->name;
  for(auto &i : infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<SetType>(i.types[name]);
    }
  }
  std::shared_ptr<SetType> p = std::make_shared<SetType>();
  p->name = std::move(name);
  p->valueType = valueType;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<FileType> Parser::get_file(std::shared_ptr<Type> valueType){
  std::string name = "_file_" + valueType->name;
  for(auto &i : infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<FileType>(i.types[name]);
    }
  }
  std::shared_ptr<FileType> p = std::make_shared<FileType>();
  p->name = std::move(name);
  p->valueType = valueType;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<PointerType> Parser::get_pointer(std::shared_ptr<Type> valueType){
  std::string name = "_pointer_" + valueType->name;
  for(auto &i : infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<PointerType>(i.types[name]);
    }
  }
  std::shared_ptr<PointerType> p = std::make_shared<PointerType>();
  p->name = std::move(name);
  p->valueType = valueType;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<RecordType> Parser::get_record(
  std::unordered_map<std::string, std::shared_ptr<Type>> &&attributes
){
  std::string name = "_record_";
  for(auto& [_,t] : attributes){
    name += "_" + t->name + "_";
  }
  for(auto& i:infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<RecordType>(i.types[name]);
    }
  }
  std::shared_ptr<RecordType> p = std::make_shared<RecordType>();
  p->name = std::move(name);
  p->attributes = std::move(attributes);
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<FunctionType> Parser::get_function(const std::vector<Arg> &args, std::shared_ptr<Type> returnType){
  std::string name = "_function_";
  for(auto& arg : args){
    name += "_" ;
    name += (arg.byRef ? "_var_" : "");
    name += arg.type->name + "_";
  }
  for(auto& i:infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<FunctionType>(i.types[name]);
    }
  }
  std::shared_ptr<FunctionType> p = std::make_shared<FunctionType>();
  p->name = std::move(name);
  p->args = args;
  p->returnType = returnType;
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
    println(lexer.getToken().id);
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

void Parser::print_type(std::shared_ptr<Type> t,const std::string& name){
  print("Type (",name,")","(",t->name,") : ");
  switch(t->type){
    case INT_TYPE : println("Integer"); break;
    case UINT_TYPE : println("Unsigned Integer"); break;
    case REAL_TYPE : println("Real Number"); break;
    case BOOLEAN_TYPE :  println("Boolean"); break;
    case CHAR_TYPE :  println("Character"); break;
    case STRING_TYPE: println("String"); break;
    case CONST_STR_TYPE : println("Constant String (length > 1)"); break;
    case ENUM_TYPE :  {
        auto e = std::dynamic_pointer_cast<EnumType>(t);
        print("( ");
        for (auto &[id, val] : e->ids){
          print(id, "(", val, ") ");
        }
        println(")");
        println("*********************************");
    }break;
    case SUBRANGE_TYPE :  {
      auto sub = std::dynamic_pointer_cast<SubrangeType>(t);
      if(sub->boundsType == INT_TYPE){
        println("From ",sub->ibounds[0]," to ",sub->ibounds[1]);
      }else{
        println("From ", sub->cbounds[0], " to ", sub->cbounds[1]);
      }
      println("*********************************");
    }break;
    case ARRAY_TYPE : {
      println("\nIndex Types : ");
      auto arr_type = std::dynamic_pointer_cast<ArrayType>(t);
      for(auto &i : arr_type->indexTypes){
        print_type(i);
      }
      println("Value Type : ");
      print_type(arr_type->valueType);
      println("*********************************");
    }break;
    case RECORD_TYPE : {
      println("Record Of :");
      for(auto& att : std::dynamic_pointer_cast<RecordType>(t)->attributes){
        print("Name : ",att.first," ");
        print_type(att.second);
      }
    }break;  
    case SET_TYPE :  {
      println("Set Of : ");
      print_type(std::dynamic_pointer_cast<SetType>(t)->valueType);
      println("*********************************");
    }break;
    case FILE_TYPE :  {
      println("File Of : ");
      print_type(std::dynamic_pointer_cast<FileType>(t)->valueType);
      println("*********************************");
    }break;
    case POINTER_TYPE : {
      println("Pointer Of : ");
      print_type(std::dynamic_pointer_cast<PointerType>(t)->valueType);
      println("*********************************");
    }break;
    case FUNCTION_TYPE : {
      auto func_type = std::dynamic_pointer_cast<FunctionType>(t);
      println("Function Of :");
      println("returnType : ");
      print_type(func_type->returnType);
      println("Arguments : ");
      for(auto& i : func_type->args){
        println(i.id," By ref ? ",i.byRef);
        print_type(i.type);
      }
      println("*********************************");
    }break;
    case VOID_TYPE :  
    break;
  }
}

void init_info(Info& info){
  info.types["int"] = std::make_shared<Type>(INT_TYPE);
  info.types["int"]->name = "int";
  info.types["uint"] = std::make_shared<Type>(UINT_TYPE);
  info.types["uint"]->name = "uint";
  info.types["real"] = std::make_shared<Type>(REAL_TYPE);
  info.types["real"]->name = "real";
  info.types["boolean"] = std::make_shared<Type>(BOOLEAN_TYPE);
  info.types["boolean"]->name = "boolean";
  info.types["char"] = std::make_shared<Type>(CHAR_TYPE);
  info.types["char"]->name = "char";
  info.types["string"] = std::make_shared<Type>(STRING_TYPE);
  info.types["string"]->name = "string";
  info.types["void"] = std::make_shared<Type>(VOID_TYPE);
  info.types["void"]->name = "void";
}

//Parser Rule

void Parser::program(){
  match_adv(PROGRAM_TOKEN);
  match_adv(ID_TOKEN);
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
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
    print_type(t,s);
  }
  for(auto& [s,t] : infos[0].variables){
    println("var (",s,")");
    print_type(t);
  }
  //match(DOT_TOKEN);
}

void Parser::block(){
  declaration_part();
  statement_part();
}

void Parser::declaration_part(){
  Info info;
  init_info(info);
  infos.emplace_front(std::move(info));
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
  // Saving constants (int | float | string)
  vm.add_inst(JMP_OP);
  uint beg = vm.add_data(0);
  for(auto &[name,c] : infos[0].constants){
    switch(c->type){
      case INT_TYPE: c->loc = vm.add_data((int)std::get<Int>(c->value)); break;
      case REAL_TYPE: c->loc = vm.add_data((float)std::get<double>(c->value)); break;
      case STRING_TYPE: c->loc = vm.write_const_string(std::get<std::string>(c->value)); 
      break;
      default:
        println("A constant (",name,") is an int, a real or a string !");
        exit(EXIT_FAILURE);
    }
  }
  vm.bytecode[beg] = (uint)vm.bytecode.size();
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
      exit(EXIT_FAILURE);
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
        c.type = STRING_TYPE;
        c.value = t.id;
        break;
    }
  }
  if(c.type == STRING_TYPE && (pos || neg)){
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
    quit_if_id_is_used(name);
    match_adv(EQ_TOKEN);
    lexer.next_sym();
    auto t = type();
    if(t->type != VOID_TYPE){
      infos[0].types[name] = t;
    }
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
      return structured_type();
      break;
    case POINTER_TOKEN: return std::dynamic_pointer_cast<Type>(pointer_type());  break;
    case PROCEDURE_TOKEN: return std::dynamic_pointer_cast<Type>(procedure_type()); break;
    case FUNCTION_TOKEN: return std::dynamic_pointer_cast<Type>(function_type()); break;
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
  if(
    lower_bound.type == STRING_TYPE &&
    get<std::string>(lower_bound.value).length() == 1 &&
    get<std::string>(upper_bound.value).length() == 1
  )
  {
    return get_subrange(get<std::string>(lower_bound.value)[0], get<std::string>(upper_bound.value)[0]);
  }
  println("Cannot create a subrange of something other than chars or ints !");
  exit(EXIT_FAILURE);
}

std::shared_ptr<Type> Parser::structured_type(){
  if(lexer.getToken().type == PACKED_TOKEN){
    lexer.next_sym();
  }
  return unpacked_structured_type();
}

std::shared_ptr<Type> Parser::unpacked_structured_type(){
  // TODO : shouldn't add safeguards (default case) ?
  switch(lexer.getToken().type){
    case ARRAY_TOKEN: return std::dynamic_pointer_cast<Type>(array_type()); break;
    case RECORD_TOKEN: return std::dynamic_pointer_cast<Type>(record_type()); break;
    // case SET_TOKEN: return std::dynamic_pointer_cast<Type>(set_type()); break;
    // case FILE_TOKEN: return std::dynamic_pointer_cast<Type>(file_type()); break;
    default:
      matches({
        ARRAY_TOKEN,
        RECORD_TOKEN,
        SET_TOKEN,
        FILE_TOKEN
      });
  }
  println("sets and files are not implemented !");
  return std::make_shared<Type>(VOID_TYPE);
}

std::shared_ptr<ArrayType> Parser::array_type(){
  match(ARRAY_TOKEN);
  match_adv(LB_TOKEN);
  std::vector<std::shared_ptr<Type>> indexTypes;
  do{
    lexer.next_sym();
    auto tmp = get_type(lexer.getToken().id);
    if(tmp){
      switch(tmp->type){
        case ENUM_TYPE:
        case SUBRANGE_TYPE:
        break;
        default: 
          println("an array index type is either (enum,subrange) !"); 
          exit(EXIT_FAILURE);
      }
      lexer.next_sym();
      indexTypes.emplace_back(tmp);
    }else{
      indexTypes.emplace_back(simple_type());
    }
  }while(lexer.getToken().type == COMMA_TOKEN);
  match(RB_TOKEN);
  match_adv(OF_TOKEN);
  lexer.next_sym();
  return get_array(indexTypes,type());
}

std::shared_ptr<RecordType> Parser::record_type(){
  Attributes atts;
  match(RECORD_TOKEN);
  lexer.next_sym();
  field_list(atts);
  match(END_TOKEN);
  lexer.next_sym();
  return get_record(std::move(atts));
}

void Parser::field_list(Attributes& atts){
  switch(lexer.getToken().type){
    case ID_TOKEN:
      fixed_part(atts);
      if(lexer.getToken().type == SEMI_TOKEN){
        lexer.next_sym();
        variant_part(atts);
      }else if(lexer.getToken().type == CASE_TOKEN){
        variant_part(atts);
      }
      break;
    case CASE_TOKEN: variant_part(atts); break;
    default:
      matches({ID_TOKEN,CASE_TOKEN});
  }
}

void Parser::fixed_part(Attributes& atts){
  auto v = id_list();
  match(COLON_TOKEN);
  lexer.next_sym();
  auto t = type();
  for(auto& s:v){
    if(atts.contains(s)){
      println("the record has defined 2 attributes with the same id (",s,") !");
      exit(EXIT_FAILURE);
    }
    atts[s] = t;
  }
  while(lexer.getToken().type == SEMI_TOKEN && lexer.next_sym().type != CASE_TOKEN){
    v = id_list();
    match(COLON_TOKEN);
    lexer.next_sym();
    t = type();
    for(auto& s:v){
      if (atts.contains(s)){
        println("the record has defined 2 attributes with the same id (", s, ") !");
        exit(EXIT_FAILURE);
      }
      atts[s] = t;
    }
  }
}

void Parser::variant_part(Attributes& atts){
  //TODO : Differentiate between var id and type name 
  match(CASE_TOKEN);
  std::string name = "tag";
  if(lexer.next_sym().type == ID_TOKEN && get_type(lexer.getToken().id) == nullptr){
    name = lexer.getToken().id;
    if(atts.contains(name)){
      println("the record has defined 2 attributes with the same id (", name, ") !");
      exit(EXIT_FAILURE);
    }
    match_adv(COLON_TOKEN);
  }
  match_adv(ID_TOKEN);
  auto t = get_type(lexer.getToken().id);
  if(!t){
    println("No type name (",lexer.getToken().id,") exists !");
    exit(EXIT_FAILURE);
  }
  if(t->type != INT_TYPE && t->type != REAL_TYPE && t->type != STRING_TYPE){
    println("The tag variable should be either a int, a real or a string !");
    exit(EXIT_FAILURE);
  }
  atts[name] = t;
  match_adv(OF_TOKEN);
  do{
    lexer.next_sym();
    variant(atts);
  }while(lexer.getToken().type == SEMI_TOKEN);
}

void Parser::variant(Attributes& atts){
  case_label_list();
  match(COLON_TOKEN);
  match_adv(LP_TOKEN);
  lexer.next_sym();
  field_list(atts);
  match(RP_TOKEN);
  lexer.next_sym();
}

std::vector<Const> Parser::case_label_list(){
  std::vector<Const> constants;
  constants.emplace_back(constant());
  while(lexer.getToken().type == COMMA_TOKEN){
    lexer.next_sym();
    constants.emplace_back(constant());
  }
  return constants;
}

std::shared_ptr<SetType> Parser::set_type(){
  match(SET_TOKEN);
  match_adv(OF_TOKEN);
  lexer.next_sym();
  auto tmp = get_type(lexer.getToken().id);
  if(tmp){
    switch(tmp->type){
      case INT_TYPE:
      case UINT_TYPE:
      case CHAR_TYPE:
      case BOOLEAN_TYPE:
      case ENUM_TYPE:
      case SUBRANGE_TYPE:
      break;
      default: 
        println("a set value type is either (int,uint,char,boolean,enum,subrange) !"); 
        exit(EXIT_FAILURE);
    }
    lexer.next_sym();
    return get_set(tmp);
  }else{
    return get_set(simple_type());
  }
}

std::shared_ptr<FileType> Parser::file_type(){
  match(FILE_TOKEN);
  if(lexer.next_sym().type == OF_TOKEN){
    lexer.next_sym();
    return get_file(type());
  }
  return get_file(get_type("int"));
}

std::shared_ptr<PointerType> Parser::pointer_type(){
  match(POINTER_TOKEN);
  match_adv(ID_TOKEN);
  std::string name = lexer.getToken().id;
  auto t = get_type(name);
  if(!t){
    println("No typename (",name,") exists !");
    exit(EXIT_FAILURE);
  }
  lexer.next_sym();
  return get_pointer(t);
}

void Parser::variable_declaration_part(){
  match(VAR_TOKEN);
  lexer.next_sym();
  do{
    std::vector<std::string> names = id_list();
    match(COLON_TOKEN);
    lexer.next_sym();
    auto t = type();
    for(auto& name : names){
      quit_if_id_is_used(name);
      infos[0].variables[name] = t;
    }
    match(SEMI_TOKEN);
  }while(lexer.next_sym().type == ID_TOKEN);
}

std::shared_ptr<FunctionType> Parser::procedure_type(){
  std::shared_ptr<FunctionType> functionType;
  match(PROCEDURE_TOKEN);
  lexer.next_sym();
  if(lexer.getToken().type == LP_TOKEN){
    return get_function(formal_parameter_list(),get_type("void"));
  }
  return get_function(std::vector<Arg>(), get_type("void"));
}

std::vector<Arg> Parser::formal_parameter_list(){
  std::vector<Arg> args;
  match(LP_TOKEN);
  do{
    lexer.next_sym();
    formal_parameter_section(args);
  }while(lexer.getToken().type == SEMI_TOKEN);
  match(RP_TOKEN);
  lexer.next_sym();
  return args;
}

bool args_same_name(const std::vector<Arg>& v,const std::string& s){
  for(auto& i:v){
    if(i.id == s){
      return true;
    }
  }
  return false;
}

void Parser::formal_parameter_section(std::vector<Arg>& args){
  Arg arg;
  arg.byRef = false;
  if(lexer.getToken().type == VAR_TOKEN){
    arg.byRef = true;
    lexer.next_sym();
  }
  auto names = id_list();
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  std::string type_name = lexer.getToken().id;
  auto t = get_type(type_name);
  if(!t){
    println("No type name with id (",type_name,") exists !");
    exit(EXIT_FAILURE);
  }
  arg.type = t;
  for(auto& s:names){
    if(args_same_name(args,s)){
      println("Arguments with the same name ?");
      exit(EXIT_FAILURE);
    }
    arg.id = s;
    args.push_back(arg);
  }
  lexer.next_sym();
}

std::shared_ptr<FunctionType> Parser::function_type(){
  std::vector<Arg> args;
  match(FUNCTION_TOKEN);
  lexer.next_sym();
  if(lexer.getToken().type == LP_TOKEN){
    args = formal_parameter_list();
  }
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  std::string type_name = lexer.getToken().id;
  auto t = get_type(type_name);
  if(!t){
    println("No type name with id (",type_name,") exists !");
    exit(EXIT_FAILURE);
  }
  lexer.next_sym();
  return get_function(args,t);
}

void Parser::procedure_declaration(){
  match(PROCEDURE_TOKEN);
  match_adv(ID_TOKEN);
  std::string name = lexer.getToken().id;
  quit_if_id_is_used(name);
  std::vector<Arg> v;
  if(lexer.next_sym().type == LP_TOKEN){
    v = formal_parameter_list();
  }
  match(SEMI_TOKEN);
  lexer.next_sym();
  infos[0].functions[name] = std::make_shared<Function>(std::move(v),get_type("void"));
  block();
}

void Parser::function_declaration(){
  match(FUNCTION_TOKEN);
  match_adv(ID_TOKEN);
  std::string name = lexer.getToken().id;
  quit_if_id_is_used(name);
  std::vector<Arg> v;
  if(lexer.next_sym().type == LP_TOKEN){
    v = formal_parameter_list();
  }
  match(COLON_TOKEN);
  match_adv(ID_TOKEN);
  std::string type_name = lexer.getToken().id;
  auto t = get_type(type_name);
  if(!t){
    println("No type name '",type_name,"' exists !");
    exit(EXIT_FAILURE);
  }
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
  infos[0].functions[name] = std::make_shared<Function>(std::move(v),t);
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