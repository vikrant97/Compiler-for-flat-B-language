declblock{
	int data[100];
	int i, sum;
}

codeblock{
	i = 0;

L1:	data[i] = i; 
	i = i + 1;
	goto L1 if i < 100;
	print "2";
	println;

	sum = 0;
	i = i%7;
if x+y==2 and y==8 or x==0 and y<=7	or a==c{
	for i=1,2{
	a+=b*g-h;
	b/=msa*a/k;
	}
	while x<0{
	a=4;
	}
	if x+u == 2{
	b=b+i;
	}
}
L2:	sum = sum + data[i];
	i = i + 1;
	goto L2 if i < 100;

	print "Sum value: <? ", sum;
}

