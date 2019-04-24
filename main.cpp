#include <iostream>
#include <vector>
#include "tile.h"
#include "player.h"
#include "card.h"
using namespace std;

// Function declarations
void buildDeck(vector<Card*> &deck);
void shuffleDeck(vector<Card*> &deck);
void buildIsland(vector<Tile*> &island, int size, int playerCount);
void shuffleIsland(vector<Tile*> &island);
void renderIsland(vector<Tile*> island, int size);
void initializePlayers(vector<Player*> &players, int playerCount);
void resources(vector<Player*> &players, int z);
void buySettlement(vector<Player*> &players, vector<Tile*> &island, int player, int size);
void buyPrompt(vector<Player*> &players, vector<Tile*> &island, int player, int size);
void tradePrompt(vector<Player*> &players, vector<Tile*> &island, int player, int size);
void takeTurn(vector<Player*> &players, vector<Tile*> &island, int player, int size);


void buildDeck(vector<Card*> &deck)
{
    for (int i = 0; i < 20; i++)
    {
        deck.push_back(new Card(i%5));
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
        cout << "What is Player " << (i + 1) << "'s name? ";
        cin >> name;
        players.push_back(new Player(name, 1, 1, 1, 1, 1, 0));
    }
    
    cout << "Welcome to the Island of Catan";
    
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
}

void resources(vector<Player*> &players, int z)
{
    cout << "Wood(" << players.at(z)->getWood();
    cout << ") Bricks(" << players.at(z)->getBricks();
    cout << ") Grain(" << players.at(z)->getGrain();
    cout << ") Wool(" << players.at(z)->getWool();
    cout << ") Ore(" << players.at(z)->getOre() << ")" << endl;
}

void buySettlement(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int row, column; //Variables which store inputs for row and column
    int index; //Variable which stores calculated index in the island vector
    
    cout << "BUYING A SETTLEMENT" << endl;
    cout << "On which row is the desired tile located?" << endl;
    
    for(int i = 1; i <= size; i++)
    {
        cout << "[" << i << "]" << endl;
    }
    
    cin >> row;
    
    while(row < 1 || row > size)
    {
        cout << "Error: Row is out of range." << endl;
        cout << "On which row is the desired tile located?" << endl;
        
        for(int i = 1; i <= size; i++)
        {
            cout << "[" << i << "]" << endl;
        }
        cin >> row;
    }
    
    cout << "On which column is the desired tile located?" << endl;
    
    for(int i = 1; i <= size; i++)
    {
        cout << "[" << i << "] ";
    }
    cout << endl;
    cin >> column;
    
    while(column < 1 || column > size)
    {
        cout << "Error: Column is out of range." << endl;
        cout << "On which column is the desired tile located?" << endl;
    
        for(int i = 1; i <= size; i++)
        {
            cout << "[" << i << "] ";
        }
        cout << endl;
        cin >> column;
    }
    
    //Calculates the index in the vector based off of the row and column input
    index = (size * (row - 1)) + (column - 1);
    cout << index << endl;
    cout << endl << "Vector index: " << (index + 1) << endl;
    cout << "Owner: " << island.at(index)->getOwner() << endl << endl;
    
    //Produce a message to the user if there is already a settlement at
    //the selected tile
    if(island.at(index)->getOwner() != -1)
    {
        renderIsland(island, size);
        cout << endl << "There is already a settlement on this tile." << endl;
        cout << "Please choose another tile." << endl << endl;
        buySettlement(players, island, player, size);
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
                if(island.at(1)->getOwner() == player || island.at(size)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either to the left of or below the chosen tile
                if(island.at(size - 1)->getOwner() == player || island.at((size * 2) - 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either to the left of, to the right of, or below the chosen tile
                if(island.at(column - 2)->getOwner() == player || island.at(column)->getOwner() == player || island.at(size + column - 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
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
                if(island.at(index + 1)->getOwner() == player || island.at(index - size)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either to the left of or above the chosen tile
                if(island.at(index - 1)->getOwner() == player || island.at(index - size)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either to the left of, to the right of, or above the chosen tile
                if(island.at(index - 1)->getOwner() == player || island.at(index + 1)->getOwner() == player || island.at(index - size)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
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
                if(island.at(index - size)->getOwner() == player || island.at(index + size)->getOwner() == player )
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user chose the last column (don't test tile to the right)
            else if(column == size)
            {
                //Verifies that the player owns a settlement either above, below, or to the left of the chosen tile
                if(island.at(index - size)->getOwner() == player || island.at(index + size)->getOwner() == player || island.at(index - 1)->getOwner() == player || island.at(index + 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
            
            //The user did not choose a side column (test both left and right)
            else
            {
                //Verifies that the player owns a settlement either above, below, to the right of, or to the left of the chosen tile
                if(island.at(index - size)->getOwner() == player || island.at(index + size)->getOwner() == player || island.at(index + 1)->getOwner() == player || island.at(index - 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player - 1)->modifyWood(-1);
                    players.at(player - 1)->modifyBricks(-1);
                    players.at(player - 1)->modifyGrain(-1);
                    players.at(player - 1)->modifyWool(-1);
                    players.at(player - 1)->modifyOre(-1);
                    renderIsland(island, size);
                    takeTurn(players, island, player, size);
                }
                else
                {
                    renderIsland(island, size);
                    cout << endl << "You don't own a settlement on an adjacent tile." << endl;
                    cout << "Please choose another tile." << endl << endl;
                    buySettlement(players, island, player, size);
                }
            }
        }
    }
}

void buyDevelopmentCard(vector<Player*> &players, vector<Card*> &deck, int player)
{
    int type = deck.at(deck.size() - 1)->getType();
    int input = 0;
    int resource = rand() % 5;
    
    if(type == 0 || type == 1 || type == 2)
    {
        players.at(player - 1)->modifyVictoryPoints(1);
    }
    else if(type == 3)
    {
        players.at(player - 1)->modifyWood(1);
        players.at(player - 1)->modifyBricks(1);
        players.at(player - 1)->modifyGrain(1);
        players.at(player - 1)->modifyWool(1);
        players.at(player - 1)->modifyOre(1);
    }
    else
    {
        cout << "Choose a player to steal a random resource from: ";
        cin >> input;
        
        while(input == player || input < 0 || input > players.size())
        {
            cout << "Invalid player choice." << endl;
            cout << "Choose a player to steal a random resource from: ";
            cin >> input;
        }
        
        players.at(input - 1)->
    }
}

void buyCity(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int row, column; //Variables which store inputs for row and column
    int index; //Variable which stores calculated index in the island vector
    
    cout << "BUYING A CITY" << endl;
    cout << "On which row is the desired tile located?" << endl;
    
    for(int i = 1; i <= size; i++)
    {
        cout << "[" << i << "]" << endl;
    }
    
    cin >> row;
    
    while(row < 1 || row > size)
    {
        cout << "Error: Row is out of range." << endl;
        cout << "On which row is the desired tile located?" << endl;
        
        for(int i = 1; i <= size; i++)
        {
            cout << "[" << i << "]" << endl;
        }
        cin >> row;
    }
    
    cout << "On which column is the desired tile located?" << endl;
    
    for(int i = 1; i <= size; i++)
    {
        cout << "[" << i << "] ";
    }
    cout << endl;
    cin >> column;
    
    while(column < 1 || column > size)
    {
        cout << "Error: Column is out of range." << endl;
        cout << "On which column is the desired tile located?" << endl;
    
        for(int i = 1; i <= size; i++)
        {
            cout << "[" << i << "] ";
        }
        cout << endl;
        cin >> column;
    }
    
    //Calculates the index in the vector based off of the row and column input
    index = (size * (row - 1)) + (column - 1);
    cout << index << endl;
    cout << endl << "Vector index: " << (index + 1) << endl;
    cout << "Owner: " << island.at(index)->getOwner() << endl << endl;
    
    //Produce a message to the user if there is already a settlement at
    //the selected tile
    if(island.at(index)->getOwner() != player && (island.at(index)->getOwner()) % 10 != player)
    {
        renderIsland(island, size);
        cout << endl << "This is not your Settlement" << endl;
        cout << "Please choose another tile." << endl << endl;
        buyCity(players, island, player, size);
    }
    else if(island.at(index)->getOwner() % 10 == player)
    {
        renderIsland(island, size);
        cout << endl << "You already have a city here." << endl;
        cout << "Please choose another tile." << endl << endl;
        buyCity(players, island, player, size);
    }
    
    //The selected tile is not already settled
    else
    {
        int land = island.at(index)->getLand(); //Save the landtype of the current tile
        int value = island.at(index)->getNumber(); //Sae the number of the current tile
        int owner = island.at(index)->getOwner();
        owner = owner * 10;
        island.at(index) = new cityTile((LandType)land, value, owner); 
        players.at(player - 1)->modifyWood(-1);
        players.at(player - 1)->modifyGrain(-3);
    }
}

void buyPrompt(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int choice = 0;
    
    while (choice != 4)
    {
        cout << "What would you like to buy?" << endl;
        cout << "1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)" << endl;
        cout << "2: City (2 ore, 3 grain)" << endl;
        cout << "3: Development Card  (1 ore, 1 grain, 1 wool)" << endl;
        cout << "4: Back" << endl;
        cin >> choice;
        
        //Test to ensure input is valid
        while (choice < 1 || choice > 4)
        {
            cout << "INVALID CHOICE!" << endl;
            cout << "1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)" << endl;
            cout << "2: City (2 ore, 3 grain)" << endl;
            cout << "3: Development Card (1 ore, 1 grain, 1 wool)" << endl;
            cout << "4: Back" << endl;
            cin >> choice;
        }
        
        //Fork to follow if user chooses to buy a settlement if they have enough resources
        if (choice == 1)
        {
            if(players.at(player - 1)->getWood() >= 1 && players.at(player - 1)->getBricks() >= 1 && players.at(player - 1)->getGrain() >= 1 && players.at(player - 1)->getWool() >= 1)
            {
                buySettlement(players, island, player, size); // theres an error
            }
            else
            {
                cout << "You do not have enough resources to build a settlement." << endl;
                takeTurn(players, island, player, size);
            }
        }
        else if (choice == 2)
        {
            if(players.at(player - 1)->getOre() >= 2 && players.at(player - 1)->getGrain() >= 3)
            {
                buyCity(players, island, player, size);//Call function for buying a city
            }
            else
            {
                cout << "You do not have enough resources to build a city." << endl;
                takeTurn(players, island, player, size);
            }
        }
        else if (choice == 3)
        {
            if(players.at(player - 1)->getOre() >= 1 && players.at(player - 1)->getGrain() >= 1 && players.at(player - 1)->getWool())
            {
                //Call function for buying a development card
            }
            else
            {
                cout << "You do not have enough resources to buy a development card." << endl;
                takeTurn(players, island, player, size);
            }
        }
    }
}

void tradePrompt(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int playerChoice, resourceChoice, amountChoice = 0;
    
    cout << "Who would you like to trade with?" << endl;
        
    for (int i = 0; i < players.size(); i++)
    {
        if(i != player - 1)
        {
            cout << i+1 << ": " << players.at(i)->getName() << " has ";
            resources(players, i);
        }
    }
    
    cin >> playerChoice;
    
    while(playerChoice < 1 || playerChoice > players.size() || playerChoice == player)
    {
        cout << "Invalid player choice." << endl;
        cout << "Who would you like to trade with?" << endl;
        
        for (int i = 0; i < players.size(); i++)
        {
            if(i != player - 1)
            {
                cout << i+1 << ": " << players.at(i)->getName() << " has ";
                resources(players, i);
            }
        }
        cin >> playerChoice;
    }
    
    cout << "Which resource would you like to give " << players.at(playerChoice-1)->getName() << "?" << endl;
    cout << "1: Wood" << endl;
    cout << "2: Brick" << endl;
    cout << "3: Grain" << endl;
    cout << "4: Wool" << endl;
    cout << "5: Ore" << endl;
    cin >> resourceChoice;
    
    switch(resourceChoice){
    case 1:
        cout << "How much wood ";
    }
    
}

void takeTurn(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int choice = 0; //Variable to store user menu input
    
    while (choice != 4)
    {
    //Prompt user for move choice
    cout << "What would you like to do?" << endl;
    cout << "1: Buy" << endl;
    cout << "2: Trade" << endl;
    cout << "3: Swap" << endl;
    cout << "4: End Turn" << endl;
    cin >> choice;
    
    //Test to ensure input is valid
    while (choice < 1 || choice > 4)
    {
        
        cout << "INVALID CHOICE!" << endl;
        cout << "1: Buy" << endl;
        cout << "2: Trade" << endl;
        cout << "3: Swap" << endl;
        cout << "4: End Turn" << endl;
        cin >> choice;
    }

    // Buy
    if (choice == 1)
        buyPrompt(players, island, player, size);
    // Trade
    else if (choice == 2)
        tradePrompt(players, island, player, size);
    //Swap three cards for one
    else if (choice == 3)
        //Implement code to swap cards
    }
}

int main()
{
    srand(time(0));
   // vector<Card*> deck;
    int size;
    int playerCount;
    int currentPlayer = 1;
    int roll = 0;
    
    vector<Tile*> island;
    vector<Player*> players;
    vector<Card*> deck;
    
    cout << "Enter a value for 'n' between 4 and 7 to create an 'n' by 'n' island: ";
    cin >> size;
    
    while (size < 4 || size > 7)
    {
        cout << "INVALID SIZE!" << endl;
        cout << "Enter a value for 'n' between 4 and 7 to create an 'n' by 'n' island: ";
        cin >> size;
    }
    
    cout << "How many players will there be? (2-4): ";
    cin >> playerCount;
    
    while (playerCount < 2 || playerCount > 4)
    {
        cout << "INVALID PLAYER COUNT!" << endl;
        cout << "How many players will there be? (2-4): ";
        cin >> playerCount;
    }
    initializePlayers(players, playerCount);
    buildIsland(island, size, playerCount);
    shuffleIsland(island);
    renderIsland(island, size);
    cout << "Island vector length: " << island.size() << endl; //DEBUGGING OUTPUT
    while(players.at(currentPlayer)->getVictoryPoints() < 10)
    {
        cout << players.at(currentPlayer)->getName() << "'s turn." << endl;
        resources(players, currentPlayer);
        roll = (rand() % 11 + 2);
        takeTurn(players, island, currentPlayer, size); 
        /* ********Look at this line and look at intialize players line, to make it easier for you******
        to see I'll copy and paste it here:
        When current players is intialized to zero and player count is taken its immediately sent up to initializePlayers
        takeTurn(players, island, currentPlayer + 1, size); causes you to skip over an entire player and when you want the last player to play their turn
        it bugs out
        But since initializePlayers FIRST value is actually zero, the program is skipping over an entire player
        I've tried tweaking it a bunch but with the if loop in intialize players to help print their name I keep getting another error which is
        the exact same thing that shows up. Ill keep looking at it, but 6 eyes are better than two :)
        void initializePlayers(vector<Player*> &players, int playerCount)
        {
            string name;
            for (int i = 0; i < playerCount; i++)
            {   
                cout << "What is Player " << (i + 1) << "'s name? ";
                cin >> name;
                players.push_back(new Player(name, 1, 1, 1, 1, 1, 0));
            }
        
        cout << "Welcome to the Island of Catan";
    
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
}

*/
        renderIsland(island, size);
        currentPlayer++;
        
        if(currentPlayer == playerCount)
        {
            currentPlayer = 0;
        }
    }
    return 0;
}
