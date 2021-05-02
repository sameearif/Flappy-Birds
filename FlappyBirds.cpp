#include<iostream> //For cout, cin etc
#include<iomanip> //For setw()
#include<windows.h> //For clearing the console screen and to remove that annoying blinking underscore and to use Sleep()
#include<conio.h> //To use kbhit and getch()
#include<stdlib.h> //To use rand()
#include<time.h> //To use srand()


using namespace std;

//2D array to store the world of flappy birds
string grid[21][31];

//Functions
void main_menu();
void loading_screen();
void clearScreen();
void ShowConsoleCursor(bool showFlag);
void make_grid(int bird_row, int bird_column, int pillar_row[], int pillar_column[], int &counter);
void make_pillars(int pillar_row[], int pillar_column[], int &counter);
void output_grid();
void bird_fall(int &bird_row, int &bird_column);
void bird_flap(int &bird_row, int &bird_column);
void move_pillars(int pillar_row[], int pillar_column[]);
void pillar_hit(bool &hit);
void calculate_score(int &score, int &high_score, bool &hit, int pillar_column[]);
void game_over(int score, int high_score);

int main()
{
    char choice = '0';
    int bird_row = 9;
    int bird_column = 0;
    int pillar_row[3];
    int pillar_column[3];
    char button_pressed;
    int counter;
    bool hit = false;
    int score = 0;
    int high_score = 0;
    char new_game = 'a';
    //Remove that annoying blinking underscore
    ShowConsoleCursor(false);
    //Load the main menu
    main_menu();
    //Game will not start until S is pressed
    while(choice != 's')
    {
        if(kbhit())
        {
            choice = getch();
        }

    }
    //Condition for the exit screen
    while(new_game == 'a')
    {
        //Loading screen will show up
        loading_screen();
        //Loading screen will be cleared and game starts
        clearScreen();
        //Draw the world of flappy birds
        make_grid(bird_row, bird_column, pillar_row, pillar_column, counter);
        //Output the world
        output_grid();
        //Sleep so that the bird does not start moving instantly after load screen
        Sleep(500);
        //Loop to ensure that the game keeps on running until the bird hits the pillar or falls
        while(bird_row < 19 && !hit)
        {
            //Loop to ensure that the bird keeps on falling until the end of the grid (until it falls off)
            while(bird_row < 19 && !hit)
            {
                //If a key is pressed the bird stops falling and flaps up
                if(kbhit())
                {
                    //Get the character pressed from the user
                    button_pressed = getch();
                    //Will only end the loop if 'f' is pressed
                    if(button_pressed == 'f')
                    {
                        break;
                    }
                }
                //Move the pillar to the left
                move_pillars(pillar_row, pillar_column);
                //Decrease the height of the bird
                bird_fall(bird_row, bird_column);
                //Clear the screen
                clearScreen();
                //Output the new screen with updated bird position
                output_grid();
                //Check if the pillar hits the bird
                pillar_hit(hit);
                //If the bird didn't hit the pillar calculate score
                calculate_score(score, high_score, hit, pillar_column);
                //To decrease the vertical falling velocity of the bird
                Sleep(0);
            }
            if(bird_row != 19 && !hit)
            {
                if(button_pressed == 'f')
                {
                    //Move the pillar to the left
                    move_pillars(pillar_row, pillar_column);
                    //Jump up if a key is pressed
                    bird_flap(bird_row, bird_column);
                    //Check if the pillar hits the bird while the bird is flapping
                    pillar_hit(hit);
                    //Calculate score if the bird didn't hit the pillar
                    calculate_score(score, high_score, hit, pillar_column);
                }
                //Clear the screen
                clearScreen();
                //Output the bird at new position
                output_grid();
                //To decrease the vertical falling velocity of the bird
                Sleep(0);
            }
        }
        Sleep(1000);
        //The game ends
        game_over(score, high_score);
        //User will enter f or e to play again or to exit
        Sleep(1000);
        new_game = getch();
        //To restrict the entry to a and e only
        while(!(new_game == 'a' || new_game == 'e'))
        {
            //User will enter f or e to play again or to exit
            new_game = getch();
        }
        //Reset the game data
        hit = false;
        score = 0;
        bird_row = 9;
        bird_column = 0;
    }
    if(new_game == 'e')
    {
        cout << "\nTHANKYOU FOR PLAYING! (^.^)";
    }

    return 0;
}

void clearScreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void make_grid(int bird_row, int bird_column, int pillar_row[], int pillar_column[], int &counter)
{
    //Variable for distance between the two pillars
    int next_pillar = 10;
    //Add empty spaces in the grid
    for(int r = 0; r <= 20; r++)
    {
        for(int c = 0; c <= 24; c++)
        {
            grid[r][c] = " ";
        }
    }
    //Draw the bird in the grid
    grid[bird_row][bird_column] = "\\\\";
    grid[bird_row + 1][bird_column] = "<\\\\-Q";

    //Draw Pillars
    srand(time(NULL));
    //Set the initial positions of the pillars, with spaces of 7 between then
    pillar_column[0] = 10;
    pillar_column[1] = pillar_column[0] + next_pillar;
    pillar_column[2] = pillar_column[1] + next_pillar;
    //Loop to make the pillars
    for(int i = 0; i <= 2; i++)
    {
        //Generate random point where there will be space for the bird to pass
        pillar_row[i] = rand()%19 + 1;
        //Call function to make the pillar
        make_pillars(pillar_row, pillar_column, i);
    }
}

void make_pillars(int pillar_row[], int pillar_column[], int &counter)
{
    //First draw the pillar without any space for the bird to pass
    for(int i = 0; i <= 20; i++)
    {
        grid[i][pillar_column[counter]] = "****";
    }
    //Now add space in the pillar for the bird to pass
    //If the random point where the space must start is smaller than 10 then the end point of the space will be greater than 10
    if(pillar_row[counter] <= 10)
    {
        for(int i = pillar_row[counter]; i <= pillar_row[counter] + 6; i++)
        {
            grid[i][pillar_column[counter]] = " ";
        }
    }
    //If the random point where the space must start is greater than 10 then the end point of the space will be smaller than 10
    else
    {
        for(int i = pillar_row[counter]; i >= pillar_row[counter] - 6; i--)
        {
            grid[i][pillar_column[counter]] = " ";
        }
    }
}

void output_grid()
{
    for(int r = 0; r <= 20; r++)
    {
        for(int c = 0; c <= 20; c++)
        {
            //setw() used so that there is alignment
            cout << setw(5) << left << grid[r][c];
        }
        cout << "\n";
    }
}

void bird_fall(int &bird_row, int &bird_column)
{
    //Remove the bird
    grid[bird_row][bird_column] = " ";
    grid[bird_row + 1][bird_column] = " ";
    //Increment bird_row to make the bird fall in the grid
    bird_row++;
    //Store the bird at the new row
    grid[bird_row][bird_column] = "\\\\";
    grid[bird_row + 1][bird_column] = "<\\\\-Q";

}

void bird_flap(int &bird_row, int &bird_column)
{
    //Remove the bird
    grid[bird_row][bird_column] = " ";
    grid[bird_row + 1][bird_column] = " ";
    //Increment bird_row by some value to make the bird jump in the grid
    bird_row = bird_row - 4;
    //So that the bird does no go out of bound of the array
    if(bird_row < 0)
    {
        bird_row = 0;
    }
    //Store the bird at the new row
    grid[bird_row][bird_column] = "<//-Q";
    grid[bird_row + 1][bird_column] = "//";
}

void move_pillars(int pillar_row[], int pillar_column[])
{
    //First remove all the generated pillars
    for(int i = 0; i <= 2; i++)
    {
        for(int r = 0; r <= 20; r++)
        {
            grid[r][pillar_column[i]] = " ";
        }
    }
    //Decrement the column index of the pillars so that they move to left
    for(int i = 0; i <= 2; i++)
    {
        //If the pillar has reached the point where the bird is, it will go back to the right most end
        if(pillar_column[i] == -1)
        {
            pillar_column[i] = 29;
            pillar_row[i] = rand()%19 + 1;
            make_pillars(pillar_row, pillar_column, i);
        }
        //If the has not reached the point where the bird is, it will decrement so that pillar moves to left
        else
        {
            pillar_column[i]--;
        }
    }
    //Generate new pillar, moved to the left
    for(int i = 0; i <= 2; i++)
    {
        make_pillars(pillar_row, pillar_column, i);
    }
}

void pillar_hit(bool &hit)
{
    //Variables to count *s to deal with the "ends" of pillar hit
    int temp_counter = 0;
    //Loop to check all rows
    for(int r = 0; r <= 20; r++)
    {
        //If at the index pillar is stored i would check if bird is about to hit the pillar or not
        if(grid[r][1] == "****")
        {
            //If at the index - 1 of the pillar bird is stored it would bit the pillar
            if(grid[r][0] == "<\\\\-Q" || grid[r][0] == "<//-Q")
            {
                //Bool variable is set to true and program breaks out of loop
                hit = true;
                break;
            }
        }
        //Check if the bird hits the pillar while flapping
        //If at the index bird is stored then it would check if there is pillar stored on top of the bird index
        if(grid[r][0] == "<\\\\-Q" || grid[r][0] == "<//-Q")
        {
            //Checks if there is pillar on top of the bird
            if(grid[r - 1][0] == "****" || grid[r + 1][0] == "****")
            {
                //Deals with the "end" hit case
                for(int i = 0; i <= 20; i++)
                {
                    //Counts the ****. There should be 14 if the bird does not hit it
                    if(grid[i][0] == "****")
                    {
                        //Counter for ****
                        temp_counter++;
                    }
                }
                //If there is no some missing **** then the bird has hit the pillar
                if(temp_counter < 14)
                {
                    //Bool variable is set to true and program breaks out of loop
                    hit = true;
                    break;
                }
            }
        }
        //Reset counter
        temp_counter = 0;
    }
}

void calculate_score(int &score, int &high_score, bool &hit, int pillar_column[])
{
    //Checks for each pillar if it has reached the position of the bird
    for(int i = 0; i <= 2; i++)
    {
        //If the pillar has reached the position of the bird it checks if the bird hit the pillar or not
        if(pillar_column[i] == 0)
        {
            //If the bird has not hit the pillar then score is increased by 1
            if(!hit)
            {
                //Increment score
                score++;
                //Output score
                cout << "Score: " << score << endl;
            }
            //If the score gets greater than the high score high score will be updated as score gets updated
            if(score > high_score)
            {
                //High score equals the new score
                high_score = score;
            }
            //Output the new high score
            cout << "High score: " << high_score;
        }
    }
}

void main_menu()
{
    cout << " --------------------------------------------------------  " << endl;
    cout << "|                                                        | " << endl;
    cout << "|   **** *    **** **** **** *   *    ***  * ***  ***    | " << endl;
    cout << "|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | " << endl;
    cout << "|   ***  *    **** **** **** *****    ***  * ***  *  *   | " << endl;
    cout << "|   *    *    *  * *    *      *      *  * * *  * *  *   | " << endl;
    cout << "|   *    **** *  * *    *      *      ***  * *  * ***    | " << endl;
    cout << "|                                                        | " << endl;
    cout << " --------------------------------------------------------  " << endl;
    cout << endl;
    cout << "      >Press 'S' to start the game \\(^-^)/ " << endl;
    cout << "\n\n";
    cout << "How to play: Press 'F' to pay respect and flap \\_('')_/ " << endl;
}

void loading_screen()
{
    system("cls");
    char back = 177;
    char front = 219;
    cout << "\n\n\n\tLoading....";
    cout << "\n\n\n\tAs long as it's not too much of a BIRDen (^-^)";
    cout << "\n\n\n";
    cout << "\t";
    for(int i = 0; i <= 25; i++)
    {
        cout << front;
        Sleep(200);
    }
}

void game_over(int score, int high_score)
{
    system("cls");
    cout << " ------------------------------------------------------------------------- " << endl;
    cout << "|    *****      *     *       * ******       ****  *       ****** ****    |" << endl;
    cout << "|   *          * *    * *   * * *           *    *  *     * *     *   *   |" << endl;
    cout << "|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |" << endl;
    cout << "|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |" << endl;
    cout << "|    *****  *       * *       * ******       ****      *    ***** *   *   |" << endl;
    cout << " ------------------------------------------------------------------------- " << endl;
    cout << "\n";
    cout << "High score : " << high_score << endl;
    cout << "Your score : " << score << endl;\
    //If our score is less than the high score
    if(high_score > score)
    {
        cout << "Don't get in a FLAP! (>_<) Try again and beat the high score! (^_-)" << endl;
    }
    //If we have beaten the high score
    else
    {
        cout << "New high score!! You are sooooo FLY! \\(^o^)/" << endl;
    }
    cout << "\n\n";
    cout << ">Press 'A' play again" << "\t>Press 'E' to exit" << endl;
}
