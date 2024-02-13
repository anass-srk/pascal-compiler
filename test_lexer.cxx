#include "lexer.hxx"

int main(int argc,char* argv[]){
  if(argc != 2){
    print("Usage : program [program.p]\n");
    exit(EXIT_SUCCESS);
  }
  Lexer l(argv[1]);
  l.next_sym();
  do{
    Lexeme lex = l.getToken();
    show_token(&lex);
    print(' ');
    switch(lex.type){
      case NUM_INT_TOKEN:
        println(lex.ival);
        break;
      case NUM_REAL_TOKEN:
        println(lex.dval);
        break;
      default:
        println(lex.id);
    }
    l.next_sym();
  }while(l.getToken().type != ERROR_TOKEN && l.getToken().type != EOF_TOKEN); 
}