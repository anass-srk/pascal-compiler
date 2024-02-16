#ifndef VM_HXX
#define VM_HXX
#include "lexer.hxx"
#include <deque>
#include <vector>
#include <numeric>

constexpr bool debug = true;

template <printable... Args>
void Print(Args &&...args){
  if constexpr (debug){
    (std::cout << ... << args);
  }
}

template <typename... Args>
void Println(Args &&...args){
  if constexpr (debug){
    (std::cout << ... << args) << '\n';
  }
};

enum OP_CODE{
  HALT_OP,
  JMP_OP,

  ADDI_OP,
  SUBI_OP,
  MULI_OP,
  DIVI_OP,
  
  ADDU_OP,
  SUBU_OP,
  MULU_OP,
  DIVU_OP,
  
  ADDF_OP,
  SUBF_OP,
  MULF_OP,
  DIVF_OP,
  
  ADDC_OP,
  SUBC_OP,
  MULC_OP,
  DIVC_OP,
  
  ADDB_OP,
  SUBB_OP,
  MULB_OP,
  DIVB_OP,

  MOV_OP,
  PUSH_OP,
  PUSH_CONST_OP,
  PUSH_ADDR_OP,
  POP_OP,

  WRITE_OP,
  READ_OP,

  JMPLT_OP,
  JMPLE_OP,
  JMPEQ_OP,
  JMPNQ_OP,
  JMPGT_OP,
  JMPGE_OP,

  CMPI_OP,
  CMPU_OP,
  CMPF_OP,
  CMPB_OP,
  CMPC_OP
};

union Address{
  uint u[2];
  size_t s;
};

enum STD_TYPE {
  UINT_STD,
  INT_STD,
  REAL_STD,
  UCHAR_STD,
  CHAR_STD
};

union StdType{
  uint u;
  int i;
  float f;
  u_char b;
  char c;
  StdType(int i) : i(i){}
  StdType(uint u) : u(u){}
  StdType(float f) : f(f){}
  StdType(u_char b) : b(b){}
  StdType(char c) : c(c){}
};

static const std::string std_type_names[] = {
  "UINT",
  "INT",
  "REAL",
  "UCHAR",
  "CHAR"
};

static const std::string flag_names[] = {
  "<",
  "=",
  ">"
};

enum FLAG{
  LT_FLAG,
  EQ_FLAG,
  GT_FLAG
};

struct VM{
  std::vector<StdType> bytecode;
  std::size_t pc;
  FLAG flag;

  size_t add_data(StdType data);
  size_t add_inst(OP_CODE instr);
  void run();

  void halt_op();
  void push_op();
  void jmp_op();
  void mov_op();
  void push_addr_op();
  void push_const_op();
  void pop_op();
  void write_op();
  void read_op();

  void addu_op();
  void subu_op();
  void mulu_op();
  void divu_op();

  void addi_op();
  void subi_op();
  void muli_op();
  void divi_op();

  void addf_op();
  void subf_op();
  void mulf_op();
  void divf_op();

  void addb_op();
  void subb_op();
  void mulb_op();
  void divb_op();

  void addc_op();
  void subc_op();
  void mulc_op();
  void divc_op();

  void cmpi_op();
  void cmpu_op();
  void cmpf_op();
  void cmpb_op();
  void cmpc_op();

  void jmple_op();
  void jmplt_op();
  void jmpeq_op();
  void jmpnq_op();
  void jmpgt_op();
  void jmpge_op();
};

#endif