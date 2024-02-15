#include "vm.hxx"

/*
the code is 
*/

int main(){
  VM vm;
  vm.add_inst(JMP_OP);
  size_t loc = vm.add_data(0);
  size_t var1 = vm.add_data(20);
  vm.bytecode[loc] = vm.bytecode.size();
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(var1);
  vm.add_inst(READ_OP);
  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(WRITE_OP);
  vm.add_inst(HALT_OP);
  vm.run();
}