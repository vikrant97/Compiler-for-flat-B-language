declblock{
	int s,a,b;
	int array1[10];
	int array2[10];
}
codeblock{
	println "hello world!";
	a=2;b=3;
	println a;
	L: a+=1;
	println "a:",a;
	goto L if a<10;
	println "a:",a;
}

