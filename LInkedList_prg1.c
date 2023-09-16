#include<stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node * next;
};
`
int main() 
{ 
    int arr[5];
    for(int i =0; i; i++) {
        scanf("%d -> ", &arr[i]);
    }

    for(int i=0; i<5; i++) {
        printf("%d ==>> ", arr[i]);
    }

    printf("tt");
    return 0;

}
