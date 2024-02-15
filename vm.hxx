#ifndef VM_HXX
#define VM_HXX
#include "lexer.hxx"
#include <deque>
#include <vector>

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
  READ_OP
};

union Address{
  uint u[2];
  size_t s;
};

union StdType{
  int i;
  uint u;
  float d;
  char c;
  bool b;
};

struct VM{
  std::vector<uint> bytecode;
  std::size_t pc;

  size_t add_data(uint data);
  size_t add_inst(OP_CODE instr);
  void run();

  void halt_op();
  void push_op();
  void jmp_op();
  void mov_op();
  void push_addr_op();
  void push_const_op();
  void pop_op();
  void write_uint();
  void read_uint();
  void addu_op();
  void subu_op();
  void mulu_op();
  void divu_op();
};


#endif