//library imports
#include <stdio.h> //for standard input and output
#include <stdbool.h> //for Invalid booleans
#include <string.h> //for many useful string functions
#include <stdlib.h> //for system cls

//preferences/settings
#define LiveAmount 10 //change this number to change the player's lives amount

void ShowDictionary()
{
    //just some smiley face
    /*
   :D
    */
}
void AddWord()
{
    //just some smiley face
    /*
   :D
    */
}
void RemoveWord()
{
    //just some smiley face
    /*
   :D
    */
}
char GetWord()
{
    char guess; //to store the guessed char input
    bool invalid = true; //to determine whether or not the input is valid

    while(invalid) //keep prompting input until all inputs are valid
    {
        //prompt the user to enter a character and store it
        printf("Guess a letter : "); scanf(" %c", &guess);
        invalid = false; //default invalid to false
        //if the user entered a non alphabet character, set invalid to true, and output an error message
        if(!((guess >= 'A' && guess <= 'Z') || (guess >= 'a' && guess <= 'z')))
            { invalid = true; printf("Please Input Alphabets only\n"); }
    }
    return guess; //return the letter input
}
void ReturnToMenu()
{
    printf("\nPress [Enter] to return\n"); //prompt the user to press enter to go back to main menu
    scanf("%*c%*c"); //get the \n characters
    MainMenu(); //go back to main menu
}
void PrintHangMan(int lives)
{
    int percent = lives*100/LiveAmount;
    if(lives == LiveAmount) return;

    if(percent <= 70) printf("#######\n");
    if(percent <= 80) printf("##"); if(percent <= 60) printf("   |");
    if(percent <= 80) printf("\n##"); if(percent <= 50) printf("   @");
    if(percent <= 80) printf("\n##"); if(percent <= 40) printf("  /"); if(percent <= 30) printf("|"); if(percent <= 20) printf("\\");
    if(percent <= 80) printf("\n##"); if(percent <= 10) printf("  /"); if(percent <= 0) printf(" \\");
    if(percent <= 80) printf("\n##");
    if(percent <= 90) printf("\n#########\n");
}
void Game()
{
    char SecretWord[] = "Exoskeleton"; //this is the secret word
    strcpy(SecretWord, strlwr(SecretWord)); //make sure all characters are lowercase
    int len = strlen(SecretWord); //get the length of the word
    char Revealed[len+1]; //make the string to hide the word
    for(int i = 0; i < len; i++) { Revealed[i] = '_'; } Revealed[len] = '\0'; //set the hidden characters to '_'
    char GuessList[27] = ""; //keep track of used/guessed words
    int GuessNum = 0; //keep track of the amount of character used
    int lives = LiveAmount; //keep track of the player's lives
    bool wrong = false, guessed = false; //to determine whether or not the player guessed the right letter

    while(true) //keep looping until the game is over
    {
        system("cls"); //clear the screen
        //if the letter was already guessed before, or if it's the wrong letter,
        //print a message, and decrease the lives
        if(guessed) { printf("You already guessed that letter!\n"); lives -= 2; }
        else if(wrong) { printf("Incorrect!\n"); lives--; }
        else if(GuessNum != 0) { printf("You're Correct!\n"); }

        PrintHangMan(lives); //prints the hangman ascii based on the lives
        printf("%s\n", Revealed); //print the hidden string
        printf("%d letters\n", len); //show the length of the secret word

        //print all used/guessed letters
        printf("Used Letters : %s", GuessNum == 0 ? "none" : ""); //print none if the amount of guesses is 0
        for(int i = 0; i < 27; i++)
        {
            if(GuessList[i] >= 'a' && GuessList[i] <= 'z')
                printf("%c", GuessList[i]);
        }

        printf("\nRemaining Lives : %d\n", lives); //show the player's lives
        if(strcmp(SecretWord, Revealed) == 0 || lives == 0) break; //if the word has been revealed, break
        char guess = GetWord(); //to store the guessed char input
        if(guess < 'a') guess += 'a'-'A'; //make sure the letter is lowercase

        //if the letter was already guessed before, set the guessed bool to true,
        //but if it's not, add the guessed letter to the gussed list, set the guessed bool to false, and increment guessnum
        if(GuessList[guess-'a'] != guess) { GuessList[guess-'a'] = guess; guessed = false; GuessNum++; }
        else guessed = true;

        wrong = true; //initially, default wrong bool to true
        //using the Linear Search to traverse through the secret word string
        for(int i = 0; i < len; i++)
        {
            //if the guessed letter is found, then reveal the letter, and set wrong bool to false
            if(SecretWord[i] == guess) { Revealed[i] = guess; wrong = false; }
        }
    }

    //prints win/lose message based on the number of lives
    printf("\nYou %s The secret word was %s.\n", lives > 0 ? "win!" : "lose...", SecretWord);
    ReturnToMenu(); //press enter to go back to main menu
}
void Credits()
{
    system("cls"); //clear the screen
    printf("Made By :\n"); //Show the Creators of this app
    printf("Abid Rakhmansyah_2440077081\n");
    printf("Ardli Fadhillah Wangsaatmaja_2440029990\n");
    printf("Azella Gania Mutyara_2440035961\n");
    printf("Steven Yanuar Prasetyo Ginting_2440091722\n");
    ReturnToMenu(); //press enter to go back to main menu
}
void MainMenu()
{
    system("cls"); //clear the screen
    //print Introduction Message
    printf("Welcome to Hangman\n");
    printf("--------------------------\n");
    //Print all available Menus
    printf("1. Start Game\n");
    printf("2. See Dictionary List\n");
    printf("3. Add Words to Dictionary\n");
    printf("4. Remove Words from Dictionary\n");
    printf("5. Credits\n");
    printf("0. Exit\n");

    char menu = '\0'; //to store what number the user entered
    while(menu < '0' || menu > '5') //keep prompting until the user entered a number from 0-3
    {
        printf(">> "); scanf(" %c", &menu); //prompt the user to Input a number to enter one of the menus
        if(menu < '0' || menu > '5') printf("Please Input 0-3\n"); //Print an error message if the user entered invalid input
    }

    if(menu == '0') return; //if the user entered 0, terminate the function (and the program)
    //if the user entered 1-3, then call the respective functions
    else if(menu == '1') Game();
    else if(menu == '2') ShowDictionary();
    else if(menu == '3') AddWord();
    else if(menu == '4') RemoveWord();
    else if(menu == '5') Credits();
}
int main()
{
    MainMenu(); //show Main menu
    return 0; //terminate program
}
