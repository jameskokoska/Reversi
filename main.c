#include <stdio.h>
#include <stdbool.h>

//Display the board with letter positions surrounding
void printBoard(char board[26][26], int n){
    printf("  ");
    //Print the first row of the board
    for(int index = 0; index<n; index++){
        printf("%c",'a'+index);
    }
    //Print the remaining board
    for(int row = 0; row<n; row++){
        printf("\n%c ",'a'+row);
        for(int col = 0; col<n; col++){
            printf("%c", board[row][col]);
        }
    }
}

//Check if a given position is within the board size
bool positionInBounds(int n, int row, int col){
    if((row>n || row<0)||(col>n || col<0)){
        return false;
    } else {
        return true;
    }
}

//Find, given position of a player, the valid moves around it
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol, int checkPair[]){
    char checkBoard;
    for(int change=1;change<n;change++) {
        checkBoard = board[row + change * deltaRow][col + change * deltaCol];
        //printf("\nCHECK: row: %d col: %d deltarow %d, deltaCol %d:", row, col, deltaRow, deltaCol);
        //printf("CHECKBOARD: %c, BESIDE: %c CHANGE %d\n", checkBoard, board[row + change * deltaRow + deltaRow][col + change * deltaCol + deltaCol],change);
        if ((checkBoard != colour) && (checkBoard != ' ') && positionInBounds(n, row + change * deltaRow + deltaRow, col + change * deltaCol + deltaCol) ==true && board[row + change * deltaRow + deltaRow][col + change * deltaCol + deltaCol] ==' ') { //if opposing colour team at this space, next space over in bounds and U
            if (checkPair == NULL) {
                printf("\n%c%c", 'a' + row + change * deltaRow + deltaRow,'a' + col + change * deltaCol + deltaCol);
                return true;
            } else if ((checkPair[0] == 'a' + row + change * deltaRow + deltaRow) &&(checkPair[1] == 'a' + col + change * deltaCol + deltaCol)) {
                checkPair[0] = -1;
                checkPair[1] = -1;
                return false;
            } else if ((checkPair[0] != 'a' + row + change * deltaRow + deltaRow) &&(checkPair[1] != 'a' + col + change * deltaCol + deltaCol)) {
                return false;
            }
        } else if (checkBoard != colour && checkBoard == board[row + change * deltaRow + deltaRow][col + change * deltaCol + deltaCol]){
            continue;
        }else {
            return false;
        }
    }
}

//Loop through the deltas and the alphabet to get the resisting checkLegalInDirection move in order
void checkLegalInDirectionLoop(char board[][26], int n, char colour, int checkPair[]){
    //Remove duplicate printing of moves
    int previousPair[2] = {0,0};

    //Loop through alphabet
    for (int checkRowLetter = 'a'; checkRowLetter < n+'a'; checkRowLetter++){
        for (int checkColLetter = 'a'; checkColLetter < n+'a'; checkColLetter++){

            //Loop through deltas
            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {

                    //Skip delta 0,0
                    if (deltaCol == 0 && deltaRow == 0)
                        continue;

                    //Loop through board
                    for (int row = 0; row < n; row++) {
                        for (int col = 0; col < n; col++) {
                            if (board[row][col] == colour) {
                                checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol, checkPair);
                            }
                        }
                    }
                }
            }
        }
    }
}

//Flip the tiles given a direction once found that the direction contains tiles to be flipped (the same colour exisists on the other side)
void flipTilesFound(char board[][26], int extend, int row, int col, char colour, int deltaRow, int deltaCol){
    for (int extend2 = 1; extend2 <= extend; extend2++) {
        //Keep flipping until the colour or U exists on the loop line
        if (board[row + deltaRow * extend2][col + deltaCol * extend2] == colour || board[row + deltaRow * extend2][col + deltaCol * extend2] == ' ') {
            break;
        } else {
            board[row + deltaRow * extend2][col + deltaCol * extend2] = colour;
        }
    }
}

//Find if a direction givens contains the same colour on another side
void flipTiles(char board[][26], int n, int row, int col, char colour){
    board[row][col] = colour;
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (deltaCol == 0 && deltaRow == 0){
                continue;
            }
            //Find how much to extend by, the length of the line to flip
            for (int extend = 1; extend < n; extend++){
                if(positionInBounds(n, row+extend*deltaRow, col+extend*deltaCol)==true && board[row+deltaRow*extend][col+deltaCol*extend] == colour){
                    flipTilesFound(board, extend, row, col, colour, deltaRow, deltaCol);
                } else {
                    continue;
                }
            }
        }
    }
}

int score(char board[][26], int n, char colour) {
    int score = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (board[row][col]==colour){
                score++;
            }
        }
    }
    return score;
}

void resetGarbageBoard(char board[][26], char garbageBoard[][26], int n){
    //Duplicate the board
    for(int row = 0; row<n; row++){
        for(int col = 0; col<n; col++){
            garbageBoard[row][col] = board[row][col];
        }
    }
}

bool computerPlay(char board[][26], char garbageBoard[][26], int n, char colour, bool play){
    int topScore=0, topRow = -1, topCol = -1;
    resetGarbageBoard(board, garbageBoard, n);
    for(int row = 0; row<n; row++){
        for(int col = 0; col<n; col++){
            int checkPair[2] = {row+'a',col+'a'};
            checkLegalInDirectionLoop(garbageBoard, n, colour, checkPair);
            if (checkPair[0] != -1 && checkPair[1] != -1){
            } else {
                flipTiles(garbageBoard,n,row,col, colour);
                if(topScore < score(garbageBoard, n, colour)){
                    topScore = score(garbageBoard, n, colour);
                    topRow = row;
                    topCol = col;
                }
                resetGarbageBoard(board, garbageBoard, n);
            }
        }
    }
    if (topRow!=-1 && topCol !=-1 && play == true){
        flipTiles(board,n,topRow,topCol, colour);
        printf("\nComputer places %c at %c%c.\n",colour, topRow+'a', topCol+'a');
        return true;
    } else if (topRow!=-1 && topCol !=-1 && play == false){
        return true;
    } else {
        return false;
    }
}

bool fullCheck(char board[][26], int n){
    for(int row = 0; row<n; row++) {
        for (int col = 0; col < n; col++) {
            if(board[row][col] == ' '){
                return false;
            }
        }
    }
    return true;
}

//Get input from the user
bool userInput(char board[][26], int n, char colour){
    //printf("Available Moves:");
    //checkLegalInDirectionLoop(board,n,colour,NULL);
    printf("\nEnter move for colour %c (RowCol): ", colour);
    char row, col;

    //Have extra character for enter
    scanf(" %c%c",&row,&col);

    //CheckPair used to remove duplicates
    int checkPair[2] = {row,col};
    checkLegalInDirectionLoop(board, n, colour, checkPair);

    if (checkPair[0] != -1 && checkPair[1] != -1 && fullCheck(board, n)==false){
        printf("Invalid move.\n");
        return true;
    } else {
        flipTiles(board,n,row-'a',col-'a', colour);
        return false;
    }
}

bool checkWin(char board[][26], int n, bool noMove1, bool noMove2){
    if(fullCheck(board, n)==true || (noMove1 ==false && noMove2 == false)) {
        if (score(board, n, 'W') < score(board, n, 'B')) {
            printf("\nB player wins.");
        } else {
            printf("\nW player wins.");
        }
        return true;
    } else {
        return false;
    }
}

int main() {
    int n;
    printf("Enter the board dimension: ");
    scanf("%d",&n);

    //Initialize the board to empty spaces
    char board[26][26];
    for (int row = 0; row<n; row++){
        for (int col = 0; col<n; col++){
            board[row][col]=' ';
        }
    }

    //Setup the middle spaces
    board[n/2][n/2-1] = 'B';
    board[n/2][n/2] = 'W';
    board[n/2-1][n/2] = 'B';
    board[n/2-1][n/2-1] = 'W';

    char compCol;
    printf("Computer plays (B/W): ");
    scanf(" %c",&compCol);

    printBoard(board, n);

    char garbageBoard[26][26];
    bool end = false;
    bool noMove1=true, noMove2=true;
    while(end==false){
        if (compCol == 'B'){
            if (computerPlay(board, garbageBoard, n, compCol, true)==true){
                printBoard(board, n);
            } else {
                printf("\n%c player has no valid move.", compCol);
                noMove2 = false;
            }

            if(checkWin(board,n,noMove1,noMove2)==true){
                break;
            }

            if (computerPlay(board, garbageBoard, n, 'W', false)==true){ //try and play as a computer to see if it has any valid moves
                end = userInput(board, n, 'W');
                if(end == true){
                    printf("%c player wins.", compCol);
                    break;
                } else {
                    printBoard(board, n);
                }
            } else {
                printf("W player has no valid move.");
                noMove1 = false;
            }

        }else {
            if (computerPlay(board, garbageBoard, n, 'B', false)==true){ //try and play as a computer to see if it has any valid moves
                end = userInput(board, n, 'B');
                if(end == true){
                    printf("%c player wins.", compCol);
                    break;
                } else {
                    printBoard(board, n);
                }
            } else {
                printf("B player has no valid move.");
                noMove1 = false;
            }

            if(checkWin(board,n,noMove1,noMove2)==true){
                break;
            }

            if (computerPlay(board, garbageBoard, n, compCol, true)==true){
                printBoard(board, n);
            } else {
                printf("\n%c player has no valid move.", compCol);
                noMove2= false;
            }
        }
        if(checkWin(board,n,noMove1,noMove2)==true){
            break;
        }
    }
    return 0;
}