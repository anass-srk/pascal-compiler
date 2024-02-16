#include "vm.hxx"

/*
the code is 
*/
void code1(VM& vm);

int main(){
  VM vm;
  // vm.load_from_file("prog.bin");
  code1(vm);
  vm.run();
  // vm.save_to_file("prog.bin");
}

// Reads an int and shows it only if it's not equal to 1. then the int is set to 7 and shown
void code1(VM& vm){
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

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(7);
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(var1);
  vm.add_inst(MOV_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(WRITE_OP);

  vm.add_inst(HALT_OP);
}