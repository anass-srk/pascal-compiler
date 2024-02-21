#ifndef SEMANTICS_HXX
#define SEMANTICS_HXX
#include "lexer.hxx"
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <tuple>

enum STD_TYPE{
  INT_TYPE,
  UINT_TYPE,
  REAL_TYPE,
  BOOLEAN_TYPE,
  CHAR_TYPE,
  CONST_STR_TYPE, // For constants only
  ENUM_TYPE,
  SUBRANGE_TYPE,
  ARRAY_TYPE,
  RECORD_TYPE,
  SET_TYPE,
  FILE_TYPE,
  POINTER_TYPE,
  FUNCTION_TYPE,
  VOID_TYPE
};

struct Type{
  std::string name = "";
  STD_TYPE type;
  uint size = 1;
  virtual ~Type() = default;
  Type(STD_TYPE t) : type(t){}
};

struct EnumType : Type{
  std::unordered_map<std::string,Int> ids;
  uint amount = 1;
  EnumType() : Type(ENUM_TYPE) {}
};

struct SubrangeType : Type{
  std::shared_ptr<Type> boundsType;
  uint amount = 1;
  union{
    Int ibounds[2];
    char cbounds[2];
  };
  SubrangeType() : Type(SUBRANGE_TYPE) {}
};

struct ArrayType : Type{
  std::vector<std::shared_ptr<Type>> indexTypes;
  std::shared_ptr<Type> valueType;
  uint amount = 0;
  uint elem_size = 1;
  ArrayType() : Type(ARRAY_TYPE) {}
};

struct Const{
  STD_TYPE type;
  std::variant<Int,double,std::string> value;
  uint loc = 0;
};

struct Var{
  std::shared_ptr<Type> type;
  uint loc = 0;
  Var(std::shared_ptr<Type> type,uint loc = 0) : type(type) , loc(loc) {}
  Var(){}
};

using Attributes = std::unordered_map<std::string, Var>;
struct RecordType : Type{
  Attributes attributes;
  RecordType() : Type(RECORD_TYPE){}
};

struct SetType : Type{
  std::shared_ptr<Type> valueType;
  SetType() : Type(SET_TYPE){}
};

struct FileType : Type{
  std::shared_ptr<Type> valueType;
  FileType() : Type(FILE_TYPE){}
};

struct PointerType : Type{
  std::shared_ptr<Type> valueType;
  PointerType() : Type(POINTER_TYPE) {}
};

struct Arg{
  std::string id;
  std::shared_ptr<Type> type;
  bool byRef;
};

struct FunctionType : Type{
  std::vector<Arg> args;
  std::shared_ptr<Type> returnType;
  FunctionType() : Type(FUNCTION_TYPE){}
};

struct Label {
  uint loc;
};

struct Function{
  std::vector<Arg> args;
  std::shared_ptr<Type> returnType;
  Function(std::vector<Arg>&& args,std::shared_ptr<Type> returnType) : args(args), returnType(returnType) {} 
};

struct Info{
  std::unordered_map<Int,Label> labels;
  std::unordered_map<std::string,std::shared_ptr<Const>> constants;
  std::unordered_map<std::string,std::shared_ptr<Type>> types;
  std::unordered_map<std::string,std::shared_ptr<Var>> variables;
  std::unordered_map<std::string, std::shared_ptr<Var>> arg_variables;
  std::vector<std::string> args_order;
  std::unordered_map<std::string,std::shared_ptr<Var>> functions;
};

#endif