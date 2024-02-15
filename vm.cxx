#include "vm.hxx"

size_t VM::add_data(uint data){
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
    switch(bytecode[pc]){
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
      case WRITE_OP: write_uint(); break;
      case READ_OP: read_uint(); break;
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
  uint addr = bytecode[pc];
  Print(addr);
  bytecode.emplace_back(bytecode[addr]);
  Println(" (",bytecode[addr],")");
  ++pc;
}
// push address
void VM::push_addr_op(){
  Print("PUSH_ADDR ");
  ++pc;
  uint addr = bytecode[pc];
  Print(addr);
  bytecode.emplace_back(addr);
  Println(" (", bytecode[addr], ")");
  ++pc;
}
// push variable
void VM::push_const_op(){
  Print("PUSH_CONST ");
  ++pc;
  uint operand = bytecode[pc];
  Println(operand);
  bytecode.emplace_back(operand);
  ++pc;
}
// jump to address
void VM::jmp_op(){
  Print("JMP ");
  ++pc;
  uint addr = bytecode[pc];
  Println(addr);
  pc = addr;
}
// move source to destination address
void VM::mov_op(){
  Print("MOV ");
  uint dest = bytecode[bytecode.size()-1];
  bytecode.pop_back();
  uint src = bytecode[bytecode.size()-1];
  bytecode.pop_back();
  Println(src," -> ",dest);
  bytecode[dest] = src;
  ++pc;
}
// why though ?
void VM::pop_op(){
  Print("POP ");
  Println(bytecode[bytecode.size()-1]);
  bytecode.pop_back();
  ++pc;
}
// write operand 
void VM::write_uint(){
  Print("WRITE ");
  uint value = bytecode[bytecode.size()-1];
  bytecode.pop_back();
  Println(value);
  ++pc;
}
// read to operand address 
void VM::read_uint(){
  Print("READ ");
  uint addr = bytecode[bytecode.size()-1];
  bytecode.pop_back();
  Println(addr);
  std::cin >> bytecode[addr];
  ++pc;
}
// add 2 operands and push result
void VM::addu_op(){
  Print("ADDU ");
  uint b = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  Println(a," + ",b);
  bytecode.emplace_back(a+b);
  ++pc;
}
// substract 2 operands and push result
void VM::subu_op(){
  Print("SUBU ");
  uint b = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  Println(a," - ",b);
  bytecode.emplace_back(a-b);
  ++pc;
}
// multiply 2 operands and push result
void VM::mulu_op(){
  Print("MULU ");
  uint b = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  Println(a," * ",b);
  bytecode.emplace_back(a*b);
  ++pc;
}
// divide 2 operands and push result
void VM::divu_op(){
  Print("DIVU ");
  uint b = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  uint a = bytecode[bytecode.size() - 1];
  bytecode.pop_back();
  Println(a," / ",b);
  bytecode.emplace_back(a/b);
  ++pc;
}

