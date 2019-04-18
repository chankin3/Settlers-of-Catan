#include <iostream>
#include <vector>
#include "tile.h"
#include "player.h"

using namespace std;

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
    index = size * row - (size - column + 1);
    
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
                if(island.at(2)->getOwner() == player || island.at(size + 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                if(island.at(size - 1)->getOwner() == player || island.at(size * 2)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                if(island.at(column - 1)->getOwner() == player || island.at(column + 1)->getOwner() == player || island.at(size + column)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                if(island.at(index - size)->getOwner() == player || island.at(index + size)->getOwner() == player || island.at(index + 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                if(island.at(index - size)->getOwner() == player || island.at(index + size)->getOwner() == player || island.at(index - 1)->getOwner() == player)
                {
                    island.at(index) = new SettledTile((LandType)land, value, player);
                    players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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
                    //players.at(player)->modifyWood(players.at(player)->getWood() - 1);
                    //players.at(player)->modifyBricks(players.at(player)->getBricks() - 1);
                    //players.at(player)->modifyGrain(players.at(player)->getGrain() - 1);
                    //players.at(player)->modifyWool(players.at(player)->getWool() - 1);
                    //players.at(player)->modifyOre(players.at(player)->getOre() - 1);
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

void takeTurn(vector<Player*> &players, vector<Tile*> &island, int player, int size)
{
    int choice; //Variable to store user menu input
    
    //Prompt user for move choice
    cout << "What would you like to do?" << endl;
    cout << "1: Buy" << endl;
    cout << "2: Trade" << endl;
    cout << "3: End Turn" << endl;
    cin >> choice;
    
    //Test to ensure input is valid
    while (choice < 1 || choice > 3)
    {
        
        cout << "INVALID CHOICE!" << endl;
        cout << "1: Buy" << endl;
        cout << "2: Trade" << endl;
        cout << "3: End Turn" << endl;
        cin >> choice;
    }
<<<<<<< HEAD
    
    //Fork to follow if user chooses to buy
=======
    cout << "lmao" << endl;
>>>>>>> 34bfcf083d641717f6d8fd9a8f082bff6b75fe43
    if (choice == 1)
    {
        cout << "What would you like to buy?" << endl;
        cout << "1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)" << endl;
        cout << "2: City (2 ore, 3 grain)" << endl;
        cout << "3: Development Card (1 ore, 1 grain, 1 wool)" << endl;
        cout << "4: Quit" << endl;
        cin >> choice;
        
        //Test to ensure input is valid
        while (choice < 1 || choice > 4)
        {
            cout << "INVALID CHOICE!" << endl;
            cout << "1: Settlement (1 wood, 1 brick, 1 grain, 1 wool)" << endl;
            cout << "2: City (2 ore, 3 grain)" << endl;
            cout << "3: Development Card (1 ore, 1 grain, 1 wool)" << endl;
            cout << "4: Quit" << endl;
            cin >> choice;
        }
        
        //Fork to follow if user chooses to buy a settlement if they have enough resources
        if (choice == 1 && players.at(player)->getWood() > 0 && players.at(player)->getBricks() > 0 && players.at(player)->getGrain() > 0 && players.at(player)->getWool() > 0)
        {
            buySettlement(players, island, player, size);
        }
        //Fork that informs the player that they don't have enough resources if they choose to build a settlement
        else if(choice == 1 && (players.at(player)->getWood() == 0 || players.at(player)->getBricks() == 0 || players.at(player)->getGrain() == 0 || players.at(player)->getWool() == 0))
        {
            cout << "You do not have enough resources to build a settlement." << endl;
            takeTurn(players, island, player, size);
        }
        
    }
}

void resources(vector<Player*> &players, int z)
{
    cout << "Wood(" << players.at(z)->getWood();
    cout << ") Bricks (" << players.at(z)->getBricks();
    cout << ") Grain (" << players.at(z)->getGrain();
    cout << ") Wool (" << players.at(z)->getWool();
    cout << ") Ore (" << players.at(z)->getOre() << ")" << endl;
}

int main()
{
    srand(time(0));
    
    int size;
    int playerCount;
    int currentPlayer = 0;
    int roll = 0;
    
    vector<Tile*> island;
    vector<Player*> players;
    
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
        takeTurn(players, island, currentPlayer + 1, size);
        renderIsland(island, size);
        
        currentPlayer++;
        
        if(currentPlayer == playerCount)
        {
            currentPlayer = 0;
        }
    }
    return 0;
}
