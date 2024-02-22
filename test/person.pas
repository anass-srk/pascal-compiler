program per;
type e = array [0..15] of char;
type person = record
                  name : e;
                  age : int
              end;
function get : person;
var human : person;
begin
    write("What s your name ?\n");
    read(human.name);
    write("And how old are you ?\n");
    read(human.age);
    get := human
end
var p : person;
var desc : e;
begin
    p := get;
    if p.age <= 30 then desc := "young"
    else if p.age <= 55 then desc := "middle-aged"
    else desc := "old";
    write("Hello ",desc,' ',p.name," !\n")
end.
