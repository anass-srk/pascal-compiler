#ifndef VM_HXX
#define VM_HXX
#include "lexer.hxx"
#include <deque>
#include <vector>
#include <numeric>
#include <stack>

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
  RET_OP,
  RET_BASIC_OP,

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

  ADDS_OP,

  MOV_OP,
  MOVS_OP,
  PUSH_OP,
  PUSHS_OP,
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
  CMPC_OP,
  CMPS_OP,

  STORE_COMPLEX_OP, // stores complex types (string...) 
};

union Address{
  uint u[2];
  size_t s;
};

enum VM_STD_TYPE {
  UINT_STD,
  INT_STD,
  REAL_STD,
  UCHAR_STD,
  CHAR_STD,
  STRING_STD
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
  "CHAR",
  "STRING"
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

class VM{
public:
  std::vector<StdType> bytecode;
  std::size_t pc;
  FLAG flag;
  std::unordered_map<uint,std::string> strings;
  std::stack<std::string> string_stack;

  size_t add_data(StdType data);
  size_t add_inst(OP_CODE instr);
  void run();
  void load_from_file(const std::string& src);
  void save_to_file(const std::string& dest);
private:
  void halt_op();
  void push_op();
  void jmp_op();
  void mov_op();
  void push_addr_op();
  void push_const_op();
  void pop_op();

  void write_op();
  void read_op();
public:
  uint write_const_string(const std::string& s);
private:
  // cell taken by storing the length is not taken into consideration
  uint get_const_string_taken_cells(size_t len);
  std::string read_const_string(uint address);
  void pushs_op();
  void adds_op();
  void movs_op();

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

  void store_complex_op();

  void ret_op();
  void ret_basic_op();
};

#endif