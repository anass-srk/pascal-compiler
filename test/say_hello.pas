program sayhello;
const hello = "Hello";
type e = array [0..20] of char;
var name : e;
begin
    write("What s your name ?\n");
    read(name);
    write(hello,' ',name," !\n")
end.

