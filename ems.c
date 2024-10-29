/************************/
/*  Ondrej Kopecek      */
/*  29.10.24            */
/*  trainee prog. no. 1 */
/************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char first_name[25];
    char last_name[25];
    int age;
    float salary;
} Employee;

void display_options()
{
    fprintf(stdout,"\n\tEmployee Management System\n\n");
    
    fprintf(stdout,"1.Add Employee\n");
    fprintf(stdout,"2.Display Employee\n");
    fprintf(stdout,"3.Search Employee by ID\n");
    fprintf(stdout,"4.Save Employee to File\n");
    fprintf(stdout,"5.Load Employee from File\n");
    fprintf(stdout,"6.Exit\n");   
}
// prompts for choice
// input must be int, else it keeps iterating
int enter_choice()
{
    int choice;
    int success;
    while(1)
    {
        fprintf(stdout, "\nEnter choice: ");
        success = scanf("%d", &choice);
        if(success)
        {
            return choice;
        }
        else
        {
            fprintf(stdout, "Invalid input, it must be an integer.\n");
            while(getchar() != '\n');
        }
    }
}
// adds employees info from stdin and saves them to struct Employee.
void add_employee(Employee *emp, unsigned i)
{   
    fprintf(stdout,"\nEnter Employee ID: ");
    scanf("%d", &emp[i].id);

    fprintf(stdout,"\nEnter Employee First name: ");
    scanf("%25s", emp[i].first_name);
   
    fprintf(stdout,"\nEnter Employee Last name: ");
    scanf("%25s", emp[i].last_name);

    fprintf(stdout,"\nEnter Employee Age: ");
    scanf("%d", &emp[i].age);

    fprintf(stdout,"\nEnter Employee Salary: ");
    scanf("%f", &emp[i].salary);
}
//compares input from user with employee's id
//returns 1 if succesful
int employee_search(int *search_id, Employee *employees, unsigned *count)
{
    for(int i = 0; i < (int)*count; i ++)
    {
        if(*search_id == employees[i].id)
        {
            fprintf(stdout,"ID: %d, NAME: %s %s, AGE: %d, SALARY: %f\n", 
                employees[i].id, employees[i].first_name, 
                employees[i].last_name , employees[i].age, 
                employees[i].salary);
            return 1;
        }
    }
    return 0;
}
int main()
{
    display_options();
    int choice;             // holds user's choice
    unsigned count = 0;     // count how many employees there are
    unsigned capacity = 1;  // count available memory

    Employee *employees = malloc(capacity*sizeof(Employee));    //initial allocation
    if(employees == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    while(choice = enter_choice())
    {
        if(choice == 6)
        {
            printf("\nsajonara!\n");
            free(employees);
            return 0;
        }
        else if(choice == 1)
        {
            
            if(capacity == count)   // if new_employee is to be added
            {
                capacity += 1;
                Employee *temporary_employee = realloc(employees, capacity * sizeof(Employee)); //  add more memory if more employees are to be added
                if(temporary_employee == NULL)
                {  
                    fprintf(stderr,"Failed to realloc memory\n");
                    free(employees);
                }
                employees = temporary_employee; // pointer employees points to the added temporary_employee aswell
            }
        
            add_employee(employees,count);
            count++;
        }
        else if(choice == 2)
        {
            if(count == 0)
            {
                fprintf(stdout,"NO emmployees to be displayed");
                continue;
            }
            for(int i = 0; i < (int)count;i++)
            {
                printf("ID: %d, NAME: %s %s, AGE: %d, SALARY: %f\n", 
                    employees[i].id, employees[i].first_name, 
                    employees[i].last_name , employees[i].age, 
                    employees[i].salary);
            }  
        }
        else if(choice == 3)
        {
            if(count == 0)
            {
                fprintf(stdout,"NO employees to be searched\n");
                continue;
            }
            int search_id;
            printf("Enter ID: ");
            scanf("%d", &search_id);
            if(!employee_search(&search_id, employees, &count))
            {
                printf("No Employee found.\n");
            }
        }
        else if(choice == 4)
        {
            if(count == 0)
            {
                fprintf(stdout,"NO employees to be saved to a file\n");
                continue;
            }

            FILE *employee_file = fopen("employees.txt","w");

            if (employee_file == NULL)
            {
                fprintf(stderr, "File hasn't been opened.\n");
            }

            for(int i = 0; i < (int)count;i++)
            {
                fprintf(employee_file,"ID: %d, NAME: %s %s , AGE: %d, SALARY: %f\n", 
                    employees[i].id, employees[i].first_name, 
                    employees[i].last_name , employees[i].age, 
                    employees[i].salary);
            }
            fprintf(stdout,"File with %d employees has been added\n", count);
            
            fclose(employee_file);
        }
        else if(choice == 5)
        {
            FILE *employee_file = fopen("employees.txt","r");
            if(employee_file == NULL)
            {
                fprintf(stderr, "Unable to open file.\n");
                continue;
            }
            else
            {
                fprintf(stdout,"Opened successfully\n");
            }
            
            //loads employees while pointer returned from fgets isn't NULL
            char buffer[100];
            while(fgets(buffer, sizeof(buffer), employee_file) != NULL)
            {
                if(count == capacity)
                {
                    capacity += 1;
                    Employee *temporary_employee = realloc(employees, capacity*sizeof(Employee));
                    if(temporary_employee == NULL)
                    {
                        fprintf(stderr, "failed to reallocate memory\n");
                        free(employees);
                        break;
                    }
                    employees = temporary_employee;
                }
                sscanf(buffer,"ID: %d, NAME: %s %s , AGE: %d, SALARY: %f\n", &employees[count].id, employees[count].first_name, employees[count].last_name , &employees[count].age, &employees[count].salary);
                count++;
            }
            fclose(employee_file);            
        }
        else
        {
            fprintf(stdout, "Invalid input, try again\n");
            continue; 
        }
    }
    return 0;
}


