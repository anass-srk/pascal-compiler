#ifndef PARSER_HXX
#define PARSER_HXX
#include "lexer.hxx"
#include <vector>
#include <initializer_list>
#include <deque>
#include <optional>
#include "semantics.hxx"
#include "vm.hxx"

// program := PROGRAM NAME '(' id_list ')' ';' block '.' 
// id_list := NAME { ',' NAME } 
// block := declaration_part statement_part .
// label := NUMBER .
// formal_parameter_list= '(' formal_parameter_section
// { ';' formal_parameter_section } ')' .
// formal_parameter_section= [ VAR ]id_list ':' parameter_type .
// parameter_type= TYPE_NAME .
// constant= [ '+' | '-' ] ( CONSTANT_NAME | NUMBER ) | STRING .
// case_label_list= constant { ',' constant } .
// type= simple_type | structured_type | pointer_type | procedural_type | TYPE_NAME .
// simple_type= subrange_type | enumerated_type .
// subrange_type= constant '..' constant .
// enumerated_type= '(' id_list ')' .
// structured_type= [ PACKED ] unpacked_structured_type .
// unpacked_structured_type= array_type | record_type | set_type
// | file_type .
// array_type= ARRAY '[' index_type { ',' index_type } ']' OF
// element_type .
// index_type= simple_type .
// element_type= type .
// record_type= RECORD field_list END .
// field_list= [ ( fixed_part [ ';' variant_part ] | variant_part ) [ ';' ] ] .
// fixed_part= record_section { ';' record_section } .
// record_section= id_list ':' type .
// variant_part= CASE tag_field TYPE_NAME OF variant { ';' variant } .
// tag_field= [ NAME ':' ] .
// variant= case_label_list ':' '(' field_list ')' .
// set_type= SET OF base_type .
// base_type= type .
// file_type= FILE [ OF file_component_type ] .
// file_component_type= type .
// pointer_type= '^' TYPE_NAME .
// procedural_type= procedure_type | function_type .
// procedure_type= PROCEDURE [ formal_parameter_list ] .
// function_type= FUNCTION [ formal_parameter_list ] ':' TYPE_NAME .
// declaration_part= { label_declaration_part | constant_definition_part |
// | type_definition_part | variable_declaration_part
// | procedure_and_function_declaration_part } .
// label_declaration_part= LABEL label { ',' label } ';' .
// constant_definition_part= CONST constant_definition ';'
// { constant_definition ';' } .
// constant_definition= NAME '=' constant .
// type_definition_part= TYPE type_definition ';' { type_definition ';' } .
// type_definition= NAME '=' type .
// variable_declaration_part= VAR variable_declaration ';'
// { variable_declaration ';' } .
// variable_declaration= id_list ':' type .
// procedure_and_function_declaration_part=
// ( procedure_declaration | function_declaration ) ';'.
// directive= FORWARD .
// procedure_declaration= procedure_heading ';' ( block | directive ) .
// procedure_heading= PROCEDURE NAME [ formal_parameter_list ] .
// function_declaration= function_heading ';' ( block | directive ) .
// function_heading= FUNCTION NAME [ formal_parameter_list ] ':' TYPE_NAME .
// statement_part= BEGIN statement_sequence END .
// statement_sequence= statement { ';' statement } .
// expression= F .
// expression_list= expression { ',' expression } .
// variable_access= ACCESS_NAME { end_access_ } .
// end_access_= { array_access_ | record_access_ | '^' | function_parameters_ } .
// array_access_= '[' expression_list ']' .
// record_access_= '.' variable_access .
// function_parameters_= '(' [ expression_list ] ')' .
// actual_parameter_list= '(' actual_parameter { ',' actual_parameter } ')' .
// actual_parameter= actual_value | actual_variable | actual_procedure
// | actual_function .
// actual_procedure= PROCEDURE_NAME .
// actual_function= FUNCTION_NAME .
// actual_value= expression .
// actual_variable= variable_access .
// expression= simple_expression [ relational_operator simple_expression ] .
// relational_operator= '=' | '<>' | '<' | '<=' | '>' | '>=' | IN .
// simple_expression= [ '+' | '-' ] term { addition_operator term } .
// addition_operator= '+' | '-' | OR .
// term= factor { multiplication_operator factor } .
// multiplication_operator= '*' | '/' | DIV | MOD | AND .
// factor= NUMBER | STRING | NIL | CONSTANT_NAME | set
// | variable_access | function_designator
// | '(' expression ')' | NOT factor .
// function_designator= FUNCTION_NAME [ actual_parameter_list ] .
// set= '[' element_list ']' .
// element_list= [ expression { ',' expression } ] .
// statement= [ LABEL ':' ] ( simple_statement | structured_statement ) .
// simple_statement= [ assignment_statement | procedure_statement
// | goto_statement ] .
// assignment_statement= ( variable_access | FUNCTION_NAME ) ':=' expression .
// procedure_statement= PROCEDURE_NAME [ actual_parameter_list ] .
// goto_statement= GOTO label .
// structured_statement= compound_statement | repetitive_statement
// | conditional_statement | with_statement .
// compound_statement= BEGIN statement_sequence END .
// repetitive_statement= while_statement | repeat_statement
// | for_statement .
// while_statement= WHILE expression DO statement .
// repeat_statement= REPEAT statement_sequence UNTIL expression .
// for_statement= FOR VARIABLE_NAME ':=' initial_expression
// ( TO | DOWNTO ) final_expression DO statement .
// initial_expression= expression .
// final_expression= expression .
// conditional_statement= if_statement | case_statement .
// if_statement= IF expression THEN statement [ ELSE statement ] .
// case_statement= CASE expression OF case_element { ';' case_element }
// [ ';' ] END .
// case_element= case_label_list ':' statement .
// with_statement= WITH variable_access { ',' variable_access } DO
// statement 

class Parser{
  Lexer lexer;
  std::string program_name;
  
void program(); // program := PROGRAM ID ';' block '.'
std::vector<std::string> id_list(); // id_list := NAME { ',' NAME }
void block();   // block := declaration_part statement_part
void declaration_part(); // declaration_part := { label_declaration_part | constant_definition_part |
// | type_definition_part | variable_declaration_part
// | procedure_declaration | function_declaration } .
void label_declaration_part(); // label_declaration_part := LABEL UINT { ',' UINT } ';'
void constant_definition_part(); // constant_definition_part := CONST NAME '=' constant ';'
// { NAME '=' constant ';' }
Const constant(); // constant= [ '+' | '-' ] ( CONSTANT_NAME | NUMBER ) | STRING
void type_definition_part(); // type_definition_part := TYPE NAME '=' type ';' { NAME '=' type ';' }
std::shared_ptr<Type> type(); // type := simple_type | structured_type | pointer_type 
//| procedure_type | function_type | TYPE_NAME
std::shared_ptr<Type> simple_type(); // simple_type := subrange_type | enumerated_type
std::shared_ptr<SubrangeType> subrange_type(); // subrange_type := constant '..' constant
std::shared_ptr<EnumType> enumerated_type();       // enumerated_type := '(' id_list ')'
std::shared_ptr<Type> structured_type();          // structured_type := [ PACKED ] unpacked_structured_type
std::shared_ptr<Type> unpacked_structured_type(); // unpacked_structured_type := array_type | record_type | set_type
// | file_type
std::shared_ptr<ArrayType> array_type(); // array_type := ARRAY '[' ( simple_type | TYPE_NAME) { ',' ( simple_type | TYPE_NAME) } ']' OF
// type
std::shared_ptr<RecordType> record_type(); // record_type := RECORD field_list END
void field_list(Attributes&);  // field_list := ( fixed_part [ ';' variant_part ] | variant_part )
void fixed_part(Attributes&); // fixed_part := id_list ':' type { ';' id_list ':' type }
void variant_part(Attributes&); // variant_part := CASE [NAME ':'] TYPE_NAME OF variant { ';' variant }
void variant(Attributes&);   // variant := case_label_list ':' '(' field_list ')'
std::vector<Const> case_label_list(); // case_label_list := constant { ',' constant }
std::shared_ptr<SetType> set_type();        // set_type := SET OF ( simple_type | TYPE_NAME)
std::shared_ptr<FileType> file_type();       // file_type := FILE [ OF type ]
std::shared_ptr<PointerType> pointer_type();    // pointer_type := '^' TYPE_NAME
std::shared_ptr<FunctionType> procedure_type();  // procedure_type := PROCEDURE [ formal_parameter_list ]
std::shared_ptr<FunctionType> function_type();   // function_type := FUNCTION [ formal_parameter_list ] ':' TYPE_NAME
std::vector<Arg> formal_parameter_list(); // formal_parameter_list := '(' formal_parameter_section
// { ';' formal_parameter_section } ')'
void formal_parameter_section(std::vector<Arg> &args); // formal_parameter_section := [ VAR ] id_list ':' TYPE_NAME
void variable_declaration_part(); // variable_declaration_part := VAR id_list ':' type ';'
// { id_list ':' type ';' }
void procedure_declaration(); // procedure_declaration := PROCEDURE NAME [ formal_parameter_list ] ';' block .
void function_declaration();  // function_declaration := FUNCTION NAME [ formal_parameter_list ] ':' TYPE_NAME ';' block .
void statement_part();  // statement_part := BEGIN statement_sequence END
void statement_sequence(); // statement_sequence := statement { ';' statement }
void statement(); // statement := [ LABEL_ID ':' ] ( simple_statement | structured_statement )
void simple_statement(); // simple_statement := [ assignment_statement | procedure_statement
// | goto_statement ]
void assignment_statement(); // assignment_statement := ( variable_access | FUNCTION_NAME | PROCEDURE_NAME ) ':=' expression
std::shared_ptr<Type> variable_access(); // variable_access := ACCESS_NAME { array_access_ | record_access_  | function_parameters_ }
std::shared_ptr<Type> array_access_(std::shared_ptr<Type>); // array_access_ := '[' expression { ',' expression } ']'
std::shared_ptr<Type> expression(); // expression := simple_expression [ relational_operator simple_expression ]
// relational_operator= '=' | '<>' | '<' | '<=' | '>' | '>='
std::shared_ptr<Type> simple_expression(); // simple_expression := [ '+' | '-' ] term { addition_operator term }
// addition_operator := '+' | '-' | OR
std::shared_ptr<Type> term(); // term := factor { multiplication_operator factor }
// multiplication_operator := '*' | '/' | DIV | AND .
std::shared_ptr<Type> factor(); // factor := NUMBER | STRING | NIL | CONSTANT_NAME | ENUM_VALUE
// | variable_access | function_designator
// | '(' expression ')' | NOT factor
void function_designator();                    // function_designator := FUNCTION_NAME [ '(' actual_parameter { ',' actual_parameter } ')' ]
std::shared_ptr<Type> actual_parameter_list(); // actual_parameter_list := '(' actual_parameter { ',' actual_parameter } ')'
std::shared_ptr<Type> actual_parameter();      // actual_parameter := expression | variable_access | PROCEDURE_NAME
// | FUNCTION_NAME .
std::shared_ptr<Type> record_access_(std::shared_ptr<Type> t); // record_access_ := '.' ATTRIBUTE_NAME
void function_parameters_(); // function_parameters_ := '(' [ expression { ',' expression } ] ')'
void procedure_statement();  // procedure_statement := ((PROCEDURE_NAME | 'write')  [ '(' actual_parameter { ',' actual_parameter } ')' ])
// | 'read' '(' variable_access {',' variable_access } ')'
void goto_statement(); // goto_statement := GOTO NUMBER
void structured_statement(); // structured_statement := compound_statement | repetitive_statement
// | conditional_statement .
void compound_statement(); // compound_statement := BEGIN statement_sequence END
void repetitive_statement(); // // repetitive_statement := while_statement | repeat_statement
// | for_statement .
void while_statement(); // while_statement := WHILE expression DO statement
void repeat_statement(); // repeat_statement := REPEAT statement_sequence UNTIL expression
void for_statement(); // for_statement := FOR variable_access ':=' expression
// ( TO | DOWNTO ) expression DO statement
void conditional_statement(); // conditional_statement := if_statement | case_statement
void if_statement(); // if_statement := IF expression THEN statement [ ELSE statement ]
void case_statement(); // case_statement := CASE expression OF case_label_list ':' statement { ';' case_label_list ':' statement }
// END
void case_element(); // case_element := case_label_list ':' statement

void match_adv(TOKEN_TYPE type);
void match(TOKEN_TYPE type);

void matches(const std::initializer_list<TOKEN_TYPE>& l);
void matches_adv(const std::initializer_list<TOKEN_TYPE>& l);

Label& get_label(Int id);
void quit_if_label_id_used(Int id);

void quit_if_id_is_used(const std::string& id);

std::shared_ptr<Const> get_constant(const std::string& id);
std::shared_ptr<Var> get_variable(const std::string& id);
std::shared_ptr<Var> get_function(const std::string &id);
std::shared_ptr<Type> get_type(const std::string& id);

inline std::string name_subrange(Int lower,Int upper){
  return "_sub_" + std::to_string(lower) + "_" + std::to_string(upper);
}
inline std::string name_subrange(char lower,char upper){
  return "_sub_" + std::string(1, lower) + "_" + std::string(1, upper);
}

std::shared_ptr<SubrangeType> get_subrange(Int lower,Int upper);
std::shared_ptr<SubrangeType> get_subrange(char lower,char upper);
std::shared_ptr<EnumType> get_enum(std::vector<std::string> &&v);
std::shared_ptr<EnumType> get_enum_value(const std::string &name);
std::shared_ptr<ArrayType> get_array(
  const std::vector<std::shared_ptr<Type>>& indexTypes,
  std::shared_ptr<Type> valueType
);
std::shared_ptr<SetType> get_set(std::shared_ptr<Type> valueType);
std::shared_ptr<FileType> get_file(std::shared_ptr<Type> valueType);
std::shared_ptr<PointerType> get_pointer(std::shared_ptr<Type> valueType);
std::shared_ptr<RecordType> get_record(
  std::unordered_map<std::string,Var>&& attributes
);
std::shared_ptr<FunctionType> get_function_type(const std::vector<Arg>& args,std::shared_ptr<Type> returnType);
void show_declarations();
void print_type(std::shared_ptr<Type> t,const std::string &name = "");

void store_constants();
void store_variables();
uint store_args_variables();
uint store_variable(std::shared_ptr<Type> t);
void check_store_comparison(std::shared_ptr<Type> a,std::shared_ptr<Type> b);
void check_store_sum(std::shared_ptr<Type> a, std::shared_ptr<Type> b,TOKEN_TYPE tt);
void check_store_mul(std::shared_ptr<Type> a, std::shared_ptr<Type> b, TOKEN_TYPE tt);
void assign_var(std::shared_ptr<Type> a, std::shared_ptr<Type> b);

public:
  std::deque<Info> infos;
  VM vm;
  Parser(Lexer);
  Lexer& getLexer();
  void parse();
};
#endif