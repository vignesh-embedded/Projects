#include<stdio.h>

int main ()
{
    int n,copy,rev=0,digit;
    printf("Enter Number: ");
    scanf("%d",&n);
    copy = n;
    while(copy!=0)
    {
        digit = copy % 10;
        rev = rev*10 + digit;
        copy = copy/10; 
    }
    if(n == rev)
    {
        printf("It's A Palindrome Number");
    }
    else
    {
        printf("Not A Palindrome");
    }
}