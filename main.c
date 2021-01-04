//library imports
#include <stdio.h> //for standard input and output
#include <stdbool.h> //for boolean variables
#include <string.h> //for many useful string functions
#include <stdlib.h> //for clear screen functions
#include <time.h> //to randomize seed based on time

//preferences/settings
#define LiveAmount 10 //defines the player's lives amount
#define DictFile "Dictionary.txt" //defines the filename
#define MaxWordLen 26 //defines the maximum word length in the dictionary (n+1)
#define MaxNameLen 51 //defines the maximum username length (n+1)

//function prototype, so we can call them even before it is defined
void ClearScreen();
void ReturnToMenu();
void ToLower(char *str, int len);
bool FileError(FILE *fp);
void MergeSort(char (*arr)[MaxWordLen], int min, int max);
//global var
char Name[MaxNameLen];
struct Leaderboard
{
    char name[MaxNameLen];
    int score;
};

void SaveScore(int score)
{
    FILE *ScoreFile = fopen("Leaderboard.txt", "r"); //open score file for reading

    if(FileError(ScoreFile)) //if file does not exist or if it's empty
    {
        fclose(ScoreFile); //close the file pointer
        ScoreFile = fopen("Leaderboard.txt", "w"); //and create a new file (or open the empty file for writing)
        fprintf(ScoreFile, "1\n"); //insert 1 as the number of scores
        fprintf(ScoreFile, "%s %d\n", Name, score); //insert player's name and the score to the file
        fclose(ScoreFile); //close the file pointer
        return;
    }

    int DataCount; fscanf(ScoreFile, "%d", &DataCount); //get the number of data in the file
    struct Leaderboard LB[DataCount+1]; //to store user and score data
    //length is data count + 1 so we can insert the new data
    bool UserExist = false; //to determine if it's a new player or not

    for(int i = 0; i < DataCount; i++) //iterate through all the data
    {
        fscanf(ScoreFile, "%s%d", LB[i].name, &LB[i].score); //insert name and score into the struct
        if(strcmp(LB[i].name, Name) == 0 && !UserExist) //if the username already exist
        {
            if(score > LB[i].score) LB[i].score = score; //replace the score of that username (if it's higher)
            UserExist = true; //keep track that this is an not a new player
        }
    }
    if(!UserExist) //if the player is new
    {
        //insert it's username and it's score into the end of the struct
        strcpy(LB[DataCount].name, Name);
        LB[DataCount].score = score;
        DataCount++; //increase the number of data
    }
    fclose(ScoreFile); //close file

    //sort data--

    ScoreFile = fopen("Leaderboard.txt", "w"); //open score file for writing
    fprintf(ScoreFile, "%d\n", DataCount); //insert new number of data in the file

    for(int i = 0; i < DataCount; i++) //iterate through all the data
        fprintf(ScoreFile, "%s %d\n", LB[i].name, LB[i].score); //insert name and score data into the file

    fclose(ScoreFile); //close file
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
void PrintHangMan(int lives)
{
    if(lives == LiveAmount) return; //if player's live is intact, do not print any hangman
    int percent = lives*100/LiveAmount; //count the lives percentage

    //print the hangman based on the player's lives current status
    if(percent <= 70) { printf("#######\n"); }
    if(percent <= 80) { printf("##"); } if(percent <= 60) { printf("   |"); }
    if(percent <= 80) { printf("\n##"); } if(percent <= 50) { printf("   @"); }
    if(percent <= 80) { printf("\n##"); } if(percent <= 40) { printf("  /"); } if(percent <= 30) { printf("|"); } if(percent <= 20) { printf("\\"); }
    if(percent <= 80) { printf("\n##"); } if(percent <= 10) { printf("  /"); } if(percent <= 0) { printf(" \\"); }
    if(percent <= 80) { printf("\n##"); }
    if(percent <= 90) { printf("\n#########\n"); }
}
int LetterRarity(bool *arr)
{
    int total = 0; //keep track of the rarity measure
    //add total by 1 for every common frequency english letters
    char common[] = "etainoshr";
    for(int i = 0; i < 9; i++) { if(arr[common[i]-'a']) total += 1; }
    //add total by 2 for every medium frequency english letters
    char medium[] = "dlucmfwyg";
    for(int i = 0; i < 9; i++) { if(arr[medium[i]-'a']) total += 2; }
    //add total by 3 for every rare frequency english letters
    char rare[] = "pbvkqjxz";
    for(int i = 0; i < 8; i++) { if(arr[rare[i]-'a']) total += 3; }
    return total; //return final rarity measure
}
bool RandomizeWord(char* SecretWord)
{
	FILE *words = fopen(DictFile, "r"); //open the file
	//if file operation failed, print an error message and return false
	if(FileError(words)) { printf("\nError : File Does Not Exist or is Empty\n"); return false; }

	int WordCount; fscanf(words, "%d", &WordCount); //Get the number of words in the file
	srand(time(0)); //pick a seed based on time
	int RandomIndex = (rand() % (WordCount-1+1))+1; //randomize a num based on the formula : (rand()%(max–min+1))+min
	for(int i = 1; i < RandomIndex; i++) fscanf(words, "%*s"); //ignores the first RandomIndex-1 words from the file

	fscanf(words, "%s", SecretWord); //get the word at RandomIndex, and assign it to the secret word
	fclose(words); return true; //close the file and returns true(meaning operation success)
}
void Game(int round, int TotalScore)
{
    char SecretWord[MaxWordLen]; //this is the secret word
    if(!RandomizeWord(SecretWord)) return; //Randomize The Secret Word, but return if file operation failed
    int len = strlen(SecretWord); //get the length of the word
    ToLower(SecretWord, len); //make sure all characters are lowercase

    char Revealed[len+1]; //make the string to hide the word
    for(int i = 0; i < len; i++) { Revealed[i] = '_'; } Revealed[len] = '\0'; //set the hidden characters to '_'
    char GuessList[27] = ""; //keep track of used/guessed words
    int GuessNum = 0; //keep track of the amount of character used

    int lives = LiveAmount; //keep track of the player's lives
    bool wrong = false, guessed = false; //to determine whether or not the player guessed the right letter
    //get all different letters in the secret word
    bool UniqueChar[27]; memset(UniqueChar, 0, 27*sizeof(bool));
    for(int i = 0; i < len; i++) UniqueChar[SecretWord[i]-'a'] = true;
    //get word difficulty based on letter rarity
    int difficulty = LetterRarity(UniqueChar);

    while(true) //keep looping until the game is over
    {
        ClearScreen(); //clear the screen
        //if the letter was already guessed before, or if it's the wrong letter,
        //print a message, decrease the lives, and make sure the lives won't go below 0
        //else if the player have guessed, and the guess is right, just print a message
        if(guessed) { printf("You already guessed that letter!\n"); lives -= 2; if(lives < 0) lives = 0; }
        else if(wrong) { printf("Incorrect!\n"); lives--; if(lives < 0) lives = 0; }
        else if(GuessNum != 0) { printf("You're Correct!\n"); }

        printf("Round %d\n", round); //print the current round
        PrintHangMan(lives); //print the hangman ascii based on the number of lives
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
        if(guess < 'a') guess += ('a'-'A'); //make sure the letter is lowercase

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
    //calculate score based on the word difficulty and the number of lives
    int score = TotalScore + (difficulty * lives);
    //prints the current score if the game is not over, or print final score if the game is over
    printf("%s Score is : %d\n", lives > 0 ? "Current" : "Final", score);
    if(lives > 0) //if the game is not over
    {
        printf("\nPress [Enter] to continue\n"); //prompt the user to press enter to continue to the next round
        scanf("%*c%*c"); //get the \n characters
        Game(round+1, score); //go the the next round
    }
    //if the game is over, insert score to leaderboard and prompt the user to press enter to go back to main menu
    else { SaveScore(score); ReturnToMenu(); }
}
void Dictionary()
{
    FILE *words = fopen(DictFile, "r"); //open the file
    //if file operation failed, print an error message and return
    if(FileError(words)) { printf("\nError : File Does Not Exist or is Empty\n"); return; }
	int WordCount; fscanf(words, "%d", &WordCount); //Get the number of words in the file
	char Dict[WordCount][MaxWordLen]; //store all the words from dictionary to be sorted

	for(int i = 0; i < WordCount; i++) fscanf(words, "%s", Dict[i]); //read all word input and store them
	MergeSort(Dict, 0, WordCount-1); //sort all words alphabetically (ascending from a to z)

	ClearScreen(); //clear the screen
	printf("Word Dictionary List :\n"); //prints title
	for (int i = 0; i < WordCount; i++) printf("%d. %s\n", i+1, Dict[i]); //prints all words from the dictionary
	fclose(words); //close the file
	ReturnToMenu(); //press enter to go back to main menu
}
void PrintLeaderboard()
{
    ClearScreen(); //clear the screen
    FILE *ScoreFile = fopen("Leaderboard.txt", "r"); //open score file for reading

    //if file does not exist or if it's empty, print no score message
    if(FileError(ScoreFile)) printf("There are no scores yet\n");
    else //else if the file exist and it's not empty
    {
        int DataCount; fscanf(ScoreFile, "%d", &DataCount); //get the number of data in the file
        printf("Leaderboard:\n"); //print header
        for(int i = 1; i <= DataCount; i++) //iterate through all the data in the file
        {
            char UserName[MaxNameLen]; int Score; //variables to store the username and the score
            fscanf(ScoreFile, "%s%d", UserName, &Score); //assign username and score data into that variables
            printf("%d. %s - %d\n", i, UserName, Score); //print data stored in that variables into the console
        }
    }

    fclose(ScoreFile); //close the file
    ReturnToMenu(); //prompt the use to press enter to go back to main menu
}
void Credits()
{
    ClearScreen(); //clear the screen
    printf("Made By :\n"); //Show the Creators of the app
    printf("Abid Rakhmansyah_2440077081\n");
    printf("Ardli Fadhillah Wangsaatmaja_2440029990\n");
    printf("Azella Gania Mutyara_2440035961\n");
    printf("Steven Yanuar Prasetyo Ginting_2440091722\n");
    ReturnToMenu(); //press enter to go back to main menu
}
void GetName()
{
    ClearScreen(); //clear the screen
    bool invalid = true; //to determine whether or not the name is valid, default it to true

    while(invalid) //keep prompting if it's still invalid
    {
        //Prompt the User to Input a Name, and put it into the Name Global Variable
        printf("Insert UserName : "); scanf(" %[^\n]", Name);
        //if the name exceeds the max length, output an error message and prompt again
        if(strlen(Name) >= MaxNameLen) { printf("Name must be less %d characters\n", MaxNameLen); continue; }

        invalid = false; //set invalid to false(assume it's valid)
        for(int i = 0; i < strlen(Name); i++) //iterate through all chars in the Name string
        {
            //if there is any illegal character, set invalid to true and break
            //alphabeta, numbers, and underscore is fine
            if(Name[i] != '_' && (Name[i] < '0' || (Name[i] > '9' && Name[i] < 'A') || (Name[i] > 'Z' && Name[i] < 'a') || Name[i] > 'z'))
                { invalid = true; break; }
        }
        //print error message if invalid is true
        if(invalid) printf("Name can only contain letters, numbers, and underscore\n");
    }
}
void MainMenu()
{
    ClearScreen(); //clear the screen
    //print Introduction Message
    printf("Welcome to Hangman, %s!\n", Name);
    printf("--------------------------\n");
    //Print all available Menus
    printf("1. Start Game\n");
    printf("2. Dictionary\n");
    printf("3. Leaderboard\n");
    printf("4. Credits\n");
    printf("5. Change Name\n");
    printf("0. Exit\n");

    char menu = '\0'; //to store what number the user entered
    while(menu < '0' || menu > '5') //keep prompting until the user entered a number from 0-5
    {
        printf(">> "); scanf(" %c", &menu); //prompt the user to Input a number to enter one of the menus
        if(menu < '0' || menu > '5') printf("Please Input 0-5\n"); //Print an error message if the user entered invalid input
    }

    if(menu == '0') return; //if the user entered 0, terminate the function (and the program)
    //if the user entered 1-5, then call the respective functions
    else if(menu == '1') Game(1, 0);
    else if(menu == '2') Dictionary();
    else if(menu == '3') PrintLeaderboard();
    else if(menu == '4') Credits();
    else if(menu == '5') { GetName(); MainMenu(); }
}
int main()
{
    GetName(); //prompt user to input their name
    MainMenu(); //show Main menu
    return 0; //terminate program
}

//Utilities
void ClearScreen()
{
    //if the program is running in windows, use system cls
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    //but, if it's running in linux, unix, or apple, use system clear
    #elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif
}
void ReturnToMenu()
{
    printf("\nPress [Enter] to return\n"); //prompt the user to press enter to go back to main menu
    scanf("%*c%*c"); //get the \n characters
    MainMenu(); //go back to main menu
}
void ToLower(char *str, int len)
{
    //for every characters in str
    for (int i = 0; i < len; i++)
    {
        //if it is uppercase, then change it to lowercase
        if(str[i] < 'a') str[i] += ('a'-'A');
    }
}
bool FileError(FILE *fp)
{
    //if file does not exists, close the file, and return true(indicates error)
    if(fp == NULL) { fclose(fp); return true; }
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
        //else if length of file is zero, close the file, and return true(indicates error)
        else { fclose(fp); return true; }
    }
}
void MergeArray(char (*arr)[MaxWordLen], int min, int mid, int max)
{
    int len1 = mid-min+1, len2 = max-mid; //get the length of the first and second half of the array
    //make a temporary array to store the first and the second half of the array
    char left[len1][MaxWordLen], right[len2][MaxWordLen];
    for(int i = 0; i < len1; i++) strcpy(left[i], arr[min+i]); //copy the first half of the array to the left array
    for(int i = 0; i < len2; i++) strcpy(right[i], arr[mid+1+i]); //copy the first half of the array to the right array

    int x = 0, y = 0; //set the starting index for the left array and the right array
    for(int i = min; i <= max; i++) //iterate from the min until max index from the original array
    {
        //if there are no more elements in the left/right array,
        //copy string from the other array to the merged array, increment that array's index, then continue
        if(x >= len1) { strcpy(arr[i], right[y++]); continue; }
        else if(y >= len2) { strcpy(arr[i], left[x++]); continue; }

        //if there are still some elements on both of the arrays,
        //compare the strings, and copy the smaller element(alphabetically) to the merged array
        if(strcmp(left[x], right[y]) < 0) strcpy(arr[i], left[x++]);
        else strcpy(arr[i], right[y++]);
    }
}
void MergeSort(char (*arr)[MaxWordLen], int min, int max)
{
    if(min >= max) return; //if min exceeds max, then return

    int mid = ((max-min)/2)+min; //get the middle point of max and min
    MergeSort(arr, min, mid); //divide and sort the first half of the array
    MergeSort(arr, mid+1, max); //divide and sort the second half of the array

    MergeArray(arr, min, mid, max); //merge and sort that first and second half of the array
}

