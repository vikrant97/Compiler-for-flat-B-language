declblock{
	int x,i,n,j,fact;
	int a[50];
}
codeblock{
	read n;
	i=1;
	fact=1;
	while i<=n{
		fact*=i;
		i+=1;
	}
	println fact;
}