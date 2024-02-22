#include "parser.hxx"

int main(int argc,char* argv[]){
  if(argc != 2){
    print("Usage : program [program]\n");
    exit(EXIT_SUCCESS);
  }
  std::string filename = argv[1];
  Lexer l(filename + ".pas");
  Parser p (std::move(l));
  p.parse();
  p.vm.save_to_file(filename + ".bin");
}