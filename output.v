module test(a,b,c,d,x,y,z);
	input a;
	input b;
	input c;
	input d;
	output x;
	output y;
	output z;

	wire  c_n;
	wire  m;
	wire  n;

	not #8 i0(c_n, c);
	and #1 i1(m, a, b);
	or #1 i2(n, c_n, d);
	nand #3 i3(x, m, n);
	nand #1 i4(y, m, n);
	nand #1 i5(z, m, n);
endmodule