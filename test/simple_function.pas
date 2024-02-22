program sf;
const prompt = "Write 2 integers :";
var x,y : int; choice : int;
function add(i,j:int) : int;
begin
    add := i+j
end
function sub(i,j:int) : int;
begin
    sub := i-j
end
function mul(i,j:int) : int;
begin
    mul := i*j
end
function ddiv(i,j:int) : int;
begin
    ddiv := i div j
end
begin
    write("Select 1 to add, 2 to substract, 3 to multiply and 4 to divide !\n");
    repeat read(choice) until (choice = 1) or (choice = 2) or (choice = 3) or (choice = 4);
    write(prompt,'\n');
    read(x,y);
    case choice of 1 : x := add(x,y); 2 : x := sub(x,y); 3 : x := mul(x,y); 4 : x := ddiv(x,y) end;
    write("The result is ",x,'\n')
end.
