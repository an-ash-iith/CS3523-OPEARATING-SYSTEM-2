#include <stdio.h>
#include <stdlib.h>

int main(){
     
     int a;
     char ch;

    //  printf("Enter the number");
    //  scanf("%d",&a);
    //  printf("Enter the character-");
    //  scanf("%c",&b);


    //   printf(" The number is %d",a);
    //   printf(" The characer is %c",b);

// output was this. Because when it enter first number after that when we put enter it took \n or newline as new charcter in the buffer and thus scanf take this 
// input even the user have not given 
// so it is required to clean the buffer using fflush
      /*
      Enter the number 12
Enter the character- The number is 12 The characer is 
      */

//****************it will give the correct output***************************//


     printf("Enter the number ");
     scanf("%d",&a);

    //  fflush(stdin);   //it is not recommended to use this one because its behaviour is undefined in std c so avoid using this it may  work or may not be 
    // for me it was not working ]

//this is good method to avoid above one
    //  while ((getchar()) != '\n');

     printf("Enter the character ");
     scanf("%c",&ch);

      printf(" The number is %d",a);
      printf(" The characer is %c",ch);

     return 0;
}