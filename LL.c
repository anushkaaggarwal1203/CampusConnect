#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int val;
    struct node *next;
}node;

node *createNode(){
    int val;
    node *a=(node*)malloc(sizeof(node));
    if(a==NULL){
        printf("memory not allocated\n");
        return a;
    }
    printf("enter value to be inserted:");
    scanf("%d",&val);
    a->val=val;
    a->next=NULL;
    return a;
}
node *insertatHead(node *head){
    node *a=createNode();
    if(a==NULL){
        return a;
    }
    if(head==NULL){
        head=a;
        return head;
    }
    else{
        a->next=head;
        head=a;
        return head;
    }
}
node *insertatTail(node *head){
    node *a=createNode();
    if(a==NULL){
        return a;
    }
    if(head==NULL){
        head=a;
        return head;
    }
    else{
        node *temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=a;
        return head;
    }
}
int size(node *head){
    node *temp=head;
    int s=0;
    while(temp!=NULL){
        s++;
        temp=temp->next;
    }
    return s;
}

node *insertatIdx(int idx,node *head){
    int s=size(head);
    if(idx<0 || idx>s){
        printf("invalid position\n");
        return head;
    }
    else if(idx==0){
        head=insertatHead(head);
        return head;
    }
    else if(idx==s){
        head=insertatTail(head);
        return head;
    }
    else{
        node *a=createNode();
        if(a==NULL){
            return head;
        }
        node *temp=head;
        for(int i=1;i<=idx-1;i++){
            temp=temp->next;
        }
        a->next=temp->next;
        temp->next=a;
        return head;
    }
}
node *delAtHead(node *head){
    if(head==NULL){
        printf("LL is empty!!\n");
        return head;
    }
    else{
        node *temp=head;
        head=head->next;
        temp->next=NULL;
        free(temp);
        temp=NULL;
        return head;
    }
}

node *delAtTail(node *head){
    if(head==NULL){
        printf("LL is empty!!\n");
        return head;
    }
    if(head->next==NULL){
        node *temp=head;
        head=head->next;
        temp->next=NULL;
        free(temp);
        temp=NULL;
        return head;
    }
    node *temp=head;
    while(temp->next->next!=NULL){
        temp=temp->next;
    }
    node *t=temp->next;
    temp->next=NULL;
    t->next=NULL;
    free(t);
    t=NULL;
    return head;
}

node *delAtIdx(int idx,node *head){
    if(head==NULL){
        printf("LL is empty!!\n");
        return head;
    }
    int s=size(head);
    if(idx<0 || idx>=s){
        printf("invalid position\n");
        return head;
    }
    else if(idx==0){
        head=delAtHead(head);
        return head;
    }
    else if(idx==s-1){
        head=delAtTail(head);
        return head;
    }
    else{
        node *temp=head;
        for(int i=1;i<=idx-1;i++){
            temp=temp->next;
        }
        node *t=temp->next;
        temp->next=temp->next->next;
        t->next=NULL;
        free(t);
        t=NULL;
        return head;
    }
}
void display(node *head){
    node *temp=head;
    if(temp==NULL){
        printf("LL is empty:");
        return;
    }
    while(temp){
        printf("%d ",temp->val);
        temp=temp->next;
    }
    printf("\n");
}

void getElement(int idx,node *head){
    int s=size(head);
    if(idx<0 || idx>s){
        printf("invalid index");
        return;
    }
    else if(idx==0){
        printf("%d\n",head->val);
    }
    else{
        node *temp=head;
        for(int i=1;i<=idx;i++){
            temp=temp->next;
        }
        printf("%d\n",temp->val);
        return;
    }
}
int main(){
    node *head=NULL;
    int ch;
    int idx;
    while(1){
        printf("\n1.insert at head\n2.insert at tail\n3.insert at idx\n4.del at head \n5.del at tail\n6.del at idx\n7.get element\n8.display\n9.exit\n");
        printf("enter choice:");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                head=insertatHead(head);
                break;
            case 2:
                head=insertatTail(head);
                break;
            case 3:
                printf("enter idx:");
                scanf("%d",&idx);
                head=insertatIdx(idx,head);
                break;
            case 4:
                head=delAtHead(head);
                break;
            case 5:
                head=delAtTail(head);
                break;
            case 6:
                printf("enter idx:");
                scanf("%d",&idx);
                head=delAtIdx(idx,head);
                break;

            case 7:
                printf("enter idx:");
                scanf("%d",&idx);
                getElement(idx,head);
                break;
            case 8:
                display(head);
                break;

            case 9:
                return 0;
            default:
                printf("invalid choice\n");
        }
    }
}