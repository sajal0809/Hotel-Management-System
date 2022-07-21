#include <stdio.h>
#include <string.h>

struct room
{
    char name[100];
    int num;
    int from_day, from_month, from_year, to_day, to_month, to_year;
    char status[20];
} a, a2;

struct details
{
    char name[100];
    char password[100];
} user;

struct date
{
    int d, m, y;
} b, r;

// ******************************************************************************************

int days(int from_day, int from_month, int from_year, int to_day, int to_month, int to_year)
{
    if (from_month == to_month && from_year == to_year)
    {
        return to_day - from_day;
    }
    else if (from_month != to_month || from_year != to_year)
    {
        int day = 0;
        while (from_day != to_day || from_month != to_month || from_year != to_year)
        {
            if (from_day == 31 && (from_month == 1 || from_month == 3 || from_month == 5 || from_month == 7 || from_month == 8 || from_month == 10))
            {
                from_month++;
                from_day = 1;
            }
            else if (from_day == 30 && (from_month == 4 || from_month == 6 || from_month == 9 || from_month == 11))
            {
                from_month++;
                from_day = 1;
            }
            else if (from_day == 29 && from_month == 2 && ((from_year % 4 == 0 && from_year % 100 != 0) || (from_year % 400 == 0)))
            {
                from_month++;
                from_day = 1;
            }
            else if (from_day == 28 && from_month == 2 && !((from_year % 4 == 0 && from_year % 100 != 0) || (from_year % 400 == 0)))
            {
                from_day = 1;
                from_month = 3;
            }
            else if (from_day == 31 && from_month == 12)
            {
                from_day = 1;
                from_month = 1;
                from_year++;
            }
            else
            {
                from_day++;
            }
            day++;
        }
        return day;
    }
}
// compare function for two dates(from and to of database and from and to from the user)
int compare(int from_day1, int from_month1, int from_year1, int to_day1, int to_month1, int to_year1, int from_day2, int from_month2, int from_year2, int to_day2, int to_month2, int to_year2)
{
    int flag = 0;
    if ((from_day1 == from_day2) && (from_month1 == from_month2) && (from_year1 == from_year2) && (to_day1 == to_day2) && (to_month1 == to_month2) && (to_year1 == to_year2))
    {
        return 0;
    }

    if (from_year2 > to_year1)
    {
        flag = 1;
        return 1;
    }
    else if (from_month2 > to_month1 && from_year2 >= to_year1)
    {
        flag = 1;
        return 1;
    }
    else if (from_day2 >= to_day1 && from_month2 >= to_month1 && from_year2 >= to_year1)
    {
        flag = 1;
        return 1;
    }
    else if (to_year2 < from_year1 && flag == 0)
    {
        return 1;
    }
    else if (to_month2 < from_month1 && flag == 0 && to_year2 <= from_year1)
    {
        return 1;
    }
    else if (to_day2 <= from_day1 && flag == 0 && to_year2 <= from_year1 && to_month2 <= from_month1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//checking if the date entered is correct or not
int checkdate(int from_day, int from_month, int from_year, int to_day, int to_month, int to_year)
{
    if (to_month > 12 || to_month <= 0 || to_day <= 0 || to_day > 31 || from_month > 12 || from_month <= 0 || from_day <= 0 || from_day > 31)
    {
        return 1;
    }
    else if (((to_month == 4 || to_month == 6 || to_month == 9 || to_month == 11) && to_day > 30) || ((from_month == 4 || from_month == 6 || from_month == 9 || from_month == 11) && from_day > 30))
    {
        return 1;
    }
    else if ((to_month == 2 && to_day > 29 && ((to_year % 4 == 0 && to_year % 100 != 0) || to_year % 400 == 0)) || (to_month == 2 && to_day > 28 && !((to_year % 4 == 0 && to_year % 100 != 0) || to_year % 400 == 0)))
    {
        return 1;
    }
    else if (from_year > to_year)
    {
        return 1;
    }
    else if (from_year == to_year)
    {
        if (from_month > to_month)
        {
            return 1;
        }
        else if (from_month == to_month)
        {
            if (from_day >= to_day)
            {
                return 1;
            }
        }
    }
    return 0;
}

//checking if there is a room available in the user entered to and from date
int available(char name2[], int room, int from_day, int from_month, int from_year, int to_day, int to_month, int to_year, char status2[])
{
    FILE *q;
    int f = 0, flag1 = 0, d1, m1, y1, d2, m2, y2, i = 0, j, g = 0;
    q = fopen("data", "r");
    do
    {
        fscanf(q, "%s : Room no. %d from %d %d %d to %d %d %d : %s", a2.name, &a2.num, &a2.from_day, &a2.from_month, &a2.from_year, &a2.to_day, &a2.to_month, &a2.to_year, a2.status);
        if (a2.num == room)
        {
            if (compare(a2.from_day, a2.from_month, a2.from_year, a2.to_day, a2.to_month, a2.to_year, from_day, from_month, from_year, to_day, to_month, to_year) == 0)
            {
                if (strcmp(a2.status, "Booked") == 0)
                {
                    return 0;
                }
            }
            flag1 = 1;
        }
    } while (!feof(q));

    if (flag1 == 1)
    {
        fclose(q);
        return 1;
    }
}

//if the room is available then the user is alloted that room
int allot(char name1[], int type, int from_day, int from_month, int from_year, int to_day, int to_month, int to_year, char status1[])
{
    int i;
    for (i = (type * 100) + 1; i < (type * 100) + 10; i++)
    {
        if (available(name1, i, from_day, from_month, from_year, to_day, to_month, to_year, status1) == 1)
        {
            return i;
        }
    }
    return 0;
}

int printroom(int choice)
{
    int choice1, num = 0;
    num = allot(a.name, choice, a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year, a.status);
    if (num == 0)
    {
        printf("\nSorry No room available of this type!\nTry another type:");
        scanf("%d", &choice1);
        printroom(choice1);
    }
    else
    {
        return num;
    }
}
int get_choice()
{
    int choice = 0;
    printf("\nEnter Type of Room(Type 1 to 6):");
    scanf("%d", &choice);
    if (choice > 6 || choice < 1)
    {
        printf("\nYour choice is invalid.pleaze enter a valid choice.\n\n");
        get_choice();
    }
    else
    {
        return choice;
    }
}

struct date check()
{
    printf("Enter date in format (dd mm yyyy): ");
    scanf("%d %d %d", &b.d, &b.m, &b.y);
    if (b.m > 12 || b.m <= 0 || b.d <= 0 || b.d > 31)
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    else if (((b.m == 4 || b.m == 6 || b.m == 9 || b.m == 11) && b.d > 30))
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    else if ((b.m == 2 && b.d > 29 && ((b.y % 4 == 0 && b.y % 100 != 0) || b.y % 400 == 0)) || (b.m == 2 && b.d > 28 && !((b.y % 4 == 0 && b.y % 100 != 0) || b.y % 400 == 0)))
    {
        printf("Sorry the Date you entered is invalid please enter again\n\n");
        check();
    }
    return b;
}

//booking mechanism
void book()
{
    FILE *p, *q;
    int b, n, i, count = 0, flag1;
    long long int fare = 0;
    int price[6] = {2500, 2000, 3500, 3000, 4500, 5000};
    FILE *pass;
    do
    {
        flag1 = 0;
        printf("\nEnter Name: ");
        fflush(stdin);
        scanf("%s", a.name);
        pass = fopen("user Password Id", "r+");
        do
        {
            fscanf(pass, "%s : %s", user.name, user.password);
            if (strcmp(user.name, a.name) == 0)
            {
                printf("\nYou are already an existing user\n\nEnter your password: ");
                char ipassword[100];
                scanf("%s", ipassword);
                if (strcmp(user.password, ipassword) == 0)
                {
                    flag1 = 1;
                }
                else
                {
                    flag1 = -1;
                    printf("\nInvalid password, You will have to make a new account with a different name\n\n");
                }
                break;
            }
        } while (!feof(pass));
        fclose(pass);
    } while (flag1 == -1);
    if (flag1 == 0)
    {
        printf("You are a new user, please enter a password for your account: ");
        char ipassword[100];
        scanf("%s", ipassword);
        pass = fopen("user Password Id", "a");
        fprintf(pass, "%s : %s\n", a.name, ipassword);
        fclose(pass);
        flag1 = 1;
    }
    if (flag1 == 1)
    {
        printf("\nFROM date\n");
        r = check();
        a.from_day = r.d;
        a.from_month = r.m;
        a.from_year = r.y;
        printf("\nTO date\n");
        r = check();
        a.to_day = r.d;
        a.to_month = r.m;
        a.to_year = r.y;
        b = checkdate(a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year);
        while (b == 1)
        {
            printf("\nThe dates you entered are not in proper order.Try again\n\n");
            printf("\nFROM date\n");
            r = check();
            a.from_day = r.d;
            a.from_month = r.m;
            a.from_year = r.y;
            printf("\nTO date\n");
            r = check();
            a.to_day = r.d;
            a.to_month = r.m;
            a.to_year = r.y;
            b = checkdate(a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year);
        }
        printf("\n\nHere are the type of rooms and their maximum occupancy with tariffs:\n  Type         Max Occupancy    Tariff(/Night)\n");
        printf("1.Single AC          1             Rs.2500\n2.Non AC Single      1             Rs.2000\n3.Double AC          2             Rs.3500\n4.Double Non AC      2             Rs.3000\n5.Duplex             4             Rs.4500\n6.Suite              5             Rs.5000\n");
        printf("\nEnter Number of Rooms you want:");
        scanf("%d", &n);
        int choice;
        while (1)
        {
            choice = get_choice();
            a.num = printroom(choice);
            count++;
            printf("\nHey %s, your booking is successful and your room number is %d!!\n\n", a.name, a.num);
            fare += days(a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year) * price[choice - 1];
            strncpy(a.status, "Booked", sizeof(a.status));
            p = fopen("data", "a");
            fprintf(p, "%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n", a.name, a.num, a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year, a.status, days(a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year) * price[choice - 1]);
            fclose(p);
            if (count == n)
            {
                break;
            }
        }
        printf("Total fare is Rs.%lld\n\n", fare);
    }
}
// *********************************************************************************************
void view()
{

    while (1)
    {
        int choice;
        printf("Enter your choice:\n 1.General floor\n 2.All Floor view \n 3.Exit\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("                     Sea Facing\n");
            printf("|************************************************|\n");
            printf("|     _______        _______        _______      |\n");
            printf("|    |       |      |       |      |       |     |\n");
            printf("|    |   1   |      |   2   |      |   3   |     |\n");
            printf("|    |_______|      |_______|      |_______|     |\n");
            printf("|                                                |\n");
            printf("|     _______        _______        _______      |\n");
            printf("|    |       |      |       |      |       |     |\n");
            printf("|    |   4   |      |   5   |      |   6   |     |\n");
            printf("|    |_______|      |_______|      |_______|     |\n");
            printf("|                                                |\n");
            printf("|     _______        _______        _______      |\n");
            printf("|    |       |      |       |      |       |     |\n");
            printf("|    |   7   |      |   8   |      |   9   |     |\n");
            printf("|    |_______|      |_______|      |_______|     |\n");
            printf("|                                                |\n");
            printf(" *************************************************\n");
        }
        else if (choice == 2)
        {

            printf("***************************************************\n");
            printf("|               Hotel Building                    |\n");
            printf("|             ____________________                |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 6        |               |\n");
            printf("|            |   (Suite)          |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 5        |               |\n");
            printf("|            |   (Duplex)         |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 4        |               |\n");
            printf("|            |   (Double Non-AC)  |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 3        |               |\n");
            printf("|            |   (Double AC)      |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 2        |               |\n");
            printf("|            |   (Non-Single AC)  |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |     Floor 1        |               |\n");
            printf("|            |   (Single AC)      |               |\n");
            printf("|            |____________________|               |\n");
            printf("|            |                    |               |\n");
            printf("|            |   Ground Floor     |               |\n");
            printf("|            |____________________|               |\n");
            printf("|                                                 |\n");
            printf("|                                                 |\n");
            printf("***************************************************\n");
        }
        else
        {
            break;
        }
    }
}

void review()
{
    int fare;

    printf("Review\n");
    int flag = 0, flag1 = 0;
    FILE *data, *pass;
    data = fopen("data", "r");
    printf("Enter Name: ");
    char uname[100];
    char upassword[100];
    scanf("%s", uname);
    printf("Enter your password: ");
    scanf("%s", upassword);
    pass = fopen("user Password Id", "r");
    do
    {
        fscanf(pass, "%s : %s", user.name, user.password);
        if (strcmp(user.name, uname) == 0 && strcmp(user.password, upassword) == 0)
        {
            flag1 = 1;
        }
    } while (!feof(pass));

    fclose(pass);
    if (flag1 == 1)
    {
        printf("Booking History:\n");
        do
        {
            fscanf(data, "%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d", a.name, &a.num, &a.from_day, &a.from_month, &a.from_year, &a.to_day, &a.to_month, &a.to_year, a.status, &fare);
            if (feof(data))
            {
                break;
            }
            if (strcmp(a.name, uname) == 0)
            {
                flag = 1;
                printf("%s : Room no. %d from %d %d %d to %d %d %d : %s Fare:%d\n\n", a.name, a.num, a.from_day, a.from_month, a.from_year, a.to_day, a.to_month, a.to_year, a.status, fare);
            }
        } while (!feof(data));
    }
    else
    {
        printf("Invalid username or password\n");
    }
}

//********************************************************************************************************
int main()
{
    

    int choice1;
    char string[10], s1[4] = {"yes"}, s2[4] = {"no"};
    void (*f1[3])() = {book, review, view};

    printf("*******************************************************************************************************\n");
    printf("*******************************************************************************************************\n");
    printf("**                                                                                                   **\n");
    printf("**                                                                                                   **\n");
    printf("**                 _____  _______  _____             ______              _____                       **\n");
    printf("**      |      |  |     |    |    |       |         |         |      |  |     |  |         |         **\n");
    printf("**      |      |  |     |    |    |       |         |         |      |  |     |  |         |         **\n");
    printf("**      |______|  |     |    |    |_____  |         |______   |______|  |     |  |         |         **\n");
    printf("**      |      |  |     |    |    |       |                |  |      |  |     |  |    |    |         **\n");
    printf("**      |      |  |     |    |    |       |                |  |      |  |     |  |    |    |         **\n");
    printf("**      |      |  |_____|    |    |_____  |_____     ______|  |      |  |_____|  |____|____|         **\n");
    printf("**    ---------------------------------------------------------------------------------------        **\n");
    printf("**    ---------------------------------------------------------------------------------------        **\n");
    printf("**                                                                                                   **\n");
    printf("**      	      **********************************************                                     **\n");
    printf("**       	      | CO-FOUNDERS ARE:                           |                                     **\n");
    printf("**      	      |                                            |                                     **\n");
    printf("**      	      |       Sajal Sameer  &  Gaurav Satpute      |                                     **\n");
    printf("**      	      **********************************************                                     **\n");
    printf("**                                                                                                   **\n");
    printf("**                                                                                                   **\n");
    printf("*******************************************************************************************************\n");
    printf("*******************************************************************************************************\n");

    //----------------------------Designing part----------------------------------------------------

    while (1)
    {
        printf("\n");
        printf("Enter:\n \n1.  Book\n \n2.  Review booking\n \n3.  View\n \n4.  Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice1);
        if (choice1 > 4)
        {
            printf("Invalid choice\n\n");
            continue;
        }
        if (choice1 == 4)
        {
            break;
        }
        else
        {
            (*f1[choice1 - 1])();
        }
        printf("\nDo you wish to continue?(yes/no):");
        scanf("%s", string);
        if (strcmp(string, s2) == 0)
        {
            break;
        }
    }

    
    return 0;
}
