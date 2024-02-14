#include "parser.hxx"

int main(int argc,char* argv[]){
  if(argc != 2){
    print("Usage : program [program.p]\n");
    exit(EXIT_SUCCESS);
  }
  Lexer l(argv[1]);
  Parser p (std::move(l));
  p.parse();
}