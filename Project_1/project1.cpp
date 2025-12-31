#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

enum class TileColor { WHITE, BLACK };
enum class TileStatus { HIDDEN, REVEALED };

struct Tile {
    int number;
    TileColor color;
    TileStatus status;
};

class Player {
private:
    Tile tiles[13];
    int numTiles = 0;
public:
    void addTile(Tile tile) {
        tiles[numTiles] = tile;
        numTiles++;
    }

    Tile& getTile(int index) {
        if (index < numTiles) return tiles[index];
        else {
            cerr << "out of tile index!" << endl;
            exit(1);
        } 
    }

    int getNumTiles() {
        return numTiles;
    }

    void displayTiles(ofstream& ofs) {
        
        /* DO NOT MODIFY the output format! */

        sortTiles();
        for (size_t i = 0; i < numTiles; i++) {
            ofs << tiles[i].number;
            ofs << (tiles[i].color == TileColor::WHITE ? "w" : "b") << "(";
            ofs << (tiles[i].status == TileStatus::HIDDEN ? "H" : "R") << ")";
            if (i != numTiles - 1) ofs << ", ";
        }
    }

    bool isTileRevealed(int index) {
        if (index < numTiles) return tiles[index].status == TileStatus::REVEALED;
        else {
            cerr << "Out of tile index!" << endl;
            exit(1);
        }
    }

    void sortTiles() {
        
        /* TODO */
        for(int i=0;i<numTiles-1;i++)
        {
            for(int j=0;j<numTiles-i-1;j++)
            {
                if(tiles[j].number>tiles[j+1].number)
                {
                    Tile tmp = tiles[j];
                    tiles[j] = tiles[j+1];
                    tiles[j+1] = tmp; 
                }
                if(tiles[j].number == tiles[j+1].number)
                {
                    if(tiles[j].color == TileColor::WHITE)
                    {
                        Tile tmp = tiles[j];
                        tiles[j] = tiles[j+1];
                        tiles[j+1] = tmp;
                    }
                }
            }
        }

    }

    bool allTilesRevealed() {

        /* TODO: return true if one's tiles are all revealed */
        int check = 0;
        for(int i=0;i<numTiles;i++)
        {
            if(tiles[i].status == TileStatus::HIDDEN)
            {
                break;
            }
            else
            {
                check++;
            }
        }
        if(check == numTiles)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
};

class DaVinciCodeGame {
private:
    Tile centerTiles[24];
    Player players[4];
    int guess[4] = {0};
    int numCenter = 24;
    int numPlayers;
    int currentPlayerIndex;
    
public:
    DaVinciCodeGame(int num) {
        numPlayers = num;
        currentPlayerIndex = 0;
        initialize();
        initializeTiles();
        initializePlayerTiles();
    }

    void initialize() {

        /* If you need, initialize any variables. */

    }

    Tile popCenter() {
        if (numCenter < 0) {
            cerr << "Out of center tile index!" << endl;
            exit(1);
        }

        /* TODO: draw one tile from center tiles */
        numCenter--;
        return centerTiles[23-numCenter];

    }   

    void initializeTiles() {

        /* TODO: initialize tiles with input.txt file */
        ifstream f;
        string text;
        f.open("input12.txt");
        for(int i=0;i<24;i++)
        {
            f >> text;
            int size = text.size();
            if(size == 2)
            {
                centerTiles[i].number = text[0] - 48;
            }
            else
            {
                centerTiles[i].number = text[1] - 38;
            }
            if(text[size-1]=='w')
            {
                centerTiles[i].color = TileColor::WHITE;
            }
            else
            {
                centerTiles[i].color = TileColor::BLACK;
            }
                       
            centerTiles[i].status = TileStatus::HIDDEN;
        }

    }  

    void initializePlayerTiles() {
        /* TODO: make each player draw 4 tiles */
        for(int i=0;i<numPlayers;i++)
        {
            for(int j=0;j<4;j++)
            {
                players[i].addTile(centerTiles[4*i+j]);
                numCenter--;
            }
            players[i].sortTiles();
        }

    }

    void displayPlayersTiles(ofstream& ofs) {

        /* DO NOT MODIFY the output format! */

        for (int i = 0; i < numPlayers; ++i) {
            ofs << "Player " << i + 1 << "'s Tile : [ ";
            players[i].displayTiles(ofs);
            ofs << " ]";
            ofs << endl;
        }
    }

    bool makeGuess(int Player_number,int Tile_number,int Guess_number) {

        /* TODO: check and return if the guess was correct */
        if(players[Player_number].getTile(Tile_number).number == Guess_number)
        {
            players[Player_number].getTile(Tile_number).status = TileStatus::REVEALED;
            return true;
        }
        else
        {
            return false;
        }
        /* if the guess was correct, target tile should be revealed */

    }

    void play() {
        ofstream ofs;
        TileColor clue_color;
        int clue_number;
        bool turnContinue = false;

        ofs.open("output12.txt");
        if (!ofs.is_open()) {
            cerr << "Outout File error!" << endl;
            exit(1);
        }

        /* DO NOT MODIFY the output format! */
        ofs << "Game Start!" << endl;
        Tile Clue_Tile;


        while (true) {
            displayPlayersTiles(ofs);   

            // Do not modify the output format
            ofs << "--------------------------------------------------------------------------------------------------" << endl;  
            ofs << "Player " << currentPlayerIndex + 1 << "'s Turn: " << endl;
            Player& currentPlayer = players[currentPlayerIndex];
   
            if (!currentPlayer.allTilesRevealed() && !turnContinue) {  

                /* TODO: make currentPlayer recieve one clue tile */
                Clue_Tile = popCenter();
                currentPlayer.addTile(Clue_Tile);
                clue_number = Clue_Tile.number;
                clue_color = Clue_Tile.color;
                currentPlayer.sortTiles();

                /* DO NOT MODIFY the output format! */

                ofs << "Clue tile received: " << clue_number << (clue_color == TileColor::WHITE ? "w" : "b" ) << endl;
            }

            int targetPlayerIndex= 0;
            targetPlayerIndex = (currentPlayerIndex + 1) % numPlayers ;
            Player& targetPlayer = players[targetPlayerIndex];
            int targetTileIndex=0;
            int guessedNumber = 0; 


            /* TODO: guess number of target tile */
            while(true)
            {
                if(targetPlayer.isTileRevealed(targetTileIndex))
                {
                    targetTileIndex++;
                }
                else
                {
                    break;
                }
            }
            Tile targetTile = targetPlayer.getTile(targetTileIndex);
            if(targetTileIndex>0)
            {
                if(targetPlayer.getTile(targetTileIndex-1).number>=guess[currentPlayerIndex])
                {
                    if(targetPlayer.getTile(targetTileIndex-1).color==TileColor::BLACK)
                    {
                        if(targetTile.color==TileColor::BLACK)
                        {
                            guess[currentPlayerIndex]=targetPlayer.getTile(targetTileIndex-1).number+1;
                        }
                        else
                        {
                            guess[currentPlayerIndex]=targetPlayer.getTile(targetTileIndex-1).number;
                        }
                    }
                    else
                    {
                        guess[currentPlayerIndex]=targetPlayer.getTile(targetTileIndex-1).number+1;
                    }
                    
                }
                
            }
            for(int i=0;i<currentPlayer.getNumTiles();i++)
            {
                if(guess[currentPlayerIndex]==currentPlayer.getTile(i).number && targetTile.color == currentPlayer.getTile(i).color)
                {
                    guess[currentPlayerIndex]++;
                }
            }
            
            guessedNumber = guess[currentPlayerIndex];

            /* DO NOT MODIFY the output format! */

            ofs << endl << "Target index is: " << targetTileIndex + 1 << ", Guessed number is: " << guessedNumber << endl;

            if (makeGuess(targetPlayerIndex,targetTileIndex,guessedNumber)) {

                // Do not modify the output format
                if (targetPlayer.allTilesRevealed()) {
                    ofs << "\nCorrect guess!\n" ;
                    ofs << "Player " << targetPlayerIndex + 1 << " has lost!" << endl;
                    ofs << "Player " << currentPlayerIndex + 1 << " is the winner!" << endl;
                    return;
                }
                ofs << "Correct guess! Player " << currentPlayerIndex + 1 << " gets another turn. \n" << endl;
                turnContinue = true;
            } else {

                 /* DO NOT MODIFY the output format! */
                ofs << "Incorrect guess! Player " << currentPlayerIndex + 1 << " reveals a clue tile. \n" << endl;
                guess[currentPlayerIndex]++;
                if (!currentPlayer.allTilesRevealed()) {

                    /* TODO: reveal one tile of currentPlayer */
                    for(int i=0;i<currentPlayer.getNumTiles();i++)
                    {
                        if(currentPlayer.getTile(i).color == clue_color && currentPlayer.getTile(i).number == clue_number)
                        {
                            currentPlayer.getTile(i).status = TileStatus::REVEALED;
                        }
                    }

                }
                turnContinue = false;
            }

            if (targetPlayer.allTilesRevealed()) {

                /* DO NOT MODIFY the output format! */

                ofs << "Player " << targetPlayerIndex + 1 << " has lost!" << endl;
                ofs << "Player " << currentPlayerIndex + 1 << " is the winner!" << endl;
                return;
            }

            if (!turnContinue) {
                currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
            }
        }
        ofs.close();
    }
};

int main() {
    DaVinciCodeGame game(2);
    game.play();
    return 0;
}