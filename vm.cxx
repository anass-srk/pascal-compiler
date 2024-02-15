#include "vm.hxx"

size_t VM::add_data(StdType data){
  size_t const loc = bytecode.size();
  bytecode.emplace_back(data);
  return loc;
}

size_t VM::add_inst(OP_CODE instr){
  size_t const loc = bytecode.size();
  bytecode.emplace_back(instr);
  return loc;
}

void VM::run(){
  pc = 0;
  while(pc < bytecode.size()){
    switch(bytecode[pc].u){
      case HALT_OP: halt_op(); break;
      case PUSH_OP: push_op(); break;
      case PUSH_CONST_OP: push_const_op(); break;
      case PUSH_ADDR_OP: push_addr_op(); break;
      case JMP_OP: jmp_op(); break;
      case MOV_OP: mov_op(); break;

      case ADDU_OP: addu_op(); break;
      case SUBU_OP: subu_op(); break;
      case MULU_OP: mulu_op(); break;
      case DIVU_OP: divu_op(); break;
      
      case ADDI_OP: addi_op(); break;
      case SUBI_OP: subi_op(); break;
      case MULI_OP: muli_op(); break;
      case DIVI_OP: divi_op(); break;

      case ADDF_OP: addf_op(); break;
      case SUBF_OP: subf_op(); break;
      case MULF_OP: mulf_op(); break;
      case DIVF_OP: divf_op(); break;

      case ADDB_OP: addb_op(); break;
      case SUBB_OP: subb_op(); break;
      case MULB_OP: mulb_op(); break;
      case DIVB_OP: divb_op(); break;

      case ADDC_OP: addc_op(); break;
      case SUBC_OP: subc_op(); break;
      case MULC_OP: mulc_op(); break;
      case DIVC_OP: divc_op(); break;

      case WRITE_OP: write_op(); break;
      case READ_OP: read_op(); break;
      default:
        println("unknown|not-inemplemented bytecode ?");
        exit(EXIT_SUCCESS);
    }
  }
}

void VM::halt_op(){
  exit(EXIT_SUCCESS);
}
// push variable using its address
void VM::push_op(){
  Print("PUSH ");
  ++pc;
  uint addr = bytecode[pc].u;
  Print(addr);
  bytecode.emplace_back(bytecode[addr]);
  Println(" (",bytecode[addr].u,")");
  ++pc;
}
// push address
void VM::push_addr_op(){
  Print("PUSH_ADDR ");
  ++pc;
  uint addr = bytecode[pc].u;
  Print(addr);
  bytecode.emplace_back(addr);
  Println(" (", bytecode[addr].u, ")");
  ++pc;
}
// push variable
void VM::push_const_op(){
  Print("PUSH_CONST ");
  ++pc;
  uint operand = bytecode[pc].u;
  Println(operand);
  bytecode.emplace_back(operand);
  ++pc;
}
// jump to address
void VM::jmp_op(){
  Print("JMP ");
  ++pc;
  uint addr = bytecode[pc].u;
  Println(addr);
  pc = addr;
}
// move source to destination address
void VM::mov_op(){
  Print("MOV ");
  uint dest = bytecode[bytecode.size()-1].u;
  bytecode.pop_back();
  uint src = bytecode[bytecode.size()-1].u;
  bytecode.pop_back();
  Println(src," -> ",dest);
  bytecode[dest].u = src;
  ++pc;
}
// why though ?
void VM::pop_op(){
  Print("POP ");
  Println(bytecode[bytecode.size()-1].u);
  bytecode.pop_back();
  ++pc;
}
// write operand 
void VM::write_op(){
  Print("WRITE ");
  StdType value = bytecode[bytecode.size()-1];
  bytecode.pop_back();
  STD_TYPE t = static_cast<STD_TYPE>(bytecode[bytecode.size() - 1].u);
  bytecode.pop_back();
  switch(t){
    case INT_STD: println(value.i); break;
    case REAL_STD: println(value.f); break;
    case UINT_STD: println(value.u); break;
    case UCHAR_STD: println(value.c); break;
    case CHAR_STD: println(value.c); break;
    default:
      println("write not fully implemented !");
      exit(EXIT_FAILURE);
      break;
  }
  ++pc;
}
// read to operand address 
void VM::read_op(){
  Print("READ ");
  uint addr = bytecode[bytecode.size()-1].u;
  bytecode.pop_back();
  STD_TYPE t = static_cast<STD_TYPE>(bytecode[bytecode.size() - 1].u);
  bytecode.pop_back();
  Println("Type (", std_type_names[t], ") ",addr);
  switch(t){
    case INT_STD: std::cin >> bytecode[addr].i; break;
    case REAL_STD: std::cin >> bytecode[addr].f; break;
    case UINT_STD: std::cin >> bytecode[addr].u; break;
    case UCHAR_STD: std::cin >> bytecode[addr].b; break;
    case CHAR_STD: std::cin >> bytecode[addr].c; break;
    default:
      println("read not fully implemented !");
      exit(EXIT_FAILURE);
      break;
  }
  ++pc;
}

/*********************************************************************/

// add 2 operands and push result
void VM::addu_op(){
  Print("ADDU ");
  uint b = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subu_op(){
  Print("SUBU ");
  uint b = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::mulu_op(){
  Print("MULU ");
  uint b = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divu_op(){
  Print("DIVU ");
  uint b = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1].u;
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}

/*********************************************************************/

// add 2 operands and push result
void VM::addi_op(){
  Print("ADDI ");
  int b = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  int a = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subi_op(){
  Print("SUBI ");
  int b = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  int a = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::muli_op(){
  Print("MULI ");
  int b = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  int a = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divi_op(){
  Print("DIVI ");
  int b = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  int a = bytecode[bytecode.size() - 1].i;
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}

/*********************************************************************/

// add 2 operands and push result
void VM::addf_op(){
  Print("ADDF ");
  float b = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  float a = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subf_op(){
  Print("SUBF ");
  float b = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  float a = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::mulf_op(){
  Print("MULF ");
  float b = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  float a = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divf_op(){
  Print("DIVF ");
  float b = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  float a = bytecode[bytecode.size() - 1].f;
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}


/*********************************************************************/

// add 2 operands and push result
void VM::addb_op(){
  Print("ADDB ");
  u_char b = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  u_char a = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subb_op(){
  Print("SUBB ");
  u_char b = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  u_char a = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::mulb_op(){
  Print("MULB ");
  u_char b = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  u_char a = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divb_op(){
  Print("DIVB ");
  u_char b = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  u_char a = bytecode[bytecode.size() - 1].b;
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}

/*********************************************************************/

// add 2 operands and push result
void VM::addc_op(){
  Print("ADDC ");
  char b = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  char a = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subc_op(){
  Print("SUBC ");
  char b = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  char a = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::mulc_op(){
  Print("MULc ");
  char b = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  char a = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divc_op(){
  Print("DIVC ");
  char b = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  char a = bytecode[bytecode.size() - 1].c;
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}