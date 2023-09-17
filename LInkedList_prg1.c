#include<stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node * next;
}*head = NULL;

struct node* create(int arr[],int n) 
{   
    int i;
    struct node *temp, *last;
    head= (struct node *)malloc(sizeof(struct node)); //create the first node seprately 
    head->data = arr[0];
    head->next = NULL;
    last = head;

    for(i = 1; i < n ; i++) {
        temp = (struct node *) malloc(sizeof(struct node));
        temp->data = arr[i];
        temp->next = NULL;
        last->next = temp;
        last = temp;

    }
}

int count( struct node *head) 
{
    int count = 0;
    if(head == NULL) {
        return count;
    }
    else {
        struct node *p = head;
        while(p) {
            count++;
            p=p->next;
        }
        return count;
    }
}

void insert(struct node *p, int index , int value )
{
    struct node *t;
    if(index < 0 || index > count(p)+1) {
        return ;
    }
    t = (struct node *)malloc(sizeof(struct node));
    t->data = value;

    if(index==0) {
        t->next= head;
        head = t;
    }
    
    else if(index==count(p)+1) {
        while(p->next!='\0') {
            p=p->next;
        }
        p->next= t;
        t->next = NULL;
    }

    else {
        for (int i = 1; i <index-1; i++) {
            p=p->next;
        }
        t->next = p->next;
        p->next = t;
    }
}
 
int delete(struct node *p, int index)
{
    struct node *q=NULL;
    int x=-1 ;

    if(index < 1 || index > count(p)) {
        return -1;
    }

    if (index == 1) {
        q = head;
        x = head->data;
        head = head->next;
        free(q);
        return x;
    }

    else {
        for(int i = 0 ; i <index-1; i++) {
            q = p;
            p = p->next;
        }
        q->next = p->next;
        x = p->data;
        free(p);
        return x;
    }
}

void display (struct node * p) 
{
   while (p!=NULL) {
        printf("%d -> ", p->data);
        p=p->next;
   }
   printf("NULL\n\n");
}

int main() 
{   int arr[] = {1, 2, 5, 7, 4 };

    create(arr,5);
    display(head);
    insert(head,0,8);
    display(head);
    delete(head,1);
    display(head);
    int end = count(head);
    printf("\n%d\n\n", end);
    insert(head,count(head)+1,8);
    display(head);
    delete(head,count(head));
    display(head);
    insert(head,4,8);
    display(head);
    delete(head,4);
    display(head);


    return 0;

}
