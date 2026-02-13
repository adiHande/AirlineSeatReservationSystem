
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#pragma warning(disable : 4996)

# define MAX_COLS 10
# define MAX_ROWS 20
# define MAX_LINE 20
# define MAX_NO_FLIGHTS 10
# define MASTER_LENGTH 20
# define BOOKING_LENGTH 20
//# define TICKET_LIMIT 4


enum SeatLayout {
    L23 = 1,
    L232,
    L343
};

struct FlDets {
    char flt[6];
    enum SeatLayout layout;
    int  rows;
};


/*struct RC {
    int row;
    int col;
};*/


/*int layout[3][4] =
    { { L23 , 5 , 2 , 99 },   // no of cols=5, 1st aisle after 2cols, 2nd aisle after 99 cols(does not exist)
      { L232, 7 , 2 , 5  },   // no. of cols=7, 1st aisle after 2 cols, 2nd aisle after 5 aisles
      { L343, 10, 3 , 7  },   // no. of cols=10,  1st aisle after 3 cols, 2nd aisle after 7 cols
    };
*/
//char Flights[MAX_NO_FLIGHTS][3][10];
struct FlDets Flights[MAX_NO_FLIGHTS];


int layoutToCols(enum SeatLayout layout) {
    switch (layout) {
    case L23:
        return  5;           break;
    case L232:
        return  7;           break;
    case L343:
        return  10;          break;
    default: return 0;
    }
}


void initSeatMap(char Seats[MAX_ROWS][MAX_COLS], int NumberOfRows, int NumberOfCols)  {
    for (int i = 0; i < NumberOfRows; i++)  {
        for (int j = 0; j < NumberOfCols; j++)   {
            Seats[i][j] = 'O';   
        }
    }
}

void showSeatMap(char Seats[MAX_ROWS][MAX_COLS], int NumberOfRows, enum SeatLayout layout) {
    int i = 0;
    int j = 0;
    char SeatCol = 'A';
    int NumberOfCols = layoutToCols(layout);

    printf("\n%5c", ' ');

    //printf("   ");
    for (i = 0; i < NumberOfCols; i++)   {
        //SeatCol = toupper(SeatCol);
        printf("%c  ", SeatCol++);
        switch (layout)      {
            case L23:
                if (i == 1)
                    printf("    ");
                break;
            case L232:
                if (i == 1 || i == 4)
                    printf("    ");
                break;
            case L343:
                if (i == 2 || i == 6)
                    printf("    ");
                break;

        }
    }
    printf("\n\n");

    for (i = 0; i < NumberOfRows; i++)      {
        printf(" %3d ", i + 1);

        for (j = 0; j < NumberOfCols; j++)  {
            char ch;
            if (Seats[i][j] == 'X')  ch = 0xDC;  //0xDB;  //0xB1    // used a character from "charmap.exe" in terminal font bcoz it looks good with 0xDC as its ASCII code//Ascii chr's character code
            else ch = '_';  //0xB0;   
            printf("%c%c ", ch, ch);    //Seats[i][j]);
            switch (layout)
            {
                case L23:
                    if (j == 1)
                        printf("    ");
                    break;
                case L232:
                    if (j == 1 || j == 4)
                       printf("    ");
                    break;
                case L343:
                    if (j == 2 || j == 6)
                        printf("    ");
                    break;

            }
        }
        printf("\n");
    }
}


void WriteSeatMap(const char Seats[MAX_ROWS][MAX_COLS], int NumberOfRows, int NumberOfCols, const char fileName[])
{
    FILE* outFile = NULL;

    outFile = fopen(fileName, "w+");
    if (outFile == NULL)   {
        printf("A file did not open properly... Exiting.\n");
        exit(0);
    }

    //fprintf(outFile, "%d %d\n", NumberOfRows, NumberOfCols); first line gives rows cols
    for (int i = 0; i < NumberOfRows; i++)       {
        for (int j = 0; j < NumberOfCols; j++)   {
            fprintf(outFile, "%c", Seats[i][j]);
        }
        fprintf(outFile, "\n");
    }
    fclose(outFile);
}



void makeReservation(char TravelDate[], char FlightName[], struct FlDets  flt, char Seats[MAX_ROWS][MAX_COLS]) {
    int ArrayRow, ArrayCol, SeatRow;
    char SeatLetter;

    char fileName[100] = "";
    strcat(fileName, TravelDate);
    strcat(fileName, ".");
    strcat(fileName, FlightName);
    strcat(fileName, ".txt");

    int maxRows, maxCols;
    maxRows = flt.rows;
    maxCols = layoutToCols(flt.layout);
    int quit = 0;

    
    
    while (!quit)
    {
        // PrintSeatMap(MovieTheater, RowsInTheater, ColsInTheater);*/

        printf("\nEnter seat choice by entering the row and seat.\nPlease pick a seat\n");
        printf("Enter '0' to exit the code\n");
        scanf(" %d%c", &SeatRow, &SeatLetter);
        // printf("%d%c", SeatRow, SeatLetter);
        SeatLetter = toupper(SeatLetter);
        ArrayRow = SeatRow - 1;
        ArrayCol = SeatLetter - 65;
        //printf("Row= %d, Column= %d", ArrayRow, ArrayCol);

        if (SeatRow == 0 || SeatLetter == 'q' || SeatLetter == 'Q') {
            quit = 1;
            return;
        }
        if (((ArrayRow < 0) || (ArrayRow >= maxRows)) || ((ArrayCol < 0) || (ArrayCol >= maxCols)))
        {
            printf("\n Invalid seat!!\n\n");
        }
        else if (Seats[ArrayRow][ArrayCol] == 'X')
        {
            printf("\n\nSeat %d%c is already reserved. Pick a different seat.  \n\n", SeatRow, SeatLetter);
        }
        else
        {
            Seats[ArrayRow][ArrayCol] = 'X';
        }
        showSeatMap(Seats, flt.rows, flt.layout);
        WriteSeatMap(Seats, maxRows, maxCols, fileName);
    }
}

    



int fileExists(const char* fname) {
    FILE* file;
    if ( (file = fopen(fname, "r")) )   {
        fclose(file);
        return 1;
    }
    else return 0;
}


void readSeatsFile(char TravelDate[], char FlightName[], struct FlDets  flt, char Seats[MAX_ROWS][MAX_COLS] ) {   // What does this do
    char fileName[100] = "";
    strcat(fileName, TravelDate);
    strcat(fileName, ".");
    strcat(fileName, FlightName);
    strcat(fileName, ".txt");

    int row, col, maxRows, maxCols;
    row = 0; col = 0;
    maxRows = flt.rows;                                //takes maxrows to 
    maxCols = layoutToCols(flt.layout);                //takes maxcols

    //printf("rows=%d, cols=%d", maxRows, maxCols);
    if (fileExists(fileName)) {
        char dummy;                                         //flag if file exists char dummy=1;

        // read file logic
        FILE* InFile = NULL;
        InFile = fopen(fileName, "r+");
        if (InFile == NULL) {
            printf("File did not open properly... Exiting.\n");
            return 1;
        }
 
        while (!feof(InFile) && !ferror(InFile)) {
            fscanf(InFile, "%c", &Seats[row][col]);
            //printf("row=%d, col=%d, readch=%c\n", row, col, Seats[row][col]);
            if (col == maxCols) {
                col = 0;
                row++;
                fscanf(InFile, "\n", &dummy);
            }
            else {
                col++;
            }

            if (row == maxRows) break;
        }
        fclose(InFile);

    }
    else {
        FILE* Newfile = NULL;
        Newfile = fopen(fileName, "w");   // create empty uninitialized file
        if (Newfile == NULL)    {
            printf("File did not open properly... Exiting.\n");
            return;
        }
        fclose(Newfile);

        initSeatMap(Seats, maxRows, maxCols );
        WriteSeatMap(Seats, maxRows, maxCols, fileName);
    }

    showSeatMap(Seats, maxRows, flt.layout);
}



int readMasterFile()    {
    char data[MAX_NO_FLIGHTS][MASTER_LENGTH];                                         
    char FileName[100] = { "Master.txt" };

    FILE* InFile = NULL;
    InFile = fopen(FileName, "r+");
    if (InFile == NULL)       {
        printf("File did not open properly... Exiting.\n");
        return 1;
    }

    int line = 0;
    while (!feof(InFile) && !ferror(InFile)) {                            //Search what ferror is
        if (fgets(data[line], MASTER_LENGTH, InFile))
            line++;                                                       // Gets all strings in the array
    }
    fclose(InFile);
    
    //printf("lines = %d\n",line);

    for (int i = 0; i < line; i++) {
        //printf("%s\n", data[i]);

        //int originalstrsize = strlen(data[i]);
        char delim[] = " \n";   
        // last word in each line ends in ' \n' i.e., space and then new line 
        // Ex: First string is "123 1 10" in Master.txt. Our Delimiter is 'space\n'. So after 1st element, '123', it finds space delimiter 
        // replaces 'space' with \0 i.e. null terminating char and makes the first element into one string. then reads "1", reads space, 
        // converts space to /0 and converts to 2nd string.It reads 3rd elemnt '10' and finds \n after it instead of space, converts it to \0 and 
        // keeps it as 3rd string. 
        // So, it breaks the first string"123 1 10" into three seperate small strings"123", "1", "10". 

        char* ptr = strtok(data[i], delim);                                // Can look up online properly
        strcpy(Flights[i].flt, ptr);
        ptr = strtok(NULL, delim);
        Flights[i].layout = atoi(ptr);
        ptr = strtok(NULL, delim);
        Flights[i].rows = atoi(ptr);

    }
}

struct FlDets flightInfo(const char flight[]) {                                       //Does this just print the output line orr is this code necessary
    //for (int i = 0; i < MAX_NO_FLIGHTS; i++)
    //    if (strcmp(Flights[i][0], flight) == 0)
    //        printf(" Flight = %s, Layout = %s, rows = %s", Flights[i][0], Flights[i][1], Flights[i][2]);
    for (int i = 0; i < MAX_NO_FLIGHTS; i++)
        if (strcmp(Flights[i].flt, flight) == 0) {
           // printf(" Flight = %s, Layout = %d, rows = %d", Flights[i].flt, Flights[i].layout, Flights[i].rows);
            return Flights[i];
        }

}


char TravelDate[9] = { "20240330" };              // Just common declaration for variables used in all functions but not necessarily defined everywhere so to keep//not junk
char FlightName[6] = { "123" };



bool isValidDate(int year, int month, int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1)
        return false;

    int max_days = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        max_days = 30;
    else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            max_days = 29;
        else
            max_days = 28;
    }

    return day <= max_days;
}

bool isDateAfterToday(int year, int month, int day) {
    time_t now;
    struct tm* current_time;
    time(&now);
    current_time = localtime(&now);

    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    if (year > current_year)
        return true;
    else if (year == current_year) {
        if (month > current_month)
            return true;
        else if (month == current_month && day >= current_day)
            return true;
    }

    return false;
}

void askDate() {
    int IsValid;
    do {
        //printf("Enter date of flight FlightName in YYYYMMDD format\n");
        //scanf("%s", TravelDate);
        //Checking Date Validity w following two Functions

        int year, month, day;

        IsValid = 1;
        printf("Enter a date in YYYYMMDD format: ");
        scanf("%4d%2d%2d", &year, &month, &day);

        if (!isValidDate(year, month, day)) {
            printf("Invalid date entered.\n");
            IsValid = 0;
        }

        if (!isDateAfterToday(year, month, day)) {
            printf("Entered date is prior to today's date.\n");
            IsValid = 0;
        }

        if (IsValid) {
            itoa((year * 10000 + month * 100 + day), TravelDate,10 );
        }

    } while (!IsValid);
}

void askFlight() {
    printf("Enter your flight name\n");
    scanf("%s", FlightName);
    // check valid flight number
}

int main(void)  {
    int year, day, month;
    char Seats[MAX_ROWS][MAX_COLS];
    struct FlDets fltInfo;
    //struct RC flRC;
    int RowsInPlane = MAX_ROWS;
    int ColsInPlane = MAX_COLS;


    readMasterFile();

    askDate();
    askFlight();
    fltInfo = flightInfo(FlightName);

    readSeatsFile(TravelDate, FlightName, fltInfo, Seats);
    makeReservation(TravelDate, FlightName, fltInfo, Seats);
}