declblock{
	int x,i,n,j,temp;
	int a[50];
}
codeblock{
	read n;
	i=1;
	while i<=n{
		read a[i];
		i+=1;
	}
	i=1;
	while i<=n{
		j=i+1;
		while j<=n{
			if a[i]>a[j]{
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
			}
			j+=1;
		}
		i+=1;
	}
	i=1;
	while i<=n{
		print a[i];
		i+=1;
	}
	println "";
	
}
