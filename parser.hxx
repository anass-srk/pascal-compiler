#ifndef PARSER_HXX
#define PARSER_HXX
#include "lexer.hxx"
#include <vector>
#include <initializer_list>

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
void id_list(); // id_list := NAME { ',' NAME }
void block();   // block := declaration_part statement_part
void declaration_part(); // declaration_part := { label_declaration_part | constant_definition_part |
// | type_definition_part | variable_declaration_part
// | procedure_declaration | function_declaration } .
void label_declaration_part(); // label_declaration_part := LABEL UINT { ',' UINT } ';'
void constant_definition_part(); // constant_definition_part := CONST NAME '=' constant ';'
// { NAME '=' constant ';' }
void constant(); // constant= [ '+' | '-' ] ( CONSTANT_NAME | NUMBER ) | STRING
void type_definition_part(); // type_definition_part := TYPE NAME '=' type ';' { NAME '=' type ';' }
void type();                 // type := simple_type | structured_type | pointer_type 
//| procedure_type | function_type | TYPE_NAME
void simple_type(); // simple_type := subrange_type | enumerated_type
void subrange_type(); // subrange_type := constant '..' constant
void enumerated_type(); // enumerated_type := '(' id_list ')'
void structured_type(); // structured_type := [ PACKED ] unpacked_structured_type
void unpacked_structured_type(); //unpacked_structured_type := array_type | record_type | set_type
// | file_type
void array_type(); // array_type := ARRAY '[' simple_type { ',' simple_type } ']' OF
// type
void record_type(); // record_type := RECORD field_list END
void field_list();  // field_list := ( fixed_part [ ';' variant_part ] | variant_part )
void fixed_part();  // fixed_part := id_list ':' type { ';' id_list ':' type }
void variant_part();   // variant_part := CASE [NAME ':'] TYPE_NAME OF variant { ';' variant }
void variant();        // variant := case_label_list ':' '(' field_list ')'
void case_label_list(); // case_label_list := constant { ',' constant }
void set_type();        // set_type := SET OF type
void file_type();       // file_type := FILE [ OF type ]
void pointer_type();    // pointer_type := '^' TYPE_NAME
void procedure_type();  // procedure_type := PROCEDURE [ formal_parameter_list ] 
void function_type();   // function_type := FUNCTION [ formal_parameter_list ] ':' TYPE_NAME
void formal_parameter_list(); // formal_parameter_list := '(' formal_parameter_section
// { ';' formal_parameter_section } ')'
void formal_parameter_section(); // formal_parameter_section := [ VAR ] id_list ':' TYPE_NAME
void variable_declaration_part(); // variable_declaration_part := VAR id_list ':' type ';'
// { id_list ':' type ';' }
void procedure_declaration(); // procedure_declaration := PROCEDURE NAME [ formal_parameter_list ] ';' block .
void function_declaration();  // function_declaration := FUNCTION NAME [ formal_parameter_list ] ':' TYPE_NAME ';' block .
void statement_part();  // statement_part := BEGIN statement_sequence END
void statement_sequence(); // statement_sequence := statement { ';' statement }
void statement(); // statement := [ LABEL ':' ] ( simple_statement | structured_statement )
void simple_statement(); // simple_statement := [ assignment_statement | procedure_statement
// | goto_statement ]
void assignment_statement(); // assignment_statement := ( variable_access | FUNCTION_NAME ) ':=' expression
void variable_access();      // variable_access := ACCESS_NAME { end_access_ }
void end_access_(); // end_access_ := { array_access_ | record_access_ | '^' | function_parameters_ }
void array_access_(); // array_access_ := '[' expression_list ']'
void expression_list(); // expression_list := expression { ',' expression }
void expression(); // expression := simple_expression [ relational_operator simple_expression ]
// relational_operator= '=' | '<>' | '<' | '<=' | '>' | '>=' | IN 
void simple_expression(); // simple_expression := [ '+' | '-' ] term { addition_operator term }
// addition_operator := '+' | '-' | OR 
void term(); // term := factor { multiplication_operator factor }
// multiplication_operator := '*' | '/' | DIV | MOD | AND .
void factor(); // factor := NUMBER | STRING | NIL | CONSTANT_NAME | set
// | variable_access | function_designator
// | '(' expression ')' | NOT factor 
void set(); // set := '[' element_list ']' 
void element_list(); // element_list := [ expression { ',' expression } ]
void function_designator(); // function_designator := FUNCTION_NAME [ actual_parameter_list ]
void actual_parameter_list(); // actual_parameter_list := '(' actual_parameter { ',' actual_parameter } ')'
void actual_parameter(); // actual_parameter := expression | variable_access | PROCEDURE_NAME
// | FUNCTION_NAME .
void record_access_(); // record_access_ := '.' variable_access
void function_parameters_(); // function_parameters_ := '(' [ expression_list ] ')'
void procedure_statement();  // procedure_statement := PROCEDURE_NAME [ actual_parameter_list ]
void goto_statement(); // goto_statement := GOTO NUMBER
void structured_statement(); // structured_statement := compound_statement | repetitive_statement
// | conditional_statement | with_statement .
void compound_statement(); // compound_statement := BEGIN statement_sequence END
void repetitive_statement(); // // repetitive_statement := while_statement | repeat_statement
// | for_statement .
void while_statement(); // while_statement := WHILE expression DO statement
void repeat_statement(); // repeat_statement := REPEAT statement_sequence UNTIL expression
void for_statement(); // for_statement := FOR VARIABLE_NAME ':=' expression
// ( TO | DOWNTO ) expression DO statement
void conditional_statement(); // conditional_statement := if_statement | case_statement
void if_statement(); // if_statement := IF expression THEN statement [ ELSE statement ]
void case_statement(); // case_statement := CASE expression OF case_element { ';' case_element }
//  [';' ELSE statement_sequence ] END
void case_element(); // case_element := case_label_list ':' statement
void with_statement(); // with_statement := WITH variable_access { ',' variable_access } DO
// statement

void match_adv(TOKEN_TYPE type);
void match(TOKEN_TYPE type);

void matches(const std::initializer_list<TOKEN_TYPE>& l);
void matches_adv(const std::initializer_list<TOKEN_TYPE>& l);

public:
  Parser(Lexer);
  Lexer& getLexer();
  void parse();
};
#endif