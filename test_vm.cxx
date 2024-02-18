#include "vm.hxx"

/*
the code is 
*/
void code1(VM& vm);
void code2(VM& vm);
void code3(VM& vm);
void code4(VM &vm);
void code5(VM& vm);
void code6(VM& vm);

int main(){
  VM vm;
  // vm.load_from_file("prog.bin");
  code6(vm);
  vm.run();
  // vm.save_to_file("prog.bin");
}

// Reads an int and shows it only if it's not equal to 1. then the int is set to 7 and shown
void code1(VM& vm){
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint var1 = vm.add_data(10);
  vm.bytecode[loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(var1);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(1);
  vm.add_inst(CMPI_OP);

  vm.add_inst(JMPEQ_OP);
  size_t end = vm.add_data(0);

  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(WRITE_OP);
  vm.bytecode[end] = (uint)vm.bytecode.size();

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(7);
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(var1);
  vm.add_inst(MOV_OP);

  vm.add_inst(PUSH_OP);
  vm.add_data(var1);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(HALT_OP);
}


// Asks you for your name and then says Hello
void code2(VM &vm){
  uint name = vm.add_inst(STORE_COMPLEX_OP);
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint wmsg = vm.write_const_string("What's your name ?");
  uint hmsg = vm.write_const_string("Hello ");
  vm.bytecode[loc] = (uint)vm.bytecode.size();
  
  vm.add_inst(PUSHS_OP);
  vm.add_data(wmsg);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
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
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
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
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(i);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(UINT_STD);
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
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(JMP_OP);
  vm.add_data(beg_loop);
  vm.bytecode[end_loop] = (uint)vm.bytecode.size();

  vm.add_inst(HALT_OP);
}
// prompts user to enter 2 numbers, calls function to add them and prints result
void code4(VM &vm){
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint prompt = vm.write_const_string("Write 2 numbers :\n");
  uint a = vm.add_data(0);
  uint b = vm.add_data(0);

  uint func = vm.add_inst(JMP_OP);
  uint locf = vm.add_data(0);
  uint arg1 = vm.add_data(0);
  uint arg2 = vm.add_data(0);
  vm.bytecode[locf] = (uint)vm.bytecode.size();

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(arg2);
  vm.add_inst(MOV_OP);
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(arg1);
  vm.add_inst(MOV_OP);

  vm.add_inst(PUSH_OP);
  vm.add_data(arg1);
  vm.add_inst(PUSH_OP);
  vm.add_data(arg2);
  vm.add_inst(ADDI_OP);

  vm.add_inst(RET_BASIC_OP);

  vm.bytecode[loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSHS_OP);
  vm.add_data(prompt);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(STRING_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(a);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(b);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_CONST_OP);
  uint ret_loc = vm.add_data(0);
  vm.add_inst(PUSH_OP);
  vm.add_data(a);
  vm.add_inst(PUSH_OP);
  vm.add_data(b);
  vm.add_inst(JMP_OP);
  vm.add_data(func);
  vm.bytecode[ret_loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(HALT_OP);
}
// a simple string manipulation test
void code5(VM &vm){
  uint str = vm.add_inst(STORE_COMPLEX_OP);
  vm.add_inst(JMP_OP);
  uint loc = vm.add_data(0);
  uint name = vm.write_const_string("Hamid");
  vm.bytecode[loc] = (uint)vm.bytecode.size();

  vm.add_inst(PUSHS_OP);
  vm.add_data(name);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(str);
  vm.add_inst(MOVS_OP);

  vm.add_inst(PUSHS_OP);
  vm.add_data(str);

  vm.add_inst(WRITE_STR_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data('W');
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(str);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(0);
  vm.add_inst(SET_STR_CHAR_OP);

  vm.add_inst(PUSHS_OP);
  vm.add_data(str);
  vm.add_inst(WRITE_STR_OP);

  vm.add_inst(PUSHS_OP);
  vm.add_data(str);
  vm.add_inst(GET_STR_LEN_OP);

  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(UINT_STD);
  vm.add_inst(WRITE_OP);

  vm.add_inst(HALT_OP);
}
/*
int x,y;
read(x,y);
if(x == 1 && x == y){
  write("both numbers are equal to 1\n");
}else{
  write("not all the numbers are equal to 0\n");
}
*/
void code6(VM &vm){
  //Constants
  vm.add_inst(JMP_OP);
  uint beg = vm.add_data(0);
  uint eq = vm.write_const_string("both numbers are equal to 1\n");
  vm.bytecode[beg] = (uint)vm.bytecode.size();
  //vars
  uint x = vm.add_inst(NOP_OP);
  uint y = vm.add_inst(NOP_OP);
  //read x and y
  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(x);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(READ_OP);

  vm.add_inst(PUSH_ADDR_OP);
  vm.add_data(y);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(INT_STD);
  vm.add_inst(READ_OP);

  //x == 1 ?
  vm.add_inst(PUSH_OP);
  vm.add_data(x);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)EQ_FLAG);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data(1);
  vm.add_inst(TESTI_OP);
  //x == y ?
  vm.add_inst(PUSH_OP);
  vm.add_data(x);
  vm.add_inst(PUSH_CONST_OP);
  vm.add_data((uint)EQ_FLAG);
  vm.add_inst(PUSH_OP);
  vm.add_data(y);
  vm.add_inst(TESTI_OP);
  // and 
  vm.add_inst(AND_OP);

  vm.add_inst(JMPFALSE_OP);
  uint Else = vm.add_data(0);

  vm.add_inst(PUSHS_OP);
  vm.add_data(eq);
  vm.add_inst(WRITE_STR_OP);

  vm.add_inst(JMP_OP);
  uint End = vm.add_data(0);

  vm.bytecode[Else] = (uint)vm.bytecode.size();

  vm.add_inst(PUSHS_OP);
  uint neq = vm.add_data(0);
  vm.bytecode[neq] = vm.write_const_string("not all the numbers are equal to 1\n");
  vm.add_inst(WRITE_STR_OP);

  vm.bytecode[End] = (uint)vm.bytecode.size();

  vm.add_inst(HALT_OP);
}
