#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BORROW_LIMIT 4
#define LATE_FEE_PER_MIN 5

void save_admin_to_file();

//Creating structure for Book, Member, Admin.
typedef struct Book 
{
    int serial_num;
    char title[100];
    char author[50];
    int is_borrowed;
    time_t borrowed_time;
    struct Book *next, *prev;
} Book;

typedef struct Member 
{
    char id[20];
    char name[30];
    char password[20];
    int borrowed_count;
    Book *borrowed_books[MAX_BORROW_LIMIT];
    struct Member *next, *prev;
} Member;

typedef struct Admin 
{
    char id[30];
    char name[30];
    char password[20];
    struct Admin *next, *prev;
} Admin;

Book *book_head = NULL;
Member *member_head = NULL;
Admin *admin_head = NULL;

//Assigning admin details
Admin *initialize_admin()
{
    if (!admin_head) 
    {
        admin_head = (Admin *)malloc(sizeof(Admin));
    
        admin_head->next = NULL;
        admin_head->prev =NULL;
        strcpy(admin_head->id,"admin@123");
        strcpy(admin_head->name,"Admin");
        strcpy(admin_head->password,"12345");
        save_admin_to_file();
    }
    return admin_head;
}

// File Handling
void save_books_to_file() 
{
    FILE *file = fopen("books.txt", "w");
    if (!file) 
    {
        printf("Error opening file for books.\n");
        return;
    }
    Book *temp = book_head;
    while (temp) 
    {
        fprintf(file, "%d|%s|%s|%d\n", temp->serial_num, temp->title, temp->author, temp->is_borrowed);
        temp = temp->next;
    }
    fclose(file);
}

void load_books_from_file() 
{
    FILE *file = fopen("books.txt", "r");
    if (!file)
    {
        printf("Error opening file for books.\n");
        return;
    }
    
    while (!feof(file)) 
    {
        Book *new_book = (Book *)malloc(sizeof(Book));
        if (fscanf(file, "%d|%[^|]|%[^|]|%d\n", &new_book->serial_num, new_book->title, new_book->author, &new_book->is_borrowed) == 4) 
        {
            new_book->next = book_head;
            if (book_head) 
            {
                book_head->prev = new_book;
            }
            new_book->prev = NULL;
            book_head = new_book;
        } 
        else 
        {
            free(new_book);
        }
    }
    fclose(file);
}

void load_admin_from_file() 
{
    FILE *file = fopen("admin.txt", "r");
    if (!file)
    { 
        printf("Error opening file for members.\n");
        return;
    }

    while (!feof(file)) 
    {
        Admin *new_admin = (Admin *)malloc(sizeof(Admin));
        if (fscanf(file, "%[^|]|%[^|]|%s\n", new_admin->id, new_admin->name, new_admin->password) == 3) 
        {
            new_admin->next = admin_head;
            if (admin_head) admin_head->prev = new_admin;
            new_admin->prev = NULL;
            admin_head = new_admin;
        } 
        else 
        {
            free(new_admin);
        }
    }
    fclose(file);
}

void save_admin_to_file() 
{
    FILE *file = fopen("admin.txt", "w");
    if (!file) 
    {
        printf("Error opening file for members.\n");
        return;
    }
    Admin *temp = admin_head;
    while (temp) 
    {
        fprintf(file, "%s|%s|%s\n", temp->id, temp->name, temp->password);
        temp = temp->next;
    }
    fclose(file);
}


void save_members_to_file() 
{
    FILE *file = fopen("members.txt", "w");
    if (!file) 
    {
        printf("Error opening file for members.\n");
        return;
    }
    Member *temp = member_head;
    while (temp) 
    {
        fprintf(file, "%s|%s|%s|%d", temp->id, temp->name, temp->password, temp->borrowed_count);
        for (int i = 0; i < temp->borrowed_count; i++) 
        {
            fprintf(file, "|%d|%ld", temp->borrowed_books[i]->serial_num, temp->borrowed_books[i]->borrowed_time);
        }
        fprintf(file, "\n");
        temp = temp->next;
    }
    fclose(file);
}

void load_members_from_file() 
{
    if (member_head) 
    {
        return; // If members are already loaded, return early
    }

    FILE *file = fopen("members.txt", "r");
    if (!file) 
    {
        printf("Error opening file for members.\n");
        return;
    }

    while (!feof(file)) 
    {
        Member *new_member = (Member *)malloc(sizeof(Member));
        char line[500];
        if (fgets(line, sizeof(line), file)) 
        {
            char *token = strtok(line, "|");
            strcpy(new_member->id, token);
            token = strtok(NULL, "|");
            strcpy(new_member->name, token);
            token = strtok(NULL, "|");
            strcpy(new_member->password, token);
            token = strtok(NULL, "|");
            new_member->borrowed_count = atoi(token);

            for (int i = 0; i < new_member->borrowed_count; i++) 
            {
                token = strtok(NULL, "|");
                int serial = atoi(token);
                token = strtok(NULL, "|");
                time_t borrowed_time = atol(token);

                Book *temp = book_head;
                while (temp) 
                {
                    if (temp->serial_num == serial) 
                    {
                        new_member->borrowed_books[i] = temp;
                        temp->borrowed_time = borrowed_time;
                        temp->is_borrowed = 1;
                        break;
                    }
                    temp = temp->next;
                }
            }

            new_member->next = member_head;
            if (member_head) member_head->prev = new_member;
            new_member->prev = NULL;
            member_head = new_member;
        }
    }
    fclose(file);
}

// Buffer Functions
void clear_buffer() 
{
    while (getchar() != '\n');
}

// Book Management
void add_book() 
{
    Book *new_book = (Book *)malloc(sizeof(Book));
    printf("Enter book title: ");
    clear_buffer();
    fgets(new_book->title, 100, stdin);
    strtok(new_book->title, "\n");
    printf("Enter author name: ");
    fgets(new_book->author, 50, stdin);
    strtok(new_book->author, "\n");

    new_book->serial_num = (book_head == NULL) ? 1 : book_head->serial_num + 1;
    new_book->is_borrowed = 0;

    new_book->next = book_head;
    if (book_head) book_head->prev = new_book;
    new_book->prev = NULL;
    book_head = new_book;

    save_books_to_file();
    printf("Book '%s' by '%s' added successfully (Serial No: %d).\n",
    new_book->title, new_book->author, new_book->serial_num);
}

void remove_book() 
{
    int serial;
    Book *temp = book_head;
    if (!book_head)
    {
        printf("No books available to remove.\n");
        return;
    }
    
    printf("Enter serial number of the book to remove: ");
    scanf("%d", &serial);

    while (temp) 
    {
        if (temp->serial_num == serial)
        {
            if (temp->prev)
            {
                temp->prev->next = temp->next;
            }
            if (temp->next)
            {
                temp->next->prev = temp->prev;
            }        
            if (temp == book_head)
            {
                book_head = temp->next;
            }
            save_books_to_file();
            printf("Book '%s' removed successfully.\n", temp->title);
            free(temp);
            return;
        }
        temp = temp->next;
    }
    printf("Book with Serial No %d not found.\n", serial);
}

void display_books() 
{
    if (!book_head) 
    {
        printf("No books available in the library.\n");
        return;
    }
    printf("Available Books:\n");
    Book *temp = book_head;
    while (temp) 
    {
        printf("Serial No: %d\tTitle: %s\tAuthor: %s\tStatus: %s\n",
               temp->serial_num, temp->title, temp->author,
               temp->is_borrowed ? "Borrowed" : "Available");
        temp = temp->next;
    }
}

void borrow_book(Member *member) 
{
    int serial;
    Book *temp = book_head;
    if (member->borrowed_count >= MAX_BORROW_LIMIT) {
        printf("You have already borrowed the maximum allowed books (%d).\n", MAX_BORROW_LIMIT);
        return;
    }
    
    display_books();
    printf("Enter the serial number of the book to borrow: ");
    scanf("%d", &serial);
    
    while (temp) 
    {
        if (temp->serial_num == serial && !temp->is_borrowed) 
        {
            temp->is_borrowed = 1;
            temp->borrowed_time = time(NULL);  // Record borrow time
            member->borrowed_books[member->borrowed_count++] = temp;
            printf("Book '%s' borrowed successfully.\n", temp->title);
            display_books();
            return;
        }
        temp = temp->next;
    }
    printf("Book with Serial No %d not available for borrowing.\n", serial);
}

// Return book
void return_book(Member *member) 
{
    int choice, paid_fee,balance;
    if (member->borrowed_count == 0) 
    {
        printf("No borrowed books to return.\n");
        return;
    }
    printf("Your borrowed books:\n");
    for (int i = 0; i < member->borrowed_count; i++) 
    {
        printf("%d. %s\n", i + 1, member->borrowed_books[i]->title);
    }
    
    printf("Enter the number of the book to return: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > member->borrowed_count) 
    {
        printf("Invalid choice.\n");
        return;
    }

    Book *returned_book = member->borrowed_books[choice - 1];
    time_t return_time = time(NULL);  // Get current time
    double minutes_elapsed = difftime(return_time, returned_book->borrowed_time) / 60;
    int late_fee = (int)minutes_elapsed * LATE_FEE_PER_MIN;  // Calculate fee

    printf("Minutes elapsed: %.2f\n", minutes_elapsed);
    printf("Late fee: Rs.%d\n", late_fee);
    if (late_fee <= 0 ) 
    {
        printf("Book '%s' returned successfully.",returned_book->title);
        returned_book->is_borrowed = 0;
        returned_book->borrowed_time = 0;  // Reset borrow time

        for (int i = choice - 1; i < member->borrowed_count - 1; i++) 
        {
            member->borrowed_books[i] = member->borrowed_books[i + 1];
        }
        member->borrowed_count--;
    }
    else
    {
        printf("Enter the fee amount to pay: ");
        scanf("%d", &paid_fee);
        
        if (paid_fee >= late_fee) 
        {
            printf("Book '%s' returned successfully. Fee paid: Rs.%d. Balance: Rs.%d\n", returned_book->title, paid_fee, paid_fee-late_fee);
            returned_book->is_borrowed = 0;
            returned_book->borrowed_time = 0;  // Reset borrow time

            for (int i = choice - 1; i < member->borrowed_count - 1; i++) 
            {
                member->borrowed_books[i] = member->borrowed_books[i + 1];
            }
            member->borrowed_count--;
        } 
        else 
        {
            printf("Insufficient fee. Book not returned.\n");
        }
    }
}

// Member Management
void add_member() 
{
    load_members_from_file(); 

    int unique_name = 0, unique_password = 0;//initiailly declared as not unique

    Member *new_member = (Member *)malloc(sizeof(Member));
    clear_buffer();
    // Check for unique member name
    while (!unique_name) 
    {        
        printf("Enter member name: ");
        fgets(new_member->name, 30, stdin);
        strtok(new_member->name, "\n");

        // Check if the member name exists in the loaded list (to avoid duplicates)
        Member *temp = member_head;
        unique_name = 1; // Assume unique 

        while (temp) 
        {
            if (strcmp(new_member->name, temp->name) == 0) //Check for unique ,if unique ? 1:0
            {
                printf("Name '%s' already exists, so please enter a unique name:\n", new_member->name);
                unique_name = 0; // Not unique
                break;
            }
            temp = temp->next;
        }
    }

    // Check for unique password
    while (!unique_password) 
    {
        printf("Enter password: ");
        fgets(new_member->password, 20, stdin);
        strtok(new_member->password, "\n");

        // Check if the password exists in the loaded list (to avoid duplicates)
        Member *temp = member_head;
        unique_password = 1; // Assume unique 

        while (temp) 
        {
            if (strcmp(new_member->password, temp->password) == 0)  //Check for unique ,if unique ? 1:0
            {
                printf("Password already exists, so please enter a unique password:\n");
                unique_password = 0; // Not unique
                break;
            }
            temp = temp->next;
        }
    }

    // Assign unique ID (using timestamp for uniqueness)
    snprintf(new_member->id, 20, "%ld", time(NULL));  // Use current time as unique ID
    new_member->borrowed_count = 0;

    // Add the new member to the linked list (to avoid storing duplicates)
    new_member->next = member_head;
    if (member_head) member_head->prev = new_member;
    new_member->prev = NULL;
    member_head = new_member;

    printf("Member '%s' added successfully (ID: %s).\n", new_member->name, new_member->id);

    save_members_to_file(); // Save updated members to file
}

void remove_member()
{
    char en_id[20];
    Member *temp = member_head;
    if (!member_head)
    {
        printf("No members available to remove.\n");
        return;
    }
    
    printf("Enter member ID to remove: ");
    clear_buffer();
    scanf("%s",en_id);
  
    while (temp)
    {
        if (strcmp(temp->id ,en_id)==0)  
        {
            if (temp->prev)
            {
                temp->prev->next = temp->next;
            }
            if (temp->next)
            {
                temp->next->prev = temp->prev;
            }
            if (temp == member_head)
            {
                member_head = temp->next;
            }

            printf("Member '%s' removed successfully.\n", temp->name);
            free(temp);
            save_members_to_file();
            return;
        }
        temp = temp->next;
    }
    printf("Member with ID %s not found.\n", en_id);
}

void display_members() 
{
    load_members_from_file();  // Ensure existing members are loaded
    Member *temp = member_head;
    if (!member_head) 
    {
        printf("No members available in the library.\n");
        return;
    }
    printf("Library Members:\n");
    
    while (temp) 
    {
        printf("ID: %s\tName: %s\n", temp->id, temp->name);
        temp = temp->next;
    }
}

//Admin Menu
void admin_menu() 
{
    while (1) 
    {
        int choice;
        printf("\n______Admin Menu______\n1. Add Book\n2. Remove Book\n3. Display Books\n4. Remove Member\n5. Display Members\n6. Log out\n>>");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1: 
            {
                add_book(); 
                break;
            }
            case 2: 
            {
                remove_book(); 
                break;
            }
            case 3: 
            {
                display_books(); 
                break;
            }
            case 4:
            { 
                remove_member(); 
                break;
            }
            case 5: 
            {
                display_members(); 
                break;
            }
            case 6: 
            {
                save_books_to_file();
                save_members_to_file();
                return;
            }

            default: 
            {
                printf("Invalid choice.\n");
            }
        }
    }
}

//Member Menu
void member_menu(Member *member) 
{
    while (1) 
    {
        int choice;
        printf("\n______Member Menu______\n1. Borrow Book\n2. Return Book\n3. Log out\n>> ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1: 
            {
                borrow_book(member); 
                break;
            }
            case 2: 
            {
                return_book(member); 
                break;
            }
            case 3: 
            {
                save_members_to_file();
                save_books_to_file();
                return;
            }
            default: 
            {
                printf("Invalid choice.\n");
            }
        }
    }
}

//Main Menu
int main()
{
    load_books_from_file();
    load_members_from_file();

    while (1) 
    {
        printf("\n______Welcome______\nABDUL KALAM  LIBRARY:\n1. Admin Login\n2. Member Login \n3. New User \n4. Exit\n>>");
        int choice;
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
            {
                Admin *a_h=initialize_admin();
                char id[30],pass[30];
                printf("Enter Admin ID: ");
                clear_buffer();
                fgets(id, 30, stdin);
                strtok(id, "\n");
                printf("Enter Admin Password: ");
                fgets(pass, 20, stdin);
                strtok(pass, "\n");
                if(strcmp(a_h->id,id)==0 && strcmp(a_h->password,pass)==0)
                {
                    printf("\nWelcome %s \n",a_h->name);
                    admin_menu();
                }
                else
                {
                    printf("Invalid ID/PASSWORD: Try again");                   
                }
                break;
            }
            case 2:
            {
                char id[20];
                char pass[20];
                printf("Enter Member ID: ");
                clear_buffer();
                scanf("%s",id);
                strtok(pass, "\n");
                printf("Enter Password: ");
                clear_buffer();
                fgets(pass, 20, stdin);
                strtok(pass, "\n");
                Member *temp = member_head;
                while (temp) 
                {
                    if (strcmp(temp->id ,id)==0 && strcmp(temp->password,pass)==0)
                    {
                        printf("\nWelcome %s \n",temp->name);
                        member_menu(temp);
                        break;
                    }
                    temp = temp->next;
                }
                if (!temp)
                {
                    printf("Invalid Member ID.\n");
                }
                break;
            }
               
            case 3: 
            {
                add_member(); 
                break;
            }
            case 4: 
            {
                save_books_to_file();
                save_members_to_file();
                printf("Byee..Byee..Happy Reading.\n");
                return 0;
            }
            default: 
            {
                printf("Invalid choice.\n");
            }
        }
    } 
}
