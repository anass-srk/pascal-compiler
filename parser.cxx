#include "parser.hxx"

//GET TYPES

std::shared_ptr<SubrangeType> Parser::get_subrange(Int lower,Int upper){
  if(lower > upper){
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
  p->boundsType = get_type("int");
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
  p->boundsType = get_type("char");
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

std::shared_ptr<EnumType> Parser::get_enum_value(const std::string &name){
  for(auto &i : infos){
    for(auto &t : i.types){
      if(t.second->type == ENUM_TYPE){
        auto p = std::dynamic_pointer_cast<EnumType>(t.second);
        if(p->ids.contains(name)){
          return p;
        }
      }
    }
  }
  return std::shared_ptr<EnumType>();
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
      // p->amount *= t->amount;
      switch(t->type){
        case ENUM_TYPE: p->amount *= std::dynamic_pointer_cast<EnumType>(t)->amount; break;
        case SUBRANGE_TYPE: p->amount *= std::dynamic_pointer_cast<SubrangeType>(t)->amount; break;
        default:
          println("An array indexing type is an enum or a subrange !");
          exit(EXIT_FAILURE);
      }
      p->indexTypes.emplace_back(t);
    }
    p->valueType = valueType;
    p->elem_size = p->valueType->size;
    p->size = p->amount * p->elem_size;
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
  p->size = 1;
  infos[0].types[p->name] = std::dynamic_pointer_cast<Type>(p);
  return p;
}

std::shared_ptr<RecordType> Parser::get_record(
  std::unordered_map<std::string, Var> &&attributes
){
  std::string name = "_record_";
  for(auto& [_,t] : attributes){
    name += "_" + t.type->name + "_" + _ + "_";
  }
  for(auto& i:infos){
    if(i.types.contains(name)){
      return std::dynamic_pointer_cast<RecordType>(i.types[name]);
    }
  }
  std::shared_ptr<RecordType> p = std::make_shared<RecordType>();
  p->name = std::move(name);
  p->attributes = std::move(attributes);
  p->size = 0;
  for(auto &[_,t] : p->attributes){
    t.loc = p->size;
    p->size += t.type->size;
  }
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
  if(program_name == id){
    println("The id (", id, ") is already taken !");
    exit(EXIT_FAILURE);
  }
  for(auto& [_,t] : infos[0].types){
    if(t->type == ENUM_TYPE && std::dynamic_pointer_cast<EnumType>(t)->ids.contains(id)){
      println("The id (", id, ") is already taken !");
      exit(EXIT_FAILURE);
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

std::shared_ptr<Var> Parser::get_variable(const std::string &id){
  for(auto& i : infos){
    if(i.variables.contains(id)){
      return i.variables[id];
    }
  }
  return std::shared_ptr<Var>();
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
    case CONST_STR_TYPE : println("Constant String (length >= 1)"); break;
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
      if(sub->boundsType == get_type("int")){
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
        print("Name : ",att.first," at ",att.second.loc," ");
        print_type(att.second.type);
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
    case VOID_TYPE :  println("Void"); break;
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
  info.types["void"] = std::make_shared<Type>(VOID_TYPE);
  info.types["void"]->name = "void";
}

void Parser::show_declarations(){
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
  println("Vars :");
  for(auto& [s,v] : infos[0].variables){
    println("var (",s,") at ",v->loc," ");
    print_type(v->type);
  }
  println("Functions : ");
  for(auto &[s,v] : infos[0].functions){
    println("Function name '",s,"' with type :");
    print_type(v->type);
  }
}

//Parser Rule

void Parser::program(){
  match_adv(PROGRAM_TOKEN);
  match_adv(ID_TOKEN);
  program_name = lexer.getToken().id;
  match_adv(SEMI_TOKEN);
  lexer.next_sym();
  block();
  match(DOT_TOKEN);
  vm.add_inst(HALT_OP);
  vm.run();
}

void Parser::block(){
  declaration_part();
  // show_declarations();
  statement_part();
}

void Parser::store_constants(){
  vm.add_inst(JMP_OP);
  uint beg = vm.add_data(0);
  for(auto &[name,c] : infos[0].constants){
    switch(c->type){
      case INT_TYPE: c->loc = vm.add_data((int)std::get<Int>(c->value)); break;
      case REAL_TYPE: c->loc = vm.add_data((float)std::get<double>(c->value)); break;
      case CHAR_TYPE: c->loc = vm.add_data((char)std::get<std::string>(c->value)[0]); break;
      case CONST_STR_TYPE:{
        std::string s = std::get<std::string>(c->value);
        if(s.length() == 0){
          println("Const strings'lengths should be greater than 0 !");
          exit(EXIT_FAILURE);
        }
        c->loc = vm.add_data((char)s[0]);
        for(uint i = 1;i < s.length();++i){
          vm.add_data((char)s[i]);
        }
      } break;
      default:
        println("A constant (",name,") is an int, a real or a string !");
        exit(EXIT_FAILURE);
    }
    println("Loc of '",name,"' is ",c->loc);
  }
  vm.bytecode[beg] = (uint)vm.bytecode.size();
}

uint Parser::store_variable(std::shared_ptr<Type> t){
  switch(t->type){
    case INT_TYPE:
    case UINT_TYPE:
    case REAL_TYPE:
    case CHAR_TYPE:
    case BOOLEAN_TYPE:
    case ENUM_TYPE:
    case SUBRANGE_TYPE:
    case POINTER_TYPE:
      return vm.add_inst(NOP_OP);
      break;
    case ARRAY_TYPE:{
      auto arr_type = std::dynamic_pointer_cast<ArrayType>(t);
      uint loc = store_variable(arr_type->valueType);
      for(uint i = 1;i < arr_type->amount;++i){
        store_variable(arr_type->valueType);
      }
      return loc;
    }break;
    case RECORD_TYPE:{
      auto record_type = std::dynamic_pointer_cast<RecordType>(t);
      uint loc = (uint)vm.bytecode.size();
      for(auto &[name,v] : record_type->attributes){
        store_variable(v.type);
      }
      return loc;
    }break;
    default:
      println("Can only create variables of these types (int,uint,real,char,boolean,subrange,enum,array,record) !");
      exit(EXIT_FAILURE);
  }
}

void Parser::store_variables(){
  for(auto &[_,v] : infos[0].variables){
    v->loc = store_variable(v->type);
    println("Storing variable '", _, "' in ",v->loc);
  }
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
  store_constants();
  // Saving variables
  store_variables();
  // vm.save_to_file("_.bin");
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
    matches({ID_TOKEN,NUM_INT_TOKEN,NUM_REAL_TOKEN,CHAR_LITERAL_TOKEN,STRING_LITERAL_TOKEN});
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
      case CHAR_LITERAL_TOKEN:
        c.type = CHAR_TYPE;
        c.value = t.id;
        break;
      case STRING_LITERAL_TOKEN:
        c.type = CONST_STR_TYPE;
        c.value = t.id;
        if(t.id.length() == 0){
          println("Const strings'lengths should be greater than 0 !");
          exit(EXIT_FAILURE);
        }
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
    case CHAR_LITERAL_TOKEN:
    case LP_TOKEN:
      return simple_type();
      break;
    case PACKED_TOKEN:
    case ARRAY_TOKEN:
    case RECORD_TOKEN:
    // case SET_TOKEN:
    // case FILE_TOKEN:
      return structured_type();
      break;
    // case POINTER_TOKEN: return std::dynamic_pointer_cast<Type>(pointer_type());  break;
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
        CHAR_LITERAL_TOKEN,
        LP_TOKEN,
        PACKED_TOKEN,
        ARRAY_TOKEN,
        RECORD_TOKEN,
        // SET_TOKEN,
        // FILE_TOKEN,
        // POINTER_TOKEN,
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
  if(lower_bound.type == CHAR_TYPE)
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
        // SET_TOKEN,
        // FILE_TOKEN
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
    // atts.insert(std::pair<std::string,Var>(s,t));
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
  if(t->type != INT_TYPE && t->type != REAL_TYPE && t->type != CHAR_TYPE){
    println("The tag variable should be either a int, a real or a char !");
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
      infos[0].variables[name] = std::make_shared<Var>(t);
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
  infos[0].functions[name] = std::make_shared<Var>(get_function(v,get_type("void")));
  block();
  show_declarations();
  infos.pop_front();
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
  infos[0].functions[name] = std::make_shared<Var>(get_function(v,t));
  block();
  infos.pop_front();
}

std::shared_ptr<Type> Parser::expression(){
  auto type_a = simple_expression();
  switch(lexer.getToken().type){
    case EQ_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)EQ_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    case NEQ_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)NE_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    case LT_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)LT_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    case LE_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)LE_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    case GT_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)GT_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    case GE_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)GE_FLAG);
      lexer.next_sym();
      auto type_b = simple_expression();
      check_store_comparison(type_a,type_b);
    }break;
    default: return type_a;
  }
  return get_type("boolean");
}

void Parser::check_store_comparison(std::shared_ptr<Type> a, std::shared_ptr<Type> b){
  if(a->type == SUBRANGE_TYPE){
    check_store_comparison(std::dynamic_pointer_cast<SubrangeType>(a)->boundsType,b);
    return;
  }
  if(b->type == SUBRANGE_TYPE){
    check_store_comparison(a,std::dynamic_pointer_cast<SubrangeType>(b)->boundsType);
    return;
  }
  if(a->type == INT_TYPE && b->type == INT_TYPE){
    vm.add_inst(TESTI_OP);
    return;
  }
  if(a->type == UINT_TYPE && b->type == UINT_TYPE){
    vm.add_inst(TESTU_OP);
    return;
  }
  if(a->type == REAL_TYPE && b->type == REAL_TYPE){
    vm.add_inst(TESTF_OP);
    return;
  }
  if (a->type == BOOLEAN_TYPE && b->type == BOOLEAN_TYPE){
    vm.add_inst(TESTB_OP);
    return;
  }
  if(a->type == CHAR_TYPE && b->type == CHAR_TYPE){
    vm.add_inst(TESTC_OP);
    return;
  }
  if(a->type == ENUM_TYPE && b->type == ENUM_TYPE){
    if(a != b){
      println("Cannot compare values of 2 different enums !");
      exit(EXIT_FAILURE);
    }
    vm.add_inst(TESTI_OP);
    return;
  }
  println("Unsupported comparison between different types !");
  exit(EXIT_FAILURE);
}

std::shared_ptr<Type> Parser::simple_expression(){
  bool neg = false;
  switch(lexer.getToken().type){
    case PLUS_TOKEN:{
      lexer.next_sym();
    }break;
    case MINUS_TOKEN:{
      neg = true;
      lexer.next_sym();
    }break;
    default: break;
  }
  auto type_a = term();
  bool in = true;
  while(in){
    auto tt = lexer.getToken().type;
    switch(tt){
      case PLUS_TOKEN:
      case MINUS_TOKEN:
      case OR_TOKEN:{
        lexer.next_sym();
        auto type_b = term();
        check_store_sum(type_a,type_b,tt);
        type_a = type_b;
      }break;
      default: in = false; break;
    }
  }
  if(neg){
    switch(type_a->type){
      case INT_TYPE:{
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data(-1);
        vm.add_inst(MULI_OP);
      }break;
      case REAL_TYPE:{
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data(-1.0f);
        vm.add_inst(MULF_OP);
      }break;
      case CHAR_TYPE:{
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((char)-1);
        vm.add_inst(MULC_OP);
      }break;
      default:
        println("Can negate only ints,floats/reals and chars !");
        exit(EXIT_FAILURE);
    }
  }
  return type_a;
}

void Parser::check_store_sum(std::shared_ptr<Type> a, std::shared_ptr<Type> b,TOKEN_TYPE tt){
  if(a->type == SUBRANGE_TYPE){
    check_store_sum(std::dynamic_pointer_cast<SubrangeType>(a)->boundsType,b,tt);
    return;
  }
  if(b->type == SUBRANGE_TYPE){
    check_store_sum(a,std::dynamic_pointer_cast<SubrangeType>(b)->boundsType,tt);
    return;
  }
  if(a->type == INT_TYPE && b->type == INT_TYPE){
    switch(tt){
      case PLUS_TOKEN: vm.add_inst(ADDI_OP); break;
      case MINUS_TOKEN: vm.add_inst(SUBI_OP); break;
      default :
        println("Summation operations for ints are + and - only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == UINT_TYPE && b->type == UINT_TYPE){
    switch(tt){
      case PLUS_TOKEN: vm.add_inst(ADDU_OP); break;
      case MINUS_TOKEN: vm.add_inst(SUBU_OP); break;
      default :
        println("Summation operations for uints are + and - only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == REAL_TYPE && b->type == REAL_TYPE){
    switch(tt){
      case PLUS_TOKEN: vm.add_inst(ADDF_OP); break;
      case MINUS_TOKEN: vm.add_inst(SUBF_OP); break;
      default :
        println("Summation operations for floats/reals are + and - only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if (a->type == BOOLEAN_TYPE && b->type == BOOLEAN_TYPE){
    switch(tt){
      case PLUS_TOKEN: vm.add_inst(ADDB_OP); break;
      case MINUS_TOKEN: vm.add_inst(SUBB_OP); break;
      case OR_TOKEN: vm.add_inst(OR_OP); break;
      default :
        println("Summation operations for booleans/bytes are + and - and 'or' only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == CHAR_TYPE && b->type == CHAR_TYPE){
    switch(tt){
      case PLUS_TOKEN: vm.add_inst(ADDC_OP); break;
      case MINUS_TOKEN: vm.add_inst(SUBC_OP); break;
      default :
        println("Summation operations for chars are + and - only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  println("Unsupported summation between different types !");
  exit(EXIT_FAILURE);
}

std::shared_ptr<Type> Parser::term(){
  auto type_a = factor();
  bool in = true;
  while(in){
    auto tt = lexer.getToken().type;
    switch(tt){
      case STAR_TOKEN:
      case SLASH_TOKEN:
      case DIV_TOKEN:
      case AND_TOKEN:{
        lexer.next_sym();
        auto type_b = factor();
        check_store_mul(type_a,type_b,tt);
        type_a = type_b;
      }break;
      default: in = false; break;
    }
  }
  return type_a;
}

void Parser::check_store_mul(std::shared_ptr<Type> a, std::shared_ptr<Type> b,TOKEN_TYPE tt){
  if(a->type == SUBRANGE_TYPE){
    check_store_mul(std::dynamic_pointer_cast<SubrangeType>(a)->boundsType,b,tt);
    return;
  }
  if(b->type == SUBRANGE_TYPE){
    check_store_mul(a,std::dynamic_pointer_cast<SubrangeType>(b)->boundsType,tt);
    return;
  }
  if(a->type == INT_TYPE && b->type == INT_TYPE){
    switch(tt){
      case STAR_TOKEN: vm.add_inst(MULI_OP); break;
      case DIV_TOKEN: vm.add_inst(DIVI_OP); break;
      default :
        println("multiplication operations for ints are * and DIV only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == UINT_TYPE && b->type == UINT_TYPE){
    switch(tt){
      case STAR_TOKEN: vm.add_inst(MULU_OP); break;
      case DIV_TOKEN: vm.add_inst(DIVU_OP); break;
      default :
        println("multiplication operations for uints are * and DIV only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == REAL_TYPE && b->type == REAL_TYPE){
    switch(tt){
      case STAR_TOKEN: vm.add_inst(MULF_OP); break;
      case SLASH_TOKEN: vm.add_inst(DIVF_OP); break;
      default :
        println("multiplication operations for floats/reals are * and / only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if (a->type == BOOLEAN_TYPE && b->type == BOOLEAN_TYPE){
    switch(tt){
      case STAR_TOKEN: vm.add_inst(MULB_OP); break;
      case DIV_TOKEN: vm.add_inst(DIVB_OP); break;
      case AND_TOKEN: vm.add_inst(AND_OP); break;
      default :
        println("multiplication operations for booleans/bytes are * and DIV and 'and' only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == CHAR_TYPE && b->type == CHAR_TYPE){
    switch(tt){
      case STAR_TOKEN: vm.add_inst(MULC_OP); break;
      case DIV_TOKEN: vm.add_inst(DIVC_OP); break;
      default :
        println("multiplication operations for chars are * and DIV only !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  println("Unsupported summation between different types !");
  exit(EXIT_FAILURE);
}

std::shared_ptr<Type> Parser::factor(){
  auto token = lexer.getToken();
  switch(token.type){
    case NUM_INT_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((int)token.ival);
      lexer.next_sym();
      return get_type("int");
    }break;
    case NUM_REAL_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((float)token.dval);
      lexer.next_sym();
      return get_type("real");
    }break;
    case CHAR_LITERAL_TOKEN:{
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((char)token.id[0]);
      lexer.next_sym();
      return get_type("char");
    }break;
    case STRING_LITERAL_TOKEN:{
      std::string s = lexer.getToken().id;
      if(s.length() == 0){
        println("Const strings'lengths should be greater than 0 !");
        exit(EXIT_FAILURE);
      }
      for(uint i = 0;i < s.length();++i){
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((char)s[i]);
      }
      lexer.next_sym();
      return std::shared_ptr<Type>(get_array(
        std::vector<std::shared_ptr<Type>>{get_subrange(0,(Int)s.length()-1)},
        get_type("char")));
    }break;
    //case NIL_TOKEN:
    //should check if ID is var or function call
    case ID_TOKEN:{
      auto c = get_constant(token.id);
      std::shared_ptr<EnumType> enum_type;
      if(c){
        lexer.next_sym();
        switch(c->type){
          case REAL_TYPE:{
            vm.add_inst(PUSH_OP);
            vm.add_data(c->loc);
            return get_type("real");
          }break;
          case INT_TYPE:{
            vm.add_inst(PUSH_OP);
            vm.add_data(c->loc);
            return get_type("int");
          }break;
          case CHAR_TYPE:{
            vm.add_inst(PUSH_OP);
            vm.add_data(c->loc);
            return get_type("char");
          }break;
          case CONST_STR_TYPE:{
            std::string s = std::get<std::string>(c->value);
            if(s.length() == 0){
              println("Const strings'lengths should be greater than 0 !");
              exit(EXIT_FAILURE);
            }
            for(uint i = 0;i < s.length();++i){
              vm.add_inst(PUSH_CONST_OP);
              vm.add_data((char)s[i]);
            }
            return std::shared_ptr<Type>(get_array(
              std::vector<std::shared_ptr<Type>>{get_subrange(0,(Int)s.length()-1)},
              get_type("char")));
          } break;
        }
      }else if((enum_type = get_enum_value(token.id))){
        lexer.next_sym();
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((int)enum_type->ids[token.id]);
        return std::dynamic_pointer_cast<Type>(enum_type);
      }else{
        auto v = variable_access();
        if(v){
          vm.add_inst(GET_VAL_OP);
          return v;
        }else{
          println("No variable or constant or enum value named '", token.id, "' exists !");
          exit(EXIT_FAILURE);
        }
      }
    }break;
    case LP_TOKEN:{
      lexer.next_sym();
      auto res = expression();
      match(RP_TOKEN);
      lexer.next_sym();
      return res;
    }break;
    case NOT_TOKEN:{
      lexer.next_sym();
      auto f = factor();
      if(f->type != BOOLEAN_TYPE){
        println("'not' operator can be used with booleans/bytes only !");
        exit(EXIT_FAILURE);
      }
      vm.add_inst(NOT_OP);      
      return f;
    }break;
    default:
      matches({
        NUM_INT_TOKEN,
        NUM_REAL_TOKEN,
        STRING_LITERAL_TOKEN,
        CHAR_LITERAL_TOKEN,
        //NIL_TOKEN,
        //ID_TOKEN,
        LP_TOKEN,
        NOT_TOKEN
      });
  }
  return std::shared_ptr<Type>();
}

void Parser::statement_part(){
  // lexer.next_sym();
  // auto t = expression();
  // switch(t->type){
  //   case INT_TYPE:{
  //     vm.add_inst(PUSH_CONST_OP);
  //     vm.add_data(INT_STD);
  //     vm.add_inst(WRITE_OP);
  //   }break;
  //   case REAL_TYPE:{
  //     vm.add_inst(PUSH_CONST_OP);
  //     vm.add_data(REAL_STD);
  //     vm.add_inst(WRITE_OP);
  //   }break;
  //   case BOOLEAN_TYPE:{
  //     vm.add_inst(PUSH_CONST_OP);
  //     vm.add_data(UINT_STD);
  //     vm.add_inst(WRITE_OP);
  //   }break;
  //   case UINT_TYPE:
  //   case CHAR_TYPE:
  //     vm.add_inst(PUSH_CONST_OP);
  //     vm.add_data(CHAR_STD);
  //     vm.add_inst(WRITE_OP);
  //     break;
  // }
  // vm.add_inst(HALT_OP);
  // vm.run();
  match(BEGIN_TOKEN);
  lexer.next_sym();
  statement_sequence();
  match(END_TOKEN);
  lexer.next_sym();
}

void Parser::statement_sequence(){
  statement();
  while(lexer.getToken().type == SEMI_TOKEN){
    lexer.next_sym();
    statement();
  }
}

void Parser::statement(){
  if(lexer.getToken().type == NUM_INT_TOKEN){
    Int id = lexer.getToken().ival;
    get_label(id).loc = (uint)vm.bytecode.size();
    match_adv(COLON_TOKEN);
    lexer.next_sym();
  }
  auto token = lexer.getToken();
  switch(token.type){
    // Should handle functions and procedures
    case ID_TOKEN:{
      auto var = get_variable(token.id);
      if(var){
        assignment_statement();
      }else{
        println("No variable '",token.id,"' exists !");
        exit(EXIT_FAILURE);
      }
    }break;
    case READ_TOKEN:
    case WRITE_TOKEN: procedure_statement(); break;
    case GOTO_TOKEN: goto_statement(); break;
    case BEGIN_TOKEN: statement_part(); break;
    case WHILE_TOKEN: while_statement(); break;
    case REPEAT_TOKEN: repeat_statement(); break;
    case FOR_TOKEN: for_statement(); break;
    case IF_TOKEN: if_statement(); break;
    case CASE_TOKEN: case_statement(); break;
    default:
      matches({
        ID_TOKEN,
        READ_TOKEN,
        WRITE_TOKEN,
        GOTO_TOKEN,
        BEGIN_TOKEN,
        WHILE_TOKEN,
        REPEAT_TOKEN,
        FOR_TOKEN,
        IF_TOKEN,
        CASE_TOKEN
      });
  }
}
// Should handle functions and procedures
void Parser::assignment_statement(){
  match(ID_TOKEN);
  std::string name = lexer.getToken().id;
  auto v = variable_access();
  if(!v){
    println("No variable named'",name,"' exists !");
    exit(EXIT_FAILURE);
  }
  match(ASSIGN_TOKEN);
  lexer.next_sym();
  auto e = expression();
  assign_var(v,e);
}

void Parser::assign_var(std::shared_ptr<Type> a, std::shared_ptr<Type> b){
  if(a->type == b->type){
    switch(a->type){
      case INT_TYPE:
      case UINT_TYPE:
      case REAL_TYPE:
      case BOOLEAN_TYPE:
      case CHAR_TYPE:
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((uint)2);
        vm.add_inst(REV_OP);
        
        vm.add_inst(MOV_OP);
        break;
      default:
        println("Assignment works only on basic types !");
        exit(EXIT_FAILURE);
    }
    return;
  }
  if(a->type == CHAR_TYPE && b->type == ARRAY_TYPE){
    auto arr_type = std::dynamic_pointer_cast<ArrayType>(b);
    if(arr_type->valueType->type != CHAR_TYPE){
      println("Can only assign first char of an array char to a char !");
      exit(EXIT_FAILURE);
    }
    println("LEN ",arr_type->amount);
    for(uint i = 1;i < arr_type->amount;++i){
      vm.add_inst(POP_OP);
    }
    vm.add_inst(PUSH_CONST_OP);
    vm.add_data((uint)2);
    vm.add_inst(REV_OP);

    vm.add_inst(MOV_OP);
    return;
  }
  println("Cannot assign an expression to a variable of a different type ! (",a->name,",",b->name,")");
  exit(EXIT_FAILURE);
}

std::shared_ptr<Type> Parser::variable_access(){
  if(lexer.getToken().type != ID_TOKEN){
    return nullptr;
  }
  auto v = get_variable(lexer.getToken().id);
  if(!v){
    return nullptr;
  }
  Var var = *v;
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(var.loc);
  bool in = true;
  lexer.next_sym();
  while(in){
    switch(lexer.getToken().type){
      case LB_TOKEN:{
        var.type = array_access_(var.type);
        // if(var.type->type == ARRAY_TYPE){
        //   auto arr_type = std::dynamic_pointer_cast<ArrayType>(v->type);
        //   var.type = arr_type->valueType;
        //   if(arr_type->indexTypes.size() != vec.size()){
        //     println("Expected ",arr_type->indexTypes.size()," indices while ",vec.size()," were provided !");
        //     exit(EXIT_FAILURE);
        //   }
        //   uint total_size = 0;
        //   for(auto& t : vec){
        //     total_size += t->size;
        //   }
        //   vm.add_inst(PUSH_CONST_OP);
        //   vm.add_data(total_size);
        //   vm.add_inst(REV_OP);
        //   for(uint i = 0;i < vec.size();++i){
        //     if(arr_type->indexTypes[i]->type == ENUM_TYPE){
        //       if(arr_type->indexTypes[i] != vec[i]){
        //         println("Expected an enum value as ", i, "-th index for the array !");
        //         exit(EXIT_FAILURE);
        //       }
        //       vm.add_inst(PUSH_CONST_OP);
        //       vm.add_data(std::dynamic_pointer_cast<EnumType>(arr_type->indexTypes[i])->amount);
        //       vm.add_inst(MULI_OP);
        //     }
        //     if(arr_type->indexTypes[i]->type == SUBRANGE_TYPE){
        //       auto sub_type = std::dynamic_pointer_cast<SubrangeType>(arr_type->indexTypes[i]);
        //       if(vec[i]->type == sub_type->boundsType->type ||
        //       (vec[i]->type == SUBRANGE_TYPE &&
        //       std::dynamic_pointer_cast<SubrangeType>(vec[i])->boundsType->type == sub_type->boundsType->type)){
                
        //         vm.add_inst(PUSH_CONST_OP);
        //         if(sub_type->boundsType->type == INT_TYPE){
        //           vm.add_data((int)sub_type->ibounds[0]);
        //           vm.add_inst(SUBI_OP);

        //           vm.add_inst(PUSH_CONST_OP);
        //           vm.add_data((int)(sub_type->amount));
        //         }else{
        //           //To be checked
        //           vm.add_data((int)sub_type->cbounds[0]);
        //           vm.add_inst(SUBI_OP);

        //           vm.add_inst(PUSH_CONST_OP);
        //           vm.add_data((int)(sub_type->amount));
        //         }
        //         vm.add_inst(MULI_OP);
        //       }else{
        //         println("Expected as index ",i," a value of the same type as the array's index type !");
        //         exit(EXIT_FAILURE);
        //       }
        //     }
        //   }

        // }else{
        //   println("Only arrays can be indexed !");
        //   exit(EXIT_FAILURE);
        // }
      }break;
      case DOT_TOKEN:{
        var.type = record_access_(var.type);
      }break;
      default: in = false; break;
    }
  }
  return var.type;
}

std::shared_ptr<Type> Parser::array_access_(std::shared_ptr<Type> t){
  if(t->type != ARRAY_TYPE){
    println("Expected array when indexing !");
    exit(EXIT_FAILURE);
  }
  auto arr_type = std::dynamic_pointer_cast<ArrayType>(t);
  match(LB_TOKEN);
  lexer.next_sym();
  auto type_a = expression();
  uint size = arr_type->elem_size;
  if(arr_type->indexTypes[0]->type == ENUM_TYPE){
    if(arr_type->indexTypes[0] != type_a){
      println("Expected an enum value as an index for the array !");
      exit(EXIT_FAILURE);
    }
    vm.add_inst(PUSH_CONST_OP);
    vm.add_data((int)size);
    vm.add_inst(MULI_OP);

    size *= std::dynamic_pointer_cast<EnumType>(arr_type->indexTypes[0])->amount;
  }else if(arr_type->indexTypes[0]->type == SUBRANGE_TYPE){
    auto sub_type = std::dynamic_pointer_cast<SubrangeType>(arr_type->indexTypes[0]);
    if(type_a->type == sub_type->boundsType->type ||
    (type_a->type == SUBRANGE_TYPE &&
    std::dynamic_pointer_cast<SubrangeType>(type_a)->boundsType->type == sub_type->boundsType->type)){
      vm.add_inst(PUSH_CONST_OP);
      if(sub_type->boundsType->type == INT_TYPE){
        vm.add_data((int)sub_type->ibounds[0]);
        vm.add_inst(SUBI_OP);
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((int)size);
      }else{
        //To be checked
        vm.add_data((int)sub_type->cbounds[0]);
        vm.add_inst(SUBI_OP);
        vm.add_inst(PUSH_CONST_OP);
        vm.add_data((int)size);
      }
      vm.add_inst(MULI_OP);
      size *= sub_type->amount;
    }else{
      println("Expected as index a value of the same type as the array's index type !");
      exit(EXIT_FAILURE);
    }
  }
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)UINT_STD);
  vm.add_inst(CONVI_OP);

  vm.add_inst(ADDU_OP);

  uint i = 1;
  while(lexer.getToken().type == COMMA_TOKEN){
    if(i >= arr_type->indexTypes.size()){
      break;
    }
    lexer.next_sym();
    type_a = expression();
    if (arr_type->indexTypes[i]->type == ENUM_TYPE){
      if (arr_type->indexTypes[i] != type_a){
        println("Expected an enum value as an index for the array !");
        exit(EXIT_FAILURE);
      }
      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((int)size);
      vm.add_inst(MULI_OP);

      size *= std::dynamic_pointer_cast<EnumType>(arr_type->indexTypes[i])->amount;
    }
    else if (arr_type->indexTypes[i]->type == SUBRANGE_TYPE){
      auto sub_type = std::dynamic_pointer_cast<SubrangeType>(arr_type->indexTypes[i]);
      if (type_a->type == sub_type->boundsType->type ||
          (type_a->type == SUBRANGE_TYPE &&
          std::dynamic_pointer_cast<SubrangeType>(type_a)->boundsType->type == sub_type->boundsType->type))
      {
        vm.add_inst(PUSH_CONST_OP);
        if (sub_type->boundsType->type == INT_TYPE){
          vm.add_data((int)sub_type->ibounds[0]);
          vm.add_inst(SUBI_OP);
          vm.add_inst(PUSH_CONST_OP);
          vm.add_data((int)size);
        }
        else{
          // To be checked
          vm.add_data((int)sub_type->cbounds[0]);
          vm.add_inst(SUBI_OP);
          vm.add_inst(PUSH_CONST_OP);
          vm.add_data((int)size);
        }
        vm.add_inst(MULI_OP);
        size *= sub_type->amount;
      }
      else{
        println("Expected as index a value of the same type as the array's index type !");
        exit(EXIT_FAILURE);
      }
    }
    vm.add_inst(PUSH_CONST_OP);
    vm.add_data((uint)UINT_STD);
    vm.add_inst(CONVI_OP);

    vm.add_inst(ADDU_OP);
    ++i;
  }
  if(i != arr_type->indexTypes.size()){
    println("Expected ",arr_type->indexTypes.size()," index types, found ",i," !");
    exit(EXIT_FAILURE);
  }

  // if(lexer.getToken().type == COMMA_TOKEN){
  //   println("Array access currently supports 1 dim arrays only !");
  //   exit(EXIT_FAILURE);
  // }
  match(RB_TOKEN);
  lexer.next_sym();
  return arr_type->valueType;
}

std::shared_ptr<Type> Parser::record_access_(std::shared_ptr<Type> t){
  if(t->type != RECORD_TYPE){
    println("Only records have attributes !");
    exit(EXIT_FAILURE);
  }
  match(DOT_TOKEN);
  match_adv(ID_TOKEN);
  std::string attribute_name = lexer.getToken().id;
  auto rec_type = std::dynamic_pointer_cast<RecordType>(t);
  if(!rec_type->attributes.contains(attribute_name)){
    println("Attribute '",attribute_name,"' does not exist for type '",rec_type->name,"' !");
    exit(EXIT_FAILURE);
  }
  Var v = rec_type->attributes[attribute_name];

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)v.loc);
  vm.add_inst(ADDU_OP);

  lexer.next_sym();  
  return v.type;
}
//Should deal with procedures
void Parser::procedure_statement(){
  switch(lexer.getToken().type){
  case READ_TOKEN:{
    auto tt = lexer.getToken().type;
    if(lexer.next_sym().type == LP_TOKEN){
      do{
        lexer.next_sym();
        auto t = variable_access();
        if(!t){
          println("Can read variables only !");
        }
        if(t->type == ARRAY_TYPE){
        if (auto arr_type = std::dynamic_pointer_cast<ArrayType>(t); arr_type->valueType->type == CHAR_TYPE){
              vm.add_inst(PUSH_CONST_OP);
              vm.add_data((uint)STRING_STD);
              vm.add_inst(READ_OP);
              vm.add_data((uint)arr_type->amount);
            }
            else{
              println("Can read arrays of chars only !");
              exit(EXIT_FAILURE);
            }
        }else{
          vm.add_inst(PUSH_ADDR_OP);
          switch(t->type){
            case INT_TYPE:  vm.add_data((uint)INT_STD); break;  
            case UINT_TYPE: vm.add_data((uint)UINT_STD);  break;
            case REAL_TYPE: vm.add_data((uint)REAL_STD);  break;
            case BOOLEAN_TYPE:  vm.add_data((uint)UCHAR_STD);  break;
            case CHAR_TYPE: vm.add_data((uint)CHAR_STD);  break;
            default:
              println("Expected basic type for reading/writing data !");
              exit(EXIT_FAILURE);
          }
          vm.add_inst(READ_OP);
        }
      }while(lexer.getToken().type == COMMA_TOKEN);
      match(RP_TOKEN);
      lexer.next_sym();
    }
  }break;
  case WRITE_TOKEN:{
    auto tt = lexer.getToken().type;
    if(lexer.next_sym().type == LP_TOKEN){
      do{
        lexer.next_sym();
        auto t = variable_access();
        bool isvar_arr = false;
        if(t){
          if(t->type == ARRAY_TYPE){
            isvar_arr = true;
            if (auto arr_type = std::dynamic_pointer_cast<ArrayType>(t); arr_type->valueType->type == CHAR_TYPE){
              for (uint i = 0; i < arr_type->amount; ++i){
                vm.add_inst(DUPL_OP);
                
                vm.add_inst(PUSH_CONST_OP);
                vm.add_data((uint)i);
                vm.add_inst(ADDU_OP);

                vm.add_inst(GET_VAL_OP);
                
                vm.add_inst(PUSH_CONST_OP);
                vm.add_data((uint)CHAR_STD);
                vm.add_inst(WRITE_OP);
              }
              vm.add_inst(POP_OP);
            }
            else{
              println("Can print arrays of chars only !");
              exit(EXIT_FAILURE);
            }
          }else{
            vm.add_inst(GET_VAL_OP);
          }
        }else{
          t = expression();
        }
        if(t->type == ARRAY_TYPE && !isvar_arr){
            if (auto arr_type = std::dynamic_pointer_cast<ArrayType>(t); arr_type->valueType->type == CHAR_TYPE){
              vm.add_inst(PUSH_CONST_OP);
              vm.add_data((uint)arr_type->amount);
              vm.add_inst(REV_OP);
              for (uint i = 0; i < arr_type->amount; ++i){
                vm.add_inst(PUSH_CONST_OP);
                vm.add_data((uint)CHAR_STD);
                vm.add_inst(WRITE_OP);
              }
            }
            else{
              println("Can print arrays of chars only !");
              exit(EXIT_FAILURE);
            }
        }else if(!isvar_arr){
          vm.add_inst(PUSH_CONST_OP);
          switch(t->type){
            case INT_TYPE:  vm.add_data((uint)INT_STD); break;  
            case UINT_TYPE: vm.add_data((uint)UINT_STD);  break;
            case REAL_TYPE: vm.add_data((uint)REAL_STD);  break;
            case BOOLEAN_TYPE:  vm.add_data((uint)UCHAR_STD);  break;
            case CHAR_TYPE: vm.add_data((uint)CHAR_STD);  break;
            default:
              println("Expected basic type for reading/writing data !");
              exit(EXIT_FAILURE);
          }
          vm.add_inst(WRITE_OP);
        }
      }while(lexer.getToken().type == COMMA_TOKEN);
      match(RP_TOKEN);
      lexer.next_sym();
    }
  }break;
    // case ID_TOKEN:
  default:
    println("Procedure calls not implemented except read and write !");
    exit(EXIT_FAILURE);
  }
}

void Parser::goto_statement(){
  match(GOTO_TOKEN);
  match_adv(NUM_INT_TOKEN);
  Label l = get_label(lexer.getToken().ival);
  if(l.loc == -1){
    println("The label has been declared but not yet defined !");
    exit(EXIT_FAILURE);
  }
  vm.add_inst(JMP_OP);
  vm.add_data((uint)l.loc);
  lexer.next_sym();
}

void Parser::while_statement(){
  match(WHILE_TOKEN);
  lexer.next_sym();
  uint beg = (uint)vm.bytecode.size();
  auto t = expression();
  if(t->type != BOOLEAN_TYPE){
    println("For conditions only booleans are accepted !");
    exit(EXIT_FAILURE);
  }
  match(DO_TOKEN);
  lexer.next_sym();

  vm.add_inst(JMPFALSE_OP);
  uint end = vm.add_data(0);
  
  statement();
  
  vm.add_inst(JMP_OP);
  vm.add_data((uint)beg);
  
  vm.bytecode[end] = (uint)vm.bytecode.size();
}

void Parser::repeat_statement(){
  uint beg = (uint)vm.bytecode.size();

  match(REPEAT_TOKEN);
  lexer.next_sym();
  statement_sequence();
  match(UNTIL_TOKEN);
  lexer.next_sym();
  auto t = expression();
  if(t->type != BOOLEAN_TYPE){
    println("For conditions only booleans are accepted !");
    exit(EXIT_FAILURE);
  }
  vm.add_inst(JMPFALSE_OP);
  vm.add_data((uint)beg);
}

void Parser::for_statement(){
  match(FOR_TOKEN);
  match_adv(ID_TOKEN);
  std::string name = lexer.getToken().id;
  auto var = variable_access();
  if(!var){
    println("No variable named '",name,"' exists !");
    exit(EXIT_FAILURE);
  }
  vm.add_inst(DUPL_OP);
  match(ASSIGN_TOKEN);
  lexer.next_sym();
  auto e1 = expression();
  assign_var(var,e1);
  matches({TO_TOKEN,DOWNTO_TOKEN});
  lexer.next_sym();
  uint beg = (uint)vm.bytecode.size();
  vm.add_inst(DUPL_OP); // 2 addresses of var at this point at the top of the stack
  vm.add_inst(GET_VAL_OP);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)LT_FLAG);
  auto e = expression();
  check_store_comparison(var,e);
  vm.add_inst(JMPFALSE_OP);
  uint end = vm.add_data(0);
  match(DO_TOKEN);
  lexer.next_sym();
  statement();
  vm.add_inst(DUPL_OP);
  vm.add_inst(DUPL_OP);

  vm.add_inst(GET_VAL_OP);

  vm.add_inst(PUSH_CONST_OP);
  switch(var->type){
    case INT_TYPE:{
      vm.add_data((int)1);
      vm.add_inst(ADDI_OP);
    }break;
    case UINT_TYPE:{
      vm.add_data((uint)1);
      vm.add_inst(ADDU_OP);
    }break; 
    case CHAR_TYPE:{
      vm.add_data((char)1);
      vm.add_inst(ADDC_OP);
    }break;
    case BOOLEAN_TYPE:{
      vm.add_data((u_char)1);
      vm.add_inst(ADDB_OP);
    }break;
    default:
      println("For loop accepts only ints,uints,chars and bytes !");
      exit(EXIT_FAILURE);
  }

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)2);
  vm.add_inst(REV_OP);

  vm.add_inst(MOV_OP);  


  vm.add_inst(JMP_OP);
  vm.add_data((uint)beg);
  vm.bytecode[end] = (uint)vm.add_inst(POP_OP);
}

void Parser::if_statement(){
  match(IF_TOKEN);
  lexer.next_sym();
  auto t = expression();
  if(t->type != BOOLEAN_TYPE){
    println("For conditions only booleans are accepted !");
    exit(EXIT_FAILURE);
  }
  vm.add_inst(JMPFALSE_OP);
  uint ELSE = vm.add_data(0);
  match(THEN_TOKEN);
  lexer.next_sym();
  statement();
  
  vm.add_inst(JMP_OP);
  uint end = vm.add_data(0);

  vm.bytecode[ELSE] = (uint)vm.bytecode.size();
  if(lexer.getToken().type == ELSE_TOKEN){
    lexer.next_sym();
    statement();
  }
  vm.bytecode[end] = (uint)vm.bytecode.size();
}

void Parser::case_statement(){
  match(CASE_TOKEN);
  lexer.next_sym();
  auto e = expression();
  println("EXP : ",e->type);
  switch(e->type){
    case INT_TYPE: 
    case REAL_TYPE:
    case CHAR_TYPE:
    break;
    default:
      println("Can compare chars, ints and floats only !");
      exit(EXIT_FAILURE);
  }
  match(OF_TOKEN);
  std::vector<uint> ends;
  std::set<decltype(Const::value)> all_consts;
  do{
    lexer.next_sym();
    auto constants = case_label_list();
    std::vector<uint> elems;
    std::vector<uint> local_ends;
    for(auto &c : constants){
      if(all_consts.contains(c.value)){
        print("This constant has already been used ! (");
        std::visit([](auto& v){println(v,")");},c.value);
        exit(EXIT_FAILURE);
      }
      all_consts.emplace(c.value);

      vm.add_inst(DUPL_OP);

      vm.add_inst(PUSH_CONST_OP);
      vm.add_data((uint)EQ_FLAG);

      vm.add_inst(PUSH_CONST_OP);
      std::shared_ptr<Type> t;
      switch(c.type){
        case INT_TYPE:{
          t = get_type("int");
          vm.add_data((int)std::get<Int>(c.value));
        }break;
        case REAL_TYPE:{
          t = get_type("real");
          vm.add_data((float)std::get<double>(c.value));
        }break;
        case CHAR_TYPE:{
          t = get_type("char");
          vm.add_data((char)std::get<std::string>(c.value)[0]);
        }break;
        default:
          println("Expected an int, a real of a char in case statements !");
          exit(EXIT_FAILURE);
      }
      check_store_comparison(e,t);
      vm.add_inst(DUPL_OP);
      vm.add_inst(JMPTRUE_OP);
      elems.push_back(vm.add_data(0));
      vm.add_inst(JMPFALSE_OP);
      local_ends.push_back(vm.add_data(0));
    }

    match(COLON_TOKEN);
    lexer.next_sym();
    for(auto elem : elems){
      vm.bytecode[elem] = (uint)vm.bytecode.size();
    }
    statement();
    vm.add_inst(JMP_OP);
    ends.push_back(vm.add_data(0));
    for(auto l : local_ends){
      vm.bytecode[l] = (uint)vm.bytecode.size();
    }
  }while(lexer.getToken().type != END_TOKEN);
  for(auto &end : ends){
    vm.bytecode[end] = (uint)vm.bytecode.size();
  }
  match(END_TOKEN);
  lexer.next_sym();
}