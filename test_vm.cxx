#include "vm.hxx"

/*
the code is 
*/

int main(){
  VM vm;
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint var1 = vm.add_data(10);
  vm.bytecode[loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(var1);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(1);
  vm.add_inst(CMPI_OP);

  vm.add_inst(JMPEQ_OP);
  size_t end = vm.add_data(0);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(WRITE_OP);
  
  vm.bytecode[end] = (uint)vm.bytecode.size();

  vm.add_inst(HALT_OP);
  vm.run();
}