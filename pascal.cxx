#include "vm.hxx"

int main(int argc,char* argv[]){
  if(argc != 2){
    print("Usage : program [program]\n");
    exit(EXIT_SUCCESS);
  }
  std::string filename = argv[1];
  VM vm;
  vm.load_from_file(filename + ".bin");
  vm.run();
}