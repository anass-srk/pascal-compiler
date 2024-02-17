#include "vm.hxx"

/*
the code is 
*/
void code1(VM& vm);
void code2(VM& vm);
void code3(VM& vm);

int main(){
  VM vm;
  // vm.load_from_file("prog.bin");
  code3(vm);
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


// Asks you for your name and then says Hello
void code2(VM &vm){
  vm.add_inst(STORE_NSTD_OP);
  uint name = vm.add_data(STRING_STD);
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint wmsg = vm.write_const_string("What's your name ?");
  uint hmsg = vm.write_const_string("Hello ");
  vm.bytecode[loc] = (uint)vm.bytecode.size();
  
  vm.add_inst(PUSHS_OP);
  vm.add_data(wmsg);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(WRITE_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(name);
  vm.add_inst(MOVS_OP);

  vm.add_inst(PUSHS_OP);
  vm.add_data(hmsg);
  vm.add_inst(PUSHS_OP);
  vm.add_data(name);
  vm.add_inst(ADDS_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(WRITE_OP);

  vm.add_inst(HALT_OP);
}
//prompts user for a number i, then displays Hello i-times
void code3(VM &vm){
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint prompt = vm.write_const_string("Write a number :\n");
  uint msg = vm.write_const_string("Hello\n");
  uint i = vm.add_data(0);
  vm.bytecode[loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSHS_OP);
  vm.add_data(prompt);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(WRITE_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(UINT_STD);
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(i);
  vm.add_inst(READ_OP);

  uint beg_loop = vm.add_inst(PUSH_OP);
  vm.add_data(i);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(1);
  vm.add_inst(CMPU_OP);

  vm.add_inst(JMPLT_OP);
  uint end_loop = vm.add_data(0);

  vm.add_inst(PUSH_OP);
  vm.add_data(i);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(1);
  vm.add_inst(SUBU_OP);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(i);
  vm.add_inst(MOV_OP);

  vm.add_inst(PUSHS_OP);
  vm.add_data(msg);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(WRITE_OP);

  vm.add_inst(JMP_OP);
  vm.add_data(beg_loop);
  vm.bytecode[end_loop] = (uint)vm.bytecode.size();

  vm.add_inst(HALT_OP);
}
