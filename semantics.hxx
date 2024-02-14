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
  virtual ~Type() = default;
  Type(STD_TYPE t) : type(t){}
};

struct EnumType : Type{
  std::unordered_map<std::string,Int> ids;
  EnumType() : Type(ENUM_TYPE) {}
};

struct SubrangeType : Type{
  STD_TYPE boundsType;
  union{
    Int ibounds[2];
    char cbounds[2];
  };
  SubrangeType() : Type(SUBRANGE_TYPE) {}
};

struct ArrayType : Type{
  std::vector<std::shared_ptr<Type>> indexTypes;
  std::shared_ptr<Type> valueType;
  ArrayType() : Type(ARRAY_TYPE) {}
};

struct Const{
  STD_TYPE type;
  std::variant<Int,double,std::string> value;
};

struct RecordType : Type {
  std::unordered_map<std::string,std::shared_ptr<Type>> attributes;
};

struct SetType : Type{
  STD_TYPE type;
};

struct FileType : Type{
  std::shared_ptr<Type> type;
};

using PointerType = FileType;

struct Arg{
  std::string id;
  std::shared_ptr<Type> type;
  bool byRef;
};

struct FunctionType : Type{
  std::vector<Arg> args;
  std::shared_ptr<Type> type;
};

struct Label {
  std::size_t loc;
};

struct Function{
  std::vector<Arg> args;
  std::shared_ptr<Type> type;
  std::size_t loc;
};

struct Info{
  std::unordered_map<Int,Label> labels;
  std::unordered_map<std::string,Const> constants;
  std::unordered_map<std::string,std::shared_ptr<Type>> types;
  std::unordered_map<std::string,std::shared_ptr<Type>> variables; 
  std::unordered_map<std::string,Function> functions;
};

#endif