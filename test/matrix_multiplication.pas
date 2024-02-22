program mm;
type matrix = array [1..2,1..2] of int;
var a,b,c : matrix; i,j,k : int;
begin
    write("filling matrix a : (2,2)\n");
    for i := 1 downto 3 do
    begin
        for j := 1 to 3 do
        begin
            write("a(",i,',',j,") is : \n");
            read(a[i,j])
        end
    end;
    write("filling matrix b : (2,2)\n");
    for i := 1 downto 3 do
    begin
        for j := 1 to 3 do
        begin
            write("b(",i,',',j,") is : \n");
            read(b[i,j])
        end
    end;
    for i := 1 downto 3 do
    begin
        for j := 1 to 3 do
        begin
            c[i,j] := 0;
            for k := 1 to 3 do c[i,j] := c[i,j] + a[i,k] * b[k,j]
        end
    end;
    for i := 1 downto 3 do
    begin
        for j := 1 to 3 do
        begin
            write("c(",i,',',j,") is : ",c[i,j],'\n')
        end
    end
end.
