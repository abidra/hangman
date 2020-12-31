//library imports
#include <stdio.h> //for standard input and output
#include <stdbool.h> //for boolean variables
#include <string.h> //for many useful string functions
#include <stdlib.h> //for system cls
#include <time.h> //to randomize seed based on time

//preferences/settings
#define LiveAmount 10 //change this number to change the player's lives amount
#define DictFile "Dictionary.txt" //defines the filename

void MainMenu(); //so we can call main menu even before it is defined

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
bool FileError(FILE *fp)
{
    //if file does not exists, close the file, prints an error message, and return true(indicates error)
    if(fp == NULL) { fclose(fp); printf("\nError : File Does Not Exist\n"); return true; }
    else
    {
        //else, move the file cursor the the end of the file
        fseek(fp, 0, SEEK_END);
        int len = ftell(fp); //and get the length of the file
        if (len > 0) //if length is not zero, the file is not empty
        {
            rewind(fp); //get back to the beginneing of the file
            return false; //return false(indicates non error)
        }
        //else(length of file is zero), close the file, print an error message, and return true(indicates error)
        else { fclose(fp); printf("\nError : File is Empty\n"); return true; }
    }
}
bool RandomizeWord(char* SecretWord)
{
	FILE *words = fopen(DictFile, "r"); //open the file
	if(FileError(words)) return false; //returns false if file operation failed

	int WordCount; fscanf(words, "%d", &WordCount); //Get the number of words in the file
	srand(time(0)); //pick a seed based on time
	int RandomIndex = (rand() % (WordCount-1+1))+1; //randomize a num based on the formula : (rand()%(max–min+1))+min
	printf("%d\n", RandomIndex);
	for(int i = 1; i < RandomIndex; i++) fscanf(words, "%*s"); //ignores the first RandomIndex-1 words from the file

	fscanf(words, "%s", SecretWord); //get the word at RandomIndex, and assign it to the secret word
	fclose(words); return true; //close the file and returns true(meaning operation success)
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
    if(lives == LiveAmount) return; //if player's live is intact, do not print any hangman
    int percent = lives*100/LiveAmount; //count the lives percentage

    //print the hangman based on the player's lives current status
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
    char SecretWord[21]; //this is the secret word
    if(!RandomizeWord(SecretWord)) return; //Randomize The Secret Word, but return if file operation failed

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
        //print a message, decrease the lives, and make sure the lives won't go below 0
        if(guessed) { printf("You already guessed that letter!\n"); lives -= 2; if(lives < 0) lives = 0; }
        else if(wrong) { printf("Incorrect!\n"); lives--; if(lives < 0) lives = 0; }
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
    printf("Made By :\n"); //Show the Creators of the app
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
    while(menu < '0' || menu > '5') //keep prompting until the user entered a number from 0-5
    {
        printf(">> "); scanf(" %c", &menu); //prompt the user to Input a number to enter one of the menus
        if(menu < '0' || menu > '5') printf("Please Input 0-5\n"); //Print an error message if the user entered invalid input
    }

    if(menu == '0') return; //if the user entered 0, terminate the function (and the program)
    //if the user entered 1-5, then call the respective functions
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
