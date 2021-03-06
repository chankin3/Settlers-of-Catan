#include <iostream>
#include <vector>
#include "tile.h"
#include "player.h"
#include "card.h"
using namespace std;

// Function declarations
void diceRoll(vector<Player*> &players, vector<Tile*> &island);
void buildDeck(vector<Card*> &deck);
void shuffleDeck(vector<Card*> &deck);
void buildIsland(vector<Tile*> &island, int size, int playerCount);
void shuffleIsland(vector<Tile*> &island);
void renderIsland(vector<Tile*> island, int size);
void initializePlayers(vector<Player*> &players, int playerCount);
void resources(vector<Player*> &players, int z);
void buySettlement(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);
void buyDevelopmentCard(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);
void buyCity(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);
void buyPrompt(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);
void tradePrompt(vector<Player*> &players, vector<Tile*> &island, int player, int size);
void swapResources(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);
int takeTurn(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size);

// Global variables
int player = 0;
int choice = 0;
int roll = 0;

void diceRoll(vector<Player*> &players, vector<Tile*> &island)
{
    int owner;
    int land;
    roll = (rand() % 11) + 2;
    cout << endl;
    cout << "\t\t\tDice roll: " << roll << endl;
    cout << "\t\t\t";
    //Go through each island tile
    for(int i = 0; i < island.size(); i++)
    {
        //Check that the current tile has an owner
        if(island.at(i)->getOwner() != -1)
        {
            //If the current tile is a city, get the owner
            if(island.at(i)->getOwner() / 10 > 0)
            {
                owner = (island.at(i)->getOwner()) / 10;
                if(roll == island.at(i)->getNumber())
                {
                    land = island.at(i)->getLand();
                    switch (land)
                    {
                        case 0:
                            players.at(owner - 1)->modifyWood(2);
                            break;
                        case 1:
                            players.at(owner - 1)->modifyBricks(2);
                            break;
                        case 2:
                            players.at(owner - 1)->modifyGrain(2);
                            break;
                        case 3:
                            players.at(owner - 1)->modifyWool(2);
                            break;
                        case 4:
                            players.at(owner - 1)->modifyOre(2);
                    }
                }
            }
            //If the current tile is just a settlement
            else
            {
                owner = island.at(i)->getOwner();
                if(roll == island.at(i)->getNumber())
                {
                    land = island.at(i)->getLand();
                    switch (land)
                    {
                        case 0:
                            players.at(owner - 1)->modifyWood(1);
                            break;
                        case 1:
                            players.at(owner - 1)->modifyBricks(1);
                            break;
                        case 2:
                            players.at(owner - 1)->modifyGrain(1);
                            break;
                        case 3:
                            players.at(owner - 1)->modifyWool(1);
                            break;
                        case 4:
                            players.at(owner - 1)->modifyOre(1);
                    }
                }
            }
        }
    }
}

void buildDeck(vector<Card*> &deck)
{
    for (int i = 0; i < 20; i++)
    {
        deck.push_back(new Card(i % 5));
    }
}

void shuffleDeck(vector<Card*> &deck)
{
    Card* temp;
    int i1, i2;
    for (int i = 0; i < 1000; i++)
    {
        i1 = rand() % deck.size();
        i2 = rand() % deck.size();
        temp = deck.at(i1);
        deck.at(i1) = deck.at(i2);
        deck.at(i2) = temp;
    }
}

void buildIsland(vector<Tile*> &island, int size, int playerCount)
{
    for (int i = 0; i < (size * size) - playerCount; i++)
    {
        island.push_back(new EmptyTile((LandType)(rand() % 5), (rand() % 11 + 2)));
    }
    for (int i = 1; i <= playerCount; i++)
    {
        island.push_back(new SettledTile((LandType)(rand() % 5), (rand() % 11 + 2), i));
    }
}

void shuffleIsland(vector<Tile*> &island)
{
    Tile* temp;
    int i1, i2;
    for (int i = 0; i < 1000; i++)
    {
        i1 = rand() % island.size();
        i2 = rand() % island.size();
        temp = island.at(i1);
        island.at(i1) = island.at(i2);
        island.at(i2) = temp;
    }
}

void renderIsland(vector<Tile*> island, int size)
{
    for (int s = 0; s < size; s++)
    {
        for (int i = 0; i <= 6; i++)
        {
            for (int j = size * s; j < island.size() / size + size * s; j++)
            {
                cout << island.at(j)->render(i) << " ";
            }
            cout << endl;
        }
    }
}

void initializePlayers(vector<Player*> &players, int playerCount)
{
    string name;
    for (int i = 0; i < playerCount; i++)
    {
        cout << "\t\t\tWhat is Player " << (i + 1) << "'s name? ";
        cin >> name;
        players.push_back(new Player(name, 1, 1, 1, 1, 1, 0));
    }
    cout << endl;
    cout << "\t-»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«-" << endl;
    cout << "\t-»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«-" << endl;
    cout << "\t             Welcome to the Island of Catan ";
    
    for (int i = 0; i < playerCount; i++)
    {
        if (i == playerCount - 1)
        {
            cout << " & " << players.at(i)->getName() << "." << endl;
        }
        else
        {
            cout << ", " << players.at(i)->getName();
        }
    }
    cout << "\t-»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«-" << endl;
    cout << "\t-»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«--»‡«-" << endl;
}

void resources(vector<Player*> &players, int z)
{
    cout << "    |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<< endl;
    cout << "    |~~~~~~~~~~~~~~~~ Victory Points: " << players.at(z)->getVictoryPoints() << " ~~~~~~~~~~~~~~~~|"<< endl;
    cout << "    |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|"<< endl << endl;
    cout << "    | Wood: " << players.at(z)->getWood();
    cout << "   Bricks: " << players.at(z)->getBricks();
    cout << "   Grain: " << players.at(z)->getGrain();
    cout << "   Wool: " << players.at(z)->getWool();
    cout << "   Ore: " << players.at(z)->getOre() << "|"<< endl << endl;
}

void buySettlement(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    int row, column; //Variables which store inputs for row and column
    int index; //Variable which stores calculated index in the island vector
    
    
    cout << "\t \tBUYING A SETTLEMENT" << endl;
    cout << "\tOn which row is the desired tile located?" << endl;
    for(int i = 1; i <= size; i++)
    {
        cout << "\t[" << i << "]" << endl;
    }
    
    cin >> row;
    
    while(row < 1 || row > size)
    {
        cout << "\t \tError: Row is out of range." << endl;
        cout << "\tOn which row is the desired tile located?" << endl;
        for(int i = 1; i <= size; i++)
        {
            cout << "\t[" << i << "]" << endl;
        }
        cin >> row;
    }
    
    cout << "\tOn which column is the desired tile located?" << endl;
    for(int i = 1; i <= size; i++)
    {
        cout << "\t[" << i << "] ";
    }
    cout << endl;
    cin >> column;
    
    while(column < 1 || column > size)
    {
        cout << "\t \tError: Column is out of range." << endl;
        cout << "\tOn which column is the desired tile located?" << endl;
    
        for(int i = 1; i <= size; i++)
        {
            cout << "\t[" << i << "] ";
        }
        cout << endl;
        cin >> column;
    }
    
    //Calculates the index in the vector based off of the row and column input
    index = (size * (row - 1)) + (column - 1);
    
    //Produce a message to the user if there is already a settlement at
    //the selected tile
    if(island.at(index)->getOwner() != -1)
    {
        renderIsland(island, size);
        cout << endl << "\tThere is already a settlement on this tile." << endl;
        cout << "\t \tPlease choose another tile." << endl << endl;
        buySettlement(players, island, deck, player, size);
    }
    
    //The selected tile is not already settled
    else
    {
        int land = island.at(index)->getLand(); //Save the landtype of the current tile
        int value = island.at(index)->getNumber(); //Sae the number of the current tile
        
        //The user chose the first row (don't test tile above)
        if(row == 1)
        { 
            //The user chose the first column (don't test tile to the left)
            if(column == 1)
            {
                //Verifies that the player owns a settlement either to the right of or below the chosen tile
                if(island.at(1)->getOwner() == (player + 1) || island.at(size)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either to the left of or below the chosen tile
                if(island.at(size - 1)->getOwner() == (player + 1) || island.at((size * 2) - 1)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either to the left of, to the right of, or below the chosen tile
                if(island.at(column - 2)->getOwner() == (player + 1) || island.at(column)->getOwner() == (player + 1) || island.at(size + column - 1)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
        }
        
        //The user chose the bottom row (don't test tile below)
        else if(row == size)
        {
            //The user chose the first column (don't test tile to the left)
            if(column == 1)
            {
                //Verifies that the player owns a settlement either to the right of or above the chosen tile
                if(island.at(index + 1)->getOwner() == (player + 1) || island.at(index - size)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\t\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either to the left of or above the chosen tile
                if(island.at(index - 1)->getOwner() == (player + 1) || island.at(index - size)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either to the left of, to the right of, or above the chosen tile
                if(island.at(index - 1)->getOwner() == (player + 1) || island.at(index + 1)->getOwner() == (player + 1) || island.at(index - size)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
        }
        
        //The user did not choose the top or bottom row (test both above and below)
        else
        {
            //The user chose the first column (don't test tile to the left)
            if(column == 1)
            {
                //Verifies that the player owns a settlement either above, below, or to the right of the chosen tile
                if(island.at(index - size)->getOwner() == (player + 1) || island.at(index + size)->getOwner() == (player + 1) || island.at(index + 1)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either above, below, or to the left of the chosen tile
                if(island.at(index - size)->getOwner() == (player + 1) || island.at(index + size)->getOwner() == (player + 1) || island.at(index - 1)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either above, below, to the right of, or to the left of the chosen tile
                if(island.at(index - size)->getOwner() == (player + 1) || island.at(index + size)->getOwner() == (player + 1) || island.at(index + 1)->getOwner() == (player + 1) || island.at(index - 1)->getOwner() == (player + 1))
                {
                    island.at(index) = new SettledTile((LandType)land, value, (player + 1));
                    players.at(player)->modifyWood(-1);
                    players.at(player)->modifyBricks(-1);
                    players.at(player)->modifyGrain(-1);
                    players.at(player)->modifyWool(-1);
                    players.at(player)->modifyVictoryPoints(1);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "\tYou don't own a settlement on an adjacent tile." << endl;
                    cout << "\t\tPlease choose another tile." << endl << endl;
                    buySettlement(players, island, deck, player, size);
                }
            }
        }
        renderIsland(island, size);
        takeTurn(players, island, deck, player, size);
    }
}

void buyDevelopmentCard(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    int type = deck.at((deck.size() - 1))->getType();
    deck.pop_back();
    deck.insert(deck.begin(), new Card(type));
    int input = 0;
    int resource = rand() % 5;
    
    players.at(player)->modifyOre(-1);
    players.at(player)->modifyGrain(-1);
    players.at(player)->modifyWool(-1);
    
    //Library card is drawn
    if(type == 0)
    {
        cout << "\t \tYou drew a Library card." << endl;
        cout << "\tYou have been awarded one victory point!" << endl << endl;
        players.at(player)->modifyVictoryPoints(1);
    }
    //University card is drawn
    else if(type == 1)
    {
        cout << "\t \tYou drew a University card." << endl;
        cout << "\tYou have been awarded one victory point!" << endl << endl;
        players.at(player)->modifyVictoryPoints(1);
    }
    //Town hall card is drawn
    else if(type == 2)
    {
        cout << "\t \tYou drew a Town Hall card." << endl;
        cout << "\tYou have been awarded one victory point!" << endl << endl;
        players.at(player)->modifyVictoryPoints(1);
    }
    //Harvest Bounty card is drawn
    else if(type == 3)
    {
        cout << "\t \tYou drew a Harvest Bounty card." << endl;
        cout << "\tYou have been awarded one of each resource!" << endl << endl;
        players.at(player)->modifyWood(1);
        players.at(player)->modifyBricks(1);
        players.at(player)->modifyGrain(1);
        players.at(player)->modifyWool(1);
        players.at(player)->modifyOre(1);
    }
    //Knight card is drawn
    else
    {
        cout << "\t\tYou drew a Knight card." << endl;
        cout << "\tChoose a player to steal a random resource from." << endl;
        for (int i = 0; i < players.size(); i++)
        {
            if(i != (player))
            {
                cout << i+1 << ": " << players.at(i)->getName() << endl;
            }
        }
        cin >> input;
        
        while(input == player || input < 0 || input > players.size())
        {
            cout << "\t\tInvalid player choice." << endl;
            cout << "\tChoose a player to steal a random resource from." << endl;
            for (int i = 0; i < players.size(); i++)
            {
                if(i != (player))
                {
                    cout << i+1 << ": " << players.at(i)->getName() << endl;
                }
            }
            cin >> input;
        }
        
        switch (resource)
        {
            case 0:
                if(players.at(input - 1)->getWood() > 0)
                {
                    players.at(input - 1)->modifyWood(-1);
                    cout << players.at(player)->getName() << " stole 1 wood from " << players.at(input - 1)->getName() << "!" << endl;
                }
                else
                {
                    cout << players.at(input - 1)->getName() << "does not have any wood. " << players.at(player)->getName() << " was gifted 1 wood." << endl;
                }
                players.at(player)->modifyWood(1);
                break;
            case 1:
                if(players.at(input - 1)->getBricks() > 0)
                {
                    players.at(input - 1)->modifyBricks(-1);
                    cout << players.at(player)->getName() << " stole 1 brick from " << players.at(input - 1)->getName() << "!" << endl;
                }
                else
                {
                    cout << players.at(input - 1)->getName() << "does not have any bricks. " << players.at(player)->getName() << " was gifted 1 brick." << endl;
                }
                players.at(player)->modifyBricks(1);
                break;
            case 2:
                if(players.at(input - 1)->getGrain() > 0)
                {
                    players.at(input - 1)->modifyGrain(-1);
                    cout << players.at(player)->getName() << " stole 1 grain from " << players.at(input - 1)->getName() << "!" << endl;
                }
                else
                {
                    cout << players.at(input - 1)->getName() << "does not have any grain. " << players.at(player)->getName() << " was gifted 1 grain." << endl;
                }
                players.at(player)->modifyGrain(1);
                break;
            case 3:
                if(players.at(input - 1)->getWool() > 0)
                {
                    players.at(input - 1)->modifyWool(-1);
                    cout << players.at(player)->getName() << " stole 1 wool from " << players.at(input - 1)->getName() << "!" << endl;
                }
                else
                {
                    cout << players.at(input - 1)->getName() << "does not have any wool. " << players.at(player)->getName() << " was gifted 1 wool." << endl;
                }
                players.at(player)->modifyWool(1);
                break;
            case 4:
                if(players.at(input - 1)->getOre() > 0)
                {
                    players.at(input - 1)->modifyOre(-1);
                    cout << players.at(player)->getName() << " stole 1 ore from " << players.at(input - 1)->getName() << "!" << endl;
                }
                else
                {
                    cout << players.at(input - 1)->getName() << "does not have any wool. " << players.at(player)->getName() << " was gifted 1 wool." << endl;
                }
                players.at(player)->modifyOre(1);
                break;
        }
    }
}

void buyCity(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    int row, column; //Variables which store inputs for row and column
    int index; //Variable which stores calculated index in the island vector
    
    cout << "\t \t!!!!!!BUYING A CITY!!!!!" << endl;
    cout << "\tOn which row is the desired tile located?" << endl;
    for(int i = 1; i <= size; i++)
    {
        cout << "\t[" << i << "]" << endl;
    }
    
    cin >> row;
    
    while(row < 1 || row > size)
    {
        cout << "\t!!!!!!Error: Row is out of range.!!!!!" << endl;
        cout << "\tOn which row is the desired tile located?" << endl;
        for(int i = 1; i <= size; i++)
        {
            cout << "\t[" << i << "]" << endl;
        }
        cin >> row;
    }
    
    cout << "\tOn which column is the desired tile located?" << endl;
    for(int i = 1; i <= size; i++)
    {
        cout << "\t[" << i << "] ";
    }
    cout << endl;
    cin >> column;
    
    while(column < 1 || column > size)
    {
        cout << "\tError: Column is out of range." << endl;
        cout << "\tOn which column is the desired tile located?" << endl;
        for(int i = 1; i <= size; i++)
        {
            cout << "\t[" << i << "] ";
        }
        cout << endl;
        cin >> column;
    }
    
    //Calculates the index in the vector based off of the row and column input
    index = (size * (row - 1)) + (column - 1);
    
    //Test to see if player owns a settlement here
    if(island.at(index)->getOwner() == (player + 1))
    {
        int land = island.at(index)->getLand(); //Save the landtype of the current tile
        int value = island.at(index)->getNumber(); //Sae the number of the current tile
        int owner = island.at(index)->getOwner();
        owner = owner * 10;
        island.at(index) = new cityTile((LandType)land, value, owner); 
        players.at(player)->modifyOre(-2);
        players.at(player)->modifyGrain(-3);
        players.at(player)->modifyVictoryPoints(1);
        renderIsland(island, size);
        takeTurn(players, island, deck, player, size);
    }
    else if((island.at(index)->getOwner() / 10) == (player + 1))
    {
        cout << "\tYou already own a city here." << endl;
        buyCity(players, island, deck, player, size);
    }
    else
    {
        cout << endl << "\tYou do not own a settlement here." << endl;
        cout << "\t \tPlease choose another tile." << endl << endl;
        buyCity(players, island, deck, player, size); 
    }
}

void buyPrompt(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    while (choice != 4)
    {
            cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
            cout << "\t|1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)|" << endl;
            cout << "\t|            2: City (2 ore, 3 grain)            |" << endl;
            cout << "\t|  3: Development Card (1 ore, 1 grain, 1 wool)  |" << endl;
            cout << "\t|                    4: Back                     |" << endl;
            cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
            cin >> choice;
        
        //Test to ensure input is valid
        while (choice < 1 || choice > 4)
        {
            cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
            cout << "\t|1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)|" << endl;
            cout << "\t|            2: City (2 ore, 3 grain)            |" << endl;
            cout << "\t|  3: Development Card (1 ore, 1 grain, 1 wool)  |" << endl;
            cout << "\t|                    4: Back                     |" << endl;
            cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
            cin >> choice;
        }
        
        //Fork to follow if user chooses to buy a settlement if they have enough resources
        if (choice == 1)
        {
            if(players.at(player)->getWood() >= 1 && players.at(player)->getBricks() >= 1 && players.at(player)->getGrain() >= 1 && players.at(player)->getWool() >= 1)
            {
                buySettlement(players, island, deck, player, size); // theres an error
            }
            else
            {
                cout << "\tYou do not have enough resources to build a settlement!" << endl;
                takeTurn(players, island, deck, player, size);
            }
        }
        else if (choice == 2)
        {
            if(players.at(player)->getOre() >= 2 && players.at(player)->getGrain() >= 3)
            {
                buyCity(players, island, deck, player, size);//Call function for buying a city
            }
            else
            {
                cout << "\t\tYou do not have enough resources to build a city!" << endl;
                takeTurn(players, island, deck, player, size);
            }
        }
        else if (choice == 3)
        {
            if(players.at(player)->getOre() >= 1 && players.at(player)->getGrain() >= 1 && players.at(player)->getWool() >= 1)
            {
                buyDevelopmentCard(players, island, deck, player, size);
            }
            else
            {
                cout << "\tYou do not have enough resources to buy a development card!" << endl;
                takeTurn(players, island, deck, player, size);
            }
        }
        else
        {
            takeTurn(players, island, deck, player, size);
        }
    }
}

void tradePrompt(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    //Initialize variables that will store information to conduct trade
    int playerChoice, giveRsrc, giveAmt, recRsrc, recAmt = 0;
    bool done = false;
    char response;
    
    //Do-while loop to validate player selection
    do
    {
        cout << "\t\tWho would you like to trade with?" << endl;
        for (int i = 0; i < players.size(); i++)
        {
            if(i != player)
            {
                cout << i+1 << ": " << players.at(i)->getName() << endl;
            }
        }
        cin >> playerChoice;
        
        if(playerChoice < 1 || playerChoice > players.size() || playerChoice == (player + 1))
        {
            cout << "\t!!!!!INVALID PLAYER SELECTION!!!!!" << endl;
        }
        else
        {
            done = true;
        }
        
    } while(!done);
    
    //Reset done to false
    done = false;
    
    //Do-while loop to validate resource type selection
    do
    {
        cout << "\t\t\tYou currently have" << endl;
        resources(players, player);
        cout << "\tWhich resource are you willing to give to " << players.at(playerChoice - 1)->getName() << "?" << endl;
        cout << "\t|!!!!!!!!!!!!!!!|" << endl;
        cout << "\t|    1: Wood    |" << endl;
        cout << "\t|    2: Brick   |" << endl;
        cout << "\t|    3: Grain   |" << endl;
        cout << "\t|    4: Wool    |" << endl;
        cout << "\t|    5: Ore     |" << endl;
        cout << "\t|!!!!!!!!!!!!!!!|" << endl;
        cin >> giveRsrc;
        
        if(giveRsrc < 1 || giveRsrc > 5)
        {
            cout << "\t\tInvalid resource selection." << endl;
        }
        else
        {
            done = true;
        }

    } while(!done);
    
    //Reset done to false
    done = false;
    
    //Do-while loop to validate quantity
    do
    {
        cout << "\t \tHow many are you willing to give? (1-5) " << endl;
        cin >> giveAmt;

        switch(giveRsrc)
        {
            case 1:
                if (players.at(player)->getWood() < giveAmt)
                {
                    cout << "\tYou dont have enough resources for that trade. Try again." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 2:
                if (players.at(player)->getBricks() < giveAmt)
                {
                    cout << "\tYou dont have enough resources for that trade. Try again." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 3:
                if (players.at(player)->getGrain() < giveAmt)
                {
                    cout << "\tYou dont have enough resources for that trade. Try again." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 4:
                if (players.at(player)->getWool() < giveAmt)
                {
                    cout << "\tYou dont have enough resources for that trade. Try again." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 5:
                if (players.at(player)->getOre() < giveAmt)
                {
                    cout << "\tYou dont have enough resources for that trade. Try again." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            default:
                cout << "\t \tInvalid trade amount." << endl;
                break;
        }
    } while(!done);
    
    //Reset done to false
    done = false;
    
    //Do-while loop to validate resource type selection
    do
    {
        cout << "\tWhich resource would you like to ask " << players.at(playerChoice-1)->getName() << " for?" << endl;
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cout << "\t|               1: Wood               |" << endl;
        cout << "\t|               2: Brick              |" << endl;
        cout << "\t|               3: Grain              |" << endl;
        cout << "\t|               4: Wool               |" << endl;
        cout << "\t|               5: Ore                |" << endl;
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cin >> recRsrc;
        
        if(recRsrc < 1 || recRsrc > 5)
        {
            cout << "\t\tInvalid resource selection" << endl;
        }
        else
        {
            done = true;
        }
    } while(!done);
    
    //Reset done to false
    done = false;
    
    //Do-while loop to validate quantity
    do
    {
        cout << "\tHow many would you like to receive? (1-5)" << endl;
        cin >> recAmt;
        
        switch(recRsrc)
        {
            case 1:
                if (players.at(playerChoice-1)->getWood() < recAmt)
                {
                    cout << "\t"<<players.at(playerChoice-1)->getName() << " doesn't have " << recAmt << " wood to trade." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 2:
                if (players.at(playerChoice-1)->getBricks() < recAmt)
                {
                    cout <<"\t"<< players.at(playerChoice-1)->getName() << " doesn't have " << recAmt << " bricks to trade." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 3:
                if (players.at(playerChoice-1)->getGrain() < recAmt)
                {
                    cout << "\t"<< players.at(playerChoice-1)->getName() << " doesn't have " << recAmt << " grain to trade." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 4:
                if (players.at(playerChoice-1)->getWool() < recAmt)
                {
                    cout << "\t"<< players.at(playerChoice-1)->getName() << " doesn't have " << recAmt << " wool to trade." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            case 5:
                if (players.at(playerChoice-1)->getOre() < recAmt)
                {
                    cout << "\t"<< players.at(playerChoice-1)->getName() << " doesn't have " << recAmt << " ore to trade." << endl;
                }
                else
                {
                    done = true;
                }
                break;
            default:
                cout << "\t\tInvalid trade amount." << endl;
                break;
        }
        
    } while(!done);
    
    cout << players.at(playerChoice-1)->getName() << ", " << players.at(player)->getName() << " wants " << recAmt << " of your ";
    switch(recRsrc)
    {
        case 1:
            cout << "wood";
            break;
        case 2:
            cout << "bricks";
            break;
        case 3:
            cout << "grain";
            break;
        case 4:
            cout << "wool";
            break;
        case 5:
            cout << "ore";
            break;
    }
    cout << " in exchange for " << giveAmt << " of their ";
    switch(giveRsrc)
    {
        case 1:
            cout << "wood." << endl;
            break;
        case 2:
            cout << "bricks." << endl;
            break;
        case 3:
            cout << "grain." << endl;
            break;
        case 4:
            cout << "wool." << endl;
            break;
        case 5:
            cout << "ore." << endl;
            break;
    }
    cout << "\t\tDo you accept? (y/n)" << endl;
    
    cin >> response;
    
    //Do-while loop to validate response
    do
    {
        if (response == 'Y' || response == 'y')
        {
            switch(recRsrc){
                case 1:
                    if (players.at(playerChoice-1)->getWood() < recAmt){
                        cout << players.at(playerChoice-1)->getName() << " does not have enough resources for that trade." << endl;
                    }
                    else if (giveRsrc == 1){
                        players.at(player)->modifyWood(recAmt);
                        players.at(player)->modifyWood(-giveAmt);
                        players.at(playerChoice-1)->modifyWood(-recAmt);
                        players.at(playerChoice-1)->modifyWood(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 2){
                        players.at(player)->modifyWood(recAmt);
                        players.at(player)->modifyBricks(-giveAmt);
                        players.at(playerChoice-1)->modifyWood(-recAmt);
                        players.at(playerChoice-1)->modifyBricks(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 3){
                        players.at(player)->modifyWood(recAmt);
                        players.at(player)->modifyGrain(-giveAmt);
                        players.at(playerChoice-1)->modifyWood(-recAmt);
                        players.at(playerChoice-1)->modifyGrain(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 4){
                        players.at(player)->modifyWood(recAmt);
                        players.at(player)->modifyWool(-giveAmt);
                        players.at(playerChoice-1)->modifyWood(-recAmt);
                        players.at(playerChoice-1)->modifyWool(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 5){
                        players.at(player)->modifyWood(recAmt);
                        players.at(player)->modifyOre(-giveAmt);
                        players.at(playerChoice-1)->modifyWood(-recAmt);
                        players.at(playerChoice-1)->modifyOre(giveAmt);
                        done = true;
                    }
                    break;
                case 2:
                    if (players.at(playerChoice-1)->getBricks() < recAmt){
                        cout << players.at(playerChoice-1)->getName() << " does not have enough resources for that trade!" << endl;
                    }
                    else if (giveRsrc == 1){
                        players.at(player)->modifyBricks(recAmt);
                        players.at(player)->modifyWood(-giveAmt);
                        players.at(playerChoice-1)->modifyBricks(-recAmt);
                        players.at(playerChoice-1)->modifyWood(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 2){
                        players.at(player)->modifyBricks(recAmt);
                        players.at(player)->modifyBricks(-giveAmt);
                        players.at(playerChoice-1)->modifyBricks(-recAmt);
                        players.at(playerChoice-1)->modifyBricks(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 3){
                        players.at(player)->modifyBricks(recAmt);
                        players.at(player)->modifyGrain(-giveAmt);
                        players.at(playerChoice-1)->modifyBricks(-recAmt);
                        players.at(playerChoice-1)->modifyGrain(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 4){
                        players.at(player)->modifyBricks(recAmt);
                        players.at(player)->modifyWool(-giveAmt);
                        players.at(playerChoice-1)->modifyBricks(-recAmt);
                        players.at(playerChoice-1)->modifyWool(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 5){
                        players.at(player)->modifyBricks(recAmt);
                        players.at(player)->modifyOre(-giveAmt);
                        players.at(playerChoice-1)->modifyBricks(-recAmt);
                        players.at(playerChoice-1)->modifyOre(giveAmt);
                        done = true;
                    }
                    break;
                case 3:
                    if (players.at(playerChoice-1)->getGrain() < recAmt){
                        cout << players.at(playerChoice-1)->getName() << " does not have enough resources for that trade!" << endl;
                    }
                    else if (giveRsrc == 1){
                        players.at(player)->modifyGrain(recAmt);
                        players.at(player)->modifyWood(-giveAmt);
                        players.at(playerChoice-1)->modifyGrain(-recAmt);
                        players.at(playerChoice-1)->modifyWood(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 2){
                        players.at(player)->modifyGrain(recAmt);
                        players.at(player)->modifyBricks(-giveAmt);
                        players.at(playerChoice-1)->modifyGrain(-recAmt);
                        players.at(playerChoice-1)->modifyBricks(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 3){
                        players.at(player)->modifyGrain(recAmt);
                        players.at(player)->modifyGrain(-giveAmt);
                        players.at(playerChoice-1)->modifyGrain(-recAmt);
                        players.at(playerChoice-1)->modifyGrain(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 4){
                        players.at(player)->modifyGrain(recAmt);
                        players.at(player)->modifyWool(-giveAmt);
                        players.at(playerChoice-1)->modifyGrain(-recAmt);
                        players.at(playerChoice-1)->modifyWool(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 5){
                        players.at(player)->modifyGrain(recAmt);
                        players.at(player)->modifyOre(-giveAmt);
                        players.at(playerChoice-1)->modifyGrain(-recAmt);
                        players.at(playerChoice-1)->modifyOre(giveAmt);
                        done = true;
                    }
                    break;
                case 4:
                    if (players.at(playerChoice-1)->getWool() < recAmt){
                        cout << players.at(playerChoice-1)->getName() << " does not have enough resources for that trade!" << endl;
                    }
                    else if (giveRsrc == 1){
                        players.at(player)->modifyWool(recAmt);
                        players.at(player)->modifyWood(-giveAmt);
                        players.at(playerChoice-1)->modifyWool(-recAmt);
                        players.at(playerChoice-1)->modifyWood(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 2){
                        players.at(player)->modifyWool(recAmt);
                        players.at(player)->modifyBricks(-giveAmt);
                        players.at(playerChoice-1)->modifyWool(-recAmt);
                        players.at(playerChoice-1)->modifyBricks(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 3){
                        players.at(player)->modifyWool(recAmt);
                        players.at(player)->modifyGrain(-giveAmt);
                        players.at(playerChoice-1)->modifyWool(-recAmt);
                        players.at(playerChoice-1)->modifyGrain(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 4){
                        players.at(player)->modifyWool(recAmt);
                        players.at(player)->modifyWool(-giveAmt);
                        players.at(playerChoice-1)->modifyWool(-recAmt);
                        players.at(playerChoice-1)->modifyWool(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 5){
                        players.at(player)->modifyWool(recAmt);
                        players.at(player)->modifyOre(-giveAmt);
                        players.at(playerChoice-1)->modifyWool(-recAmt);
                        players.at(playerChoice-1)->modifyOre(giveAmt);
                        done = true;
                    }
                    break;
                case 5:
                    if (players.at(playerChoice-1)->getOre() < recAmt){
                        cout << players.at(playerChoice-1)->getName() << " does not have enough resources for that trade." << endl;
                    }
                    else if (giveRsrc == 1){
                        players.at(player)->modifyOre(recAmt);
                        players.at(player)->modifyWood(-giveAmt);
                        players.at(playerChoice-1)->modifyOre(-recAmt);
                        players.at(playerChoice-1)->modifyWood(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 2){
                        players.at(player)->modifyOre(recAmt);
                        players.at(player)->modifyBricks(-giveAmt);
                        players.at(playerChoice-1)->modifyOre(-recAmt);
                        players.at(playerChoice-1)->modifyBricks(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 3){
                        players.at(player)->modifyOre(recAmt);
                        players.at(player)->modifyGrain(-giveAmt);
                        players.at(playerChoice-1)->modifyOre(-recAmt);
                        players.at(playerChoice-1)->modifyGrain(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 4){
                        players.at(player)->modifyOre(recAmt);
                        players.at(player)->modifyWool(-giveAmt);
                        players.at(playerChoice-1)->modifyOre(-recAmt);
                        players.at(playerChoice-1)->modifyWool(giveAmt);
                        done = true;
                    }
                    else if (giveRsrc == 5){
                        players.at(player)->modifyOre(recAmt);
                        players.at(player)->modifyOre(-giveAmt);
                        players.at(playerChoice-1)->modifyOre(-recAmt);
                        players.at(playerChoice-1)->modifyOre(giveAmt);
                        done = true;
                    }
                    break;
            }
            cout << endl << players.at(player)->getName() << " now has ";
            resources(players, player);
            cout << endl;
            cout << players.at(playerChoice-1)->getName() << " now has ";
            resources(players, playerChoice-1);
            cout << endl;
        }
        else if(response == 'N' || response == 'n')
        {
            cout << players.at(playerChoice-1)->getName() << " did not accept your trade" << endl;
        }
        else
        {
            cout << "!Invalid response!" << endl;
        }
    } while(!done);
    takeTurn(players, island, deck, player, size);
}

void swapResources(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    //Initialize variables which will store the type of resources that
    //the player selects
    int swpRsrc, recRsrc = 0;
    
    //Check that player has at least 3 of a resource
    if(players.at(player)->getWood() < 3 && players.at(player)->getBricks() < 3 && players.at(player)->getGrain() < 3 && players.at(player)->getWool() < 3 && players.at(player)->getOre() < 3)
    {
        cout << "\t!You need at least three of one resource to swap!" << endl << endl;
    }
    //If the player has enough of one resuorce
    else
    {
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cout << "\t|Which resource would you like to trade in?|" << endl;
        cout << "\t|                 1: Wood                  |" << endl;
        cout << "\t|                 2: Brick                 |" << endl;
        cout << "\t|                 3: Grain                 |" << endl;
        cout << "\t|                 4: Wool                  |" << endl;
        cout << "\t|                 5: Ore                   |" << endl;
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cin >> swpRsrc;
        
        //Check that the input for the selected resource is valid
        if (swpRsrc < 1 || swpRsrc > 5){
            cout << "\t\tInvaild response." << endl;
            swapResources(players, island, deck, player, size);
        }
        
        //Modify different properties of each player depending on the selected resource
        switch(swpRsrc){
            case 1:
                //Ensure that the player has enough wood
                if(players.at(player)->getWood() >= 3){
                    players.at(player)->modifyWood(-3);
                    break;
                }
                //Display error if player doesn't have enough wood
                else{
                    cout << "\t!You do not have enough of that resource to swap!" << endl << endl;
                    swapResources(players, island, deck, player, size);
                    break;
                    }
            case 2:
                //Ensure that the player has enough bricks
                if(players.at(player)->getBricks() >= 3){
                    players.at(player)->modifyBricks(-3);
                    break;
                }
                //Display error if player doesn't have enough bricks
                else{
                    cout << "\t!You do not have enough of that resource to swap!" << endl << endl;
                    swapResources(players, island, deck, player, size);
                    break;
                    }
            case 3:
                //Ensure that the player has enough grain
                if(players.at(player)->getGrain() >= 3){
                    players.at(player)->modifyGrain(-3);
                    break;
                }
                //Display error if player doesn't have enough grain
                else{
                    cout << "\t!You do not have enough of that resource to swap!" << endl << endl;
                    swapResources(players, island, deck, player, size);
                    break;
                    }
            case 4:
                //Ensure that the player has enough wool
                if(players.at(player)->getWool() >= 3){
                    players.at(player)->modifyWool(-3);
                    break;
                }
                //Display error if player doesn't have enough wool
                else{
                    cout << "\tYou do not have enough of that resource to swap." << endl;
                    swapResources(players, island, deck, player, size);
                    break;
                }
            case 5:
                //Ensure that the player has enough ore
                if(players.at(player)->getOre() >= 3){
                    players.at(player)->modifyOre(-3);
                    break;
                }
                //Display error if player doesn't have enough ore
                else{
                    cout << "\tYou do not have enough of that resource to swap." << endl;
                    swapResources(players, island, deck, player, size);
                    break;
                }
        }
        //Ask player what resource they would like to receive in exchange
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cout << "\t|Which resource would you like to receive?|" << endl;
        cout << "\t|                 1: Wood                 |" << endl;
        cout << "\t|                 2: Brick                |" << endl;
        cout << "\t|                 3: Grain                |" << endl;
        cout << "\t|                 4: Wool                 |" << endl;
        cout << "\t|                 5: Ore                  |" << endl;
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cin >> recRsrc;
        
        //Validate that the input is valid
        if (recRsrc < 1 || recRsrc > 5){
            cout << "\t\tInvaild response!" << endl;
            swapResources(players, island, deck, player, size);
        }
        
        //Give the player one resource of their choice
        switch(recRsrc){
            case 1:
                players.at(player)->modifyWood(1);
                break;
            case 2:
                players.at(player)->modifyBricks(1);
                break;
            case 3:
                players.at(player)->modifyGrain(1);
                break;
            case 4:
                players.at(player)->modifyWool(1);
                break;
            case 5:
                players.at(player)->modifyOre(1);
                break;
        }
    }
}

int takeTurn(vector<Player*> &players, vector<Tile*> &island, vector<Card*> &deck, int player, int size)
{
    while(players.at(player)->getVictoryPoints() < 10)
    {
        //Display the current players name and show the number of resources they have
        cout << players.at(player)->getName() << "'s turn." << endl << endl;
        resources(players, player);
        
        //Prompt user for move choice
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cout << "\t|What would you like to do?|" << endl;
        cout << "\t|         1: Buy           |" << endl;
        cout << "\t|         2: Trade         |" << endl;
        cout << "\t|         3: Swap          |" << endl;
        cout << "\t|        4: End Turn       |" << endl;
        cout << "\t|!!!!!!!!!!!!!!!!!!!!!!!!!!|" << endl;
        cin >> choice;
        
        //Test to ensure input is valid
        while (choice < 1 || choice > 4)
        {
            cout << "\t|!!!!!!!!!!!!!!!!|" << endl;
            cout << "\t|!INVALID CHOICE!|" << endl;
            cout << "\t|    1: Buy      |" << endl;
            cout << "\t|    2: Trade    |" << endl;
            cout << "\t|    3: Swap     |" << endl;
            cout << "\t|   4: End Turn  |" << endl;
            cout << "\t|!!!!!!!!!!!!!!!!|" << endl;
            cin >> choice;
        }
    
        // Buy
        if (choice == 1)
        {
            choice = 0;
            buyPrompt(players, island, deck, player, size);
        }
        // Trade
        else if (choice == 2)
        {
            choice = 0;
            tradePrompt(players, island, deck, player, size);
        }
        //Swap three resources for one
        else if (choice == 3)
        {
            choice = 0;
            swapResources(players, island, deck, player, size);
            //Implement code to swap resources
        }
        else
        {
            choice = 0;
            player++;
            if(player >= players.size())
            {
                player = 0;
            }
            //Show the island
            renderIsland(island, size);
            //Roll dice
            diceRoll(players, island);
            takeTurn(players, island, deck, player, size);
        }
    }
    return player;
}

int main()
{
    //Seed the random number generator
    srand(time(0));
    //Declare variables used to store size of n x n island
    //as well as number of players in the game
    int size;
    int playerCount;
    //Create vectors to store tile objects for the island,
    //player objects for each player, and card objects for
    //the development card deck
    vector<Tile*> island;
    vector<Player*> players;
    vector<Card*> deck;
    //Declare variable to store the winner
    int winner;
    cout << endl;
    //Prompt user to input size for an n x n island, then store in size variable
    cout << "\tEnter a value for 'n' between 4 and 7 to create an 'n' by 'n' island: ";
    cin >> size;
    //Validate size input
    while (size < 4 || size > 7)
    {
        cout << "\t!!!!!!!!!!!!!!!!!!!!!!!!!!!INVALID SIZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        cout << "\tEnter a value for 'n' between 4 and 7 to create an 'n' by 'n' island: ";
        cin >> size;
    }
    
    //Prompt user to input number of players
    cout << endl;
    cout << "\t\tHow many players will there be? (2-4): ";
    cin >> playerCount;
    cout << endl;
    //Validate input for number of players
    while (playerCount < 2 || playerCount > 4)
    {
        cout << "\t!!!!!!!!!!!!!!!!!!!INVALID PLAYER COUNT!!!!!!!!!!!!!!!!!!!" << endl;
        cout << "\t\tHow many players will there be? (2-4): ";
        cin >> playerCount;
        cout << endl;
    }
    
    //Build player vector
    initializePlayers(players, playerCount);
    //Build island vector
    buildIsland(island, size, playerCount);
    //Shuffle the elements in the island vector to promote randomness
    shuffleIsland(island);
    //Build development card deck
    buildDeck(deck);
    //Shuffle development card deck
    shuffleDeck(deck);
    //Show the island
    renderIsland(island, size);
    //Roll dice
    diceRoll(players, island);
    //Start first turn
    winner = takeTurn(players, island, deck, player, size);
    cout << players.at(winner)->getName() << " has " << players.at(winner)->getVictoryPoints() << " victory points. " << players.at(winner)->getName() << " wins!" << endl;
    return 0;
}
