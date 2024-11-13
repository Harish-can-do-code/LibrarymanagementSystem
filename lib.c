#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void primary_display();
void member();
int id_store(char enter_m[]);

void primary_display()//This display is used as primary to showe the member/admin/new_user(or)Entry Portal.
 {
  int enter1;
    printf("<<<PRESS THE KEY TO ENTER>>>\n");
    printf("ADMIN (OR) STAFF   ->[1]\n");
    printf("MEMBER             ->[2]\n");
    printf("NEW-USER           ->[3]\n");
    scanf("%d",&enter1);
    switch(enter1)
    {
        case 1:
        {
            //admin();
            //break;
        }
        case 2:
        {
            member();
            break;
        }
        case 3:
        {
           // new_user();
           // break;
        }
        default:
        {
            printf("--------INVALIED KEY-------\n");
            printf("----ENTER THE CORRECT KEY----\n");
        }
    }
}

void member()
{
     char enter_m[7];//It use to get the id of the member.
     int enter_o;//It use to get the option of the member.
    int choice=1;
    while(choice==1)
    {
        printf("---------THE LIBRARY PORTAL---------\n");
        printf("....MEMBER....\n");
        printf("PLEASE ENTER YOUR REGISTED ID:");
        scanf("%s", enter_m);
        if(strlen(enter_m)>7)
        {
            printf("--------INVALIED ID-------\n");
            printf("----ENTER THE CORRECT ID----\n");
        }
        else
        {
            id_store(enter_m); 
            //printf("lend,return,renew=>switch case function call");
            return ;
        }
        printf("PRESS KEY-->[1] TO RE-ENTER THE MEMBER ID (OR) PRESS KEY-->[0] TO END\n");
        scanf("%d",&choice);
    }  
}

 int id_store(char enter_m[])
  {
    struct id_data
    {
        struct id_data *pre_node;//1
        char id[7];//2
        char name[30];//3
        char phone_no[11];//4
        struct id_data *next_node;//5
    };
    struct id_data *head,*temp,*new1,*new2,*new3,*new4;
    new1=(struct id_data*)malloc(sizeof(struct id_data));
    new2=(struct id_data*)malloc(sizeof(struct id_data));
    new3=(struct id_data*)malloc(sizeof(struct id_data));
    new4=(struct id_data*)malloc(sizeof(struct id_data));

    strcpy(new2->id, "SYED92");
    strcpy(new2->id, "nave85");
    strcpy(new2->id, "RAHU95");
    strcpy(new2->id, "DINE65");

    strcpy(new1->name, "MOHAMED SYED HARISH.S");
    strcpy(new2->name, "NAVEEN KUMAR.R");
    strcpy(new3->name, "RAHUL RAJA.R");
    strcpy(new4->name, "DINESH.S");

    strcpy(new1->phone_no, "63******92");
    strcpy(new2->phone_no, "63******85");
    strcpy(new3->phone_no, "91******09");
    strcpy(new4->phone_no, "97******65");
    head=new1;
    new1->pre_node=0;
    new1->next_node=new2;

    new2->pre_node=new1;
    new2->next_node=new3;

    new3->pre_node=new2;
    new3->next_node=new4;

    new4->pre_node=new3;
    new4->next_node=0;

    temp=head;
  while(temp!=NULL)
   { 
    if(strcmp(temp->id, enter_m)!=0)
    {
        printf("ID: %s\n", temp->id);
        printf("Name: %s\n", temp->name);
        printf("Phone Number: %s\n", temp->phone_no);
    }
    temp=temp->next_node;
   }
     /*if()
    {
        printf("ID not found.\n");
    }*/
    return 0;
}
void main()
{ int catch1;
    printf("---------THE LIBRARY PORTAL---------\n");
    primary_display();
   // catch1=enter1;
}

//The code is commited by Syed