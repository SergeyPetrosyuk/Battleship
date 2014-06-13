#include <stdio.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "windows.h"
using namespace std;

// INCLUDE CLASSES /////////////////////////
#include "game_menu.h"
#include "BaseMapObj.h"
#include "Scout.h"
#include "Fregat.h"
#include "Linkor.h"
#include "Aircraft.h"
#include "AntiAircraft.h"
// END INCLUDE CLASSES /////////////////////

struct strategy{
	int sX;
	int sY;
	int sDirect;
};

strategy firstStrategy[12];
strategy secondStrategy[12];
int currentStrategy = 0;

#define GAME_INIT	0
#define GAME_MENU	1
#define GAME_ENGINE 2
#define GAME_RUN	3
#define GAME_EXIT	4

// direction for ii logic shooting
#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3
#define UNKNOWN -1

// start coords for ii logic shooting
int startX = -1;
int startY = -1;
int currentX = -1;
int currentY = -1;
int currentDirect = UNKNOWN;
int parts = 0;

BaseMapObj *PlayerMap[12][12];
BaseMapObj *iiMap[12][12];

BaseMapObj *PlayerShips[12];
BaseMapObj *PlShipsParts[10];
BaseMapObj *iiShips[12];
BaseMapObj *iiShipsParts[10];
int p = 0;		// iterator for parts array
int plShots[12][12];	//shots on the map
int iiShots[12][12];	//shots on the map
int destroyedPlShips = 0;
int destroyediiShips = 0;
int iiLogicWork = 0;
	// 0 - dosn't work
	// 1 - works

int PlayerOrii = 0; // player
			 //= 1; // ii

void create_maps(BaseMapObj *createMaps[12][12]);
void create_ships(BaseMapObj *addShips[]);
void setShips(BaseMapObj *Map[12][12], BaseMapObj *Ships[12], BaseMapObj *partsOfShips[10], int aWhoPlay);
void printPlMap(BaseMapObj *Map[12][12]);
void printiiMap(BaseMapObj *aMap[12][12]);
void addShipsParts(int aP, int aX, int aY, BaseMapObj *shipHead, BaseMapObj *parts[10], BaseMapObj *MapForAdd[12][12]);
int checkFields(BaseMapObj *Maps[12][12], int pX, int pY, int size, int dir);
void printHeadOrBody(BaseMapObj *Object);
int isPlaceFull(BaseMapObj *Object);
void createStrategyIi();
void shooting(int whoShoot, int aX, char aY, BaseMapObj *friendShip, BaseMapObj *enemyMap[12][12]);
int charToInt(char aCh);
void iiSeesPlShip(int (&_array)[12][12], int &aX, char &aY);
void getupPlane(BaseMapObj *PlShip, BaseMapObj *aMap[12][12], int (&aShots)[12][12], int aX, char aY);
int checkCoords(int aX, char aY);
void checkAircraftShot(int aX, int aY, BaseMapObj *aMap[12][12], int (&aShots)[12][12]);
void iiShootingLogic(BaseMapObj *aMap[12][12], int (&aShots)[12][12], int *aX, int *aY);
int isOldShot(int testValue);
void setiiLogicDirect(int *aX, int *aY);
int isLastShootGood(int *aX, int *aY, int (&aShots)[12][12]);
int iiIsRightTarget(BaseMapObj *mainShip, BaseMapObj *shootedShip); 
void whatShipIsReady(string *Type, int *aI);
void main_menu();

int game_state;

int main()
{

	game_state = GAME_INIT;

	while(game_state != GAME_EXIT)
	{
		switch (game_state) 
		{
			case 0:
			{//GAME_INIT
				// CREATE MAPS //
				create_maps(PlayerMap);		// create a map for a pl.
				create_maps(iiMap);			// create a map for an i.i.
				// END create maps //

				// CREATE SHIPS //
				create_ships(PlayerShips);  // create ships for player
				create_ships(iiShips);		// create ships for i.i.

				createStrategyIi();
				// END create ships //

				for(int i = 0; i < 12; i++)
				{
					for(int j = 0; j < 12; j++)
					{
						plShots[i][j] = 0;
						iiShots[i][j] = 0;
					}//for
				}//for

				game_state = GAME_MENU;		// switch a game status to GAME_MENU
				break;
			}
			case 1:
			{//GAME_MENU
				main_menu();
				break;
			}
			case 2:
			{// GAME_ENGINE
				system("cls");
				cout << "Set your ships at the map >> \n\n";
				// set player's ships ////////////////////////
				int whoPlay = 0;	//humen plays
				setShips(PlayerMap, PlayerShips, PlShipsParts, whoPlay = 0);

				// set ii's ships     ////////////////////////
				whoPlay = 1;		//ii plays
				p = 0;
				setShips(iiMap, iiShips, iiShipsParts, whoPlay = 1);

				srand(time(NULL));
				currentStrategy = rand() % 1; //choosing the strategy

				game_state = GAME_RUN;
				break;
			}
			case 3:
			{//GAME_RUN
				int i = 9; // iterator of ships
				int itrForLetters = 0;
				while(true)
				{
					if(destroyediiShips == 22 || destroyedPlShips == 22)
						break;

					system("cls");
					printPlMap(PlayerMap);
					printiiMap(iiMap);
					//printPlMap(iiMap);

					// shooting ////////////////////////////////////////
				
					// player is shooting //////////////////////////////
					int x = 0;
					char y = ' ';
					char *letters = "abcdefghijkl";

					if(i > 0)
					{
						if(PlayerShips[i]->getStatusObj() == DESTROYED)
						{
							i--;

							if(i < 0)
								i = 9;

							continue;
						}

						string TypeShip = " ";
						whatShipIsReady(&TypeShip, &i);

						cout << "Yuo shoot with " << TypeShip << ". \n";
						cout << "Input coords for shooting by eney \n";
						cout << "Integer: ";
						cin >> x;

						cout << "Letter: ";
						cin >> y;

						if(!checkCoords(x, y))
						{
							system("cls");
							cout << "\n \n \n";
							cout << "             You have inputed wrong coords! Try again. \n";
							cout << "                    Pres any key to continue...";
							getch();
							continue;
						}

						shooting(PlayerOrii = 0, x, y, PlayerShips[i], iiMap);
						

						// player stoped to shoot ////////////////////////////


						// "ii" is shooting //////////////////////////////////


						if(parts == 0 && currentDirect != UNKNOWN)
						{
							iiLogicWork = 0;
							startX = -1;
							startY = -1;
							currentDirect = -1; //UNKNOWN
							currentX = -1;
							currentY = -1;
						}//if

						if(!iiLogicWork)
						{
							srand(time(NULL));
							x = rand() % 12;
							itrForLetters = rand() % 12;
							y = letters[itrForLetters];

							if(!isOldShot(iiShots[itrForLetters][x]))
							{
								for(int i = 0; i < 12; i++)
								{
									int stopFor = 0;
									for(int j = 0; j < 12; j++)
									{
										if(iiShots[i][j] == 0)
										{
											x = j;
											y = letters[i];
											stopFor = 1;
											break;
										}//if
									}//for
									if(stopFor)
										break;
								}//for
							}//if

							iiSeesPlShip(iiShots, x, y);
						}// if
						else
						{
							iiShootingLogic(PlayerMap, iiShots, &startX, &startY);
							x = currentX;
							y = letters[currentY];
						}// else

						//shooting(PlayerOrii = 0, x, y, PlayerShips[i], iiMap);
						shooting(PlayerOrii = 1, x, y, iiShips[i], PlayerMap);

					i--;
					if(i < 0)
						i = 9;
						
					}// if
					else
					{
						// get up player's planes ///////////////////////
						cout << "You can learn enemy's teretory with a plane. ";
						cout << "Input coords for shooting by eney \n";
						cout << "Integer: ";
						cin >> x;
						cout << "Letter: ";
						cin >> y;

						if(!checkCoords(x, y))
						{
							system("cls");
							cout << "You have inputed wrong coords! \n Try again. \n";
							cout << "Pres any key to continue...";
							getch();
						}

						if(PlayerShips[0]->getBullets() == 1)
							shooting(PlayerOrii = 0, x, y, iiShips[i], PlayerMap);
						else
							getupPlane(PlayerShips[0], iiMap, plShots, x, y);

						// get up "ii's" planes ///////////////////////////
						srand(time(NULL));
						x = rand() % 12;
						y = letters[rand() % 12];

						//if(!isOldShot(iiShots, x, y)) // if it is an old shot
						//	continue;

						if(iiShips[0]->getBullets() == 1)
							shooting(PlayerOrii = 1, x, y, iiShips[i], PlayerMap);
						else
							getupPlane(iiShips[0], PlayerMap, iiShots, x, y);

						i--;

						if(i < 0)
							i = 9;
					}// else
				}//while

				if(destroyediiShips == 22)
				{
					system("cls");
					cout << "My congratilations Soldier!!!\n";
					cout << "You won this buttle, but it isn't a finish of war... \n \n";
				}//if
				else
				{
					system("cls");
					cout << "Our country belived in you... \n";
					cout << "Your enemy was stronger";
				}//else

				cout << '\n' << '\n' << "Pres any key to exit.";
				getch();

				game_state = GAME_EXIT;
				break;
			}
			case 4:
			{//GAME_EXIT
				
				break;
			}
			default:
				break;
		}
	}//while

	return 0;
}//main

void create_maps(BaseMapObj * createMaps[12][12]) // + //
{
	// create pl. map //////////////////
	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			createMaps[i][j] = 0;
		}//for
	}//for
}// create_map()

void create_ships(BaseMapObj *addShips[])  // + //
{
	for(int i = 0; i < 12; i++)
	{
		if(i == 0)
		{
			addShips[i] = new Aircraft();
			addShips[i]->setShipId(i);
		}//if
		else if(i > 0 && i < 3)
		{
			addShips[i] = new Linkor();
			addShips[i]->setShipId(i);
		}//else if
		else if(i > 2 && i < 6)
		{
			addShips[i] = new Fregat();
			addShips[i]->setShipId(i);
		}//else if
		else if(i > 5 && i < 10)
		{
			addShips[i] = new Scout();
			addShips[i]->setShipId(i);
		}//else if
		else
		{
			addShips[i] = new AntiAircraft();
			addShips[i]->setShipId(i);
		}//else
	}//for
}// create_ships()

void main_menu()
{
	system("cls");
	cout << ">>> BATTLESHIP Game <<< \n\n";
	cout << "MAIN MENU \n \n";
	cout << "[1] - start a new game >> \n";
	cout << "[0] - exit >> \n";
	cout << "input: ";
	int choice;
	cin >> choice;

	switch (choice)
	{
		case 1:
		{
			game_state = GAME_ENGINE;
			break;
		}
		case 0: 
		{
			game_state = GAME_EXIT;
			break;
		}
		default:
			break;
	}
}// main_menu()

void setShips(BaseMapObj *Map[12][12], BaseMapObj *Ships[12], BaseMapObj *partsOfShips[10], int aWhoPlay) // + //
{
	for(int i = 11; i >= 0; i--)
	{
		int choice = 1; //choise of direction
		int positionX;  //position by horisontal
		int positionY;  //position by vertical
		char letter;

		if(!aWhoPlay) // if it is a player.
		{
			system("cls");
			printPlMap(PlayerMap);
			if(Ships[i]->getTypeObj() == SCOUT)
			{
				cout << "Choose position for SCOUT SHIP \n";
			}
			else if(Ships[i]->getTypeObj() == FREGAT)
			{
				cout << "Choose position for FREGAT SHIP \n";
			}
			else if(Ships[i]->getTypeObj() == LINKOR)
			{
				cout << "Choose position for LINKOR SHIP \n";
			}
			else if(Ships[i]->getTypeObj() == AIRCRAFT)
			{
				cout << "Choose position for AIRCRAFT SHIP \n";
			}
			else if(Ships[i]->getTypeObj() == ANTIAIRCRAFT)
			{
				cout << "Choose position for ANTIAIRCRAFT SHIP \n";
			}

			if(Ships[i]->getTypeObj() != SCOUT && Ships[i]->getTypeObj() != ANTIAIRCRAFT)
			{
				cout << '\n';

				cout << "Choose a direction \n";
				cout << "[0] - vertical \n";
				cout << "[1] - horizontal \n";
				cout << "input: ";
				cin >> choice;

				while(choice < 0 || choice > 1) // Choose the Direction
				{
					cout << "You inputed a wrong integer. \n";
					cout << "You should input 0 or 1";
					cout << "input: ";
					cin >> choice;
				}

				if(choice)
				{
					Ships[i]->setDirectObj(HORISONTAL);
				}
				else
				{
					Ships[i]->setDirectObj(VERTICAL);
				}
			}//if

			cout << "\n Choose the coords: \n";
			cout << "integer: ";
			cin >> positionY;

			if(positionY < 0 || positionY > 11)
			{
				cout << "You inputed a wrong integer. \n";
				cout << "Press any key to try again.";
				getch();
				i++;
				continue;
			}//if

			cout << "letter: ";
			cin >> letter;

			switch (letter)
			{
				case 'a':{positionX = 0; break;}
				case 'b':{positionX = 1; break;}
				case 'c':{positionX = 2; break;}
				case 'd':{positionX = 3; break;}
				case 'e':{positionX = 4; break;}
				case 'f':{positionX = 5; break;}
				case 'g':{positionX = 6; break;}
				case 'h':{positionX = 7; break;}
				case 'i':{positionX = 8; break;}
				case 'j':{positionX = 9; break;}
				case 'k':{positionX = 10; break;}
				case 'l':{positionX = 11; break;}
				default:
					{
						cout << "You inputed a wrong letter.";
						cout << "Press any key to try again.";
						i++;
						continue;
					}
			}//switch

			if(!isPlaceFull(Map[positionX][positionY]))
			{
				cout << "Not enough place to set your ship! \n";
				cout << "Press any key to try again...";
				getch();
				i++;
				continue;
			}//if
		}//if
		else // it's an "ii"
		{

			if(currentStrategy == 0)
			{
				positionX = firstStrategy[i].sY;
				positionY = firstStrategy[i].sX;
				choice = firstStrategy[i].sDirect;
			}
			else
			{
				positionX = secondStrategy[i].sY;
				positionY = secondStrategy[i].sX;
				choice = secondStrategy[i].sDirect;
			}

			if(choice)
			{
				Ships[i]->setDirectObj(HORISONTAL);
			}//if
			else
			{
				Ships[i]->setDirectObj(VERTICAL);
			}//else
		}//else

		switch (Ships[i]->getTypeObj())
		{
			case 0: // SCOUT
			{
				Ships[i]->setPositionX(positionX);
				Ships[i]->setPositionY(positionY);
				Map[positionX][positionY] = Ships[i];
				break;
			}
			case 1: // FREGAT
			{
				int check = checkFields(Map, positionX, positionY, Ships[i]->getSizeObj(), Ships[i]->getDirectObj());

				if(!check)
				{
					if(!aWhoPlay)
					{
						cout << "Not enough place to set your ship! \n";
						cout << "Press any key to try again...";
						getch();
					}//if
					
					i++;
					break;
				}//if

				Ships[i]->setPositionX(positionX);
				Ships[i]->setPositionY(positionY);
				Map[positionX][positionY] = Ships[i];
				addShipsParts(p, positionX, positionY, Ships[i], partsOfShips, Map);
				break;
			}
			case 2: // LINCOR
			{
				int check = checkFields(Map, positionX, positionY, Ships[i]->getSizeObj(), Ships[i]->getDirectObj());

				if(!check)
				{
					if(!aWhoPlay)
					{
						cout << "Not enough place to set your ship! \n";
						cout << "Press any key to try again...";
						getch();
					}//if

					i++;
					break;
				}//if

				Ships[i]->setPositionX(positionX);
				Ships[i]->setPositionY(positionY);
				Map[positionX][positionY] = Ships[i];

				addShipsParts(p, positionX, positionY, Ships[i], partsOfShips, Map);
				break;
			}
			case 3: // AIRCRAFT
			{
				int check = checkFields(Map, positionX, positionY, Ships[i]->getSizeObj(), Ships[i]->getDirectObj());

				if(!check)
				{
					if(!aWhoPlay)
					{
						cout << "Not enough place to set your ship! \n";
						cout << "Press any key to try again...";
						getch();
					}//if

					i++;
					break;
				}//if

				Ships[i]->setPositionX(positionX);
				Ships[i]->setPositionY(positionY);
				Map[positionX][positionY] = Ships[i];
				addShipsParts(p, positionX, positionY, Ships[i], partsOfShips, Map);
				break;
			}
			case 4: // ANTIAIRCRAFT
			{
				Ships[i]->setPositionX(positionX);
				Ships[i]->setPositionY(positionY);
				Map[positionX][positionY] = Ships[i];
				break;
			}
			default:
				break;
		}//switch
	}//for

	if(!aWhoPlay)
	{
		system("cls");
		printPlMap(PlayerMap);
		cout << "All your ships were set. \n";
		cout << "Press any key to start game.";
		getch();
	}

}// setPlShips()

int checkFields(BaseMapObj *Maps[12][12], int pY, int pX, int size, int dir) // + //
{
	int result = 0;

	if(dir) // if direction = HORIZONTAL
	{
		if(pX - 1 < 0)
		{
			return result = 0;
		}//if

		for(int i = 1; i < size; i++)
		{
			pX = pX - 1;
			if(Maps[pY][pX] == 0)
			{
				result = 1;
			}//if
			else
			{
				return result = 0;
			}//else
		}//for
		return result;
	}//if
	else //if direction = VERTICAL
	{
		if(pY + (size-1) > 11)
		{
			return result = 0;
		}//if
		for(int i = 1; i < size; i++)
		{
			pY = pY + 1;
			if(Maps[pY][pX] == 0)
			{
				result = 1;
			}//if
			else
			{
				return result = 0;
			}//else
		}//for
		return result;
	}//else
}

void addShipsParts(int aP, int aY, int aX, BaseMapObj *shipHead, BaseMapObj *parts[10], BaseMapObj *MapForAdd[12][12]) // + //
{
	for(int i = 0; i < shipHead->getSizeObj()-1; i++)
	{
		switch (shipHead->getTypeObj())
		{
			case 1: //FREGAT
			{
				parts[p] = new Fregat();
				break;
			}
			case 2: //LINKOR
			{
				parts[p] = new Linkor();
				break;
			}
			case 3: //AIRCRAFT
			{
				parts[p] = new Aircraft();
				break;
			}
			default:
				break;
		}

		if(shipHead->getDirectObj() == VERTICAL)
		{
			aY = aY + 1;
		}//if
		else
		{
			aX = aX - 1;
		}//else

		parts[p]->setPositionX(aX);
		parts[p]->setPositionY(aY);
		parts[p]->setDirectObj(VERTICAL);
		parts[p]->setHeadOrBody(BODY);
		parts[p]->setShipId(shipHead->getShipId());
		MapForAdd[aY][aX] = parts[p]; // add part on the map
		p++;
	}//for
}//addShipsParts()

void printPlMap(BaseMapObj *Map[12][12])
{
	char *letters = "abcdefghijkl"; // letters
	char forward = 62;  // horizontal direct
	char d = 197;		// destroyed ship or part
	char water = 178;   // empty field
	char miss = '*';	// miss shot
	char up = 94;		// vertical direct

	char *scoutShow = ">>"; // scout show
	char part = 219;		// part of ships
	char aac = 208;			// antiaircraft


	cout << "Player's Map \n\n";

	for(int i = 0; i < 12; i++) // integers
	{
		if(i > 9)
		{
			cout << i << ' ';
		}
		else
		{
			cout << ' ' << i << ' ';
		}
	}//for

	cout << '\n';
	cout << '\n';

	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			if(Map[i][j] == 0) // if there isn't a ship
			{
				if(iiShots[i][j] == 0)
					cout << water << water << ' '; // show water
				else
					cout << miss << miss << ' '; // show water
			}//if

			else 
			{
				switch (Map[i][j] -> getTypeObj())
				{
					case 0: //SCOUT
					{
						if(Map[i][j]->getStatusObj() == 1) // if a ship was destoyed
						{
							cout << ' ' << d << ' ';
						}//if
						else
						{ // A ship is life
							cout << scoutShow << ' ';
						}//else
						break;
					}
					case 1: //FREGAT
					{
						printHeadOrBody(Map[i][j]);
						break;
					}
					case 2: //LINKOR
					{
						printHeadOrBody(Map[i][j]);
						break;
					}
					case 3: //AIRCRAFT
					{
						printHeadOrBody(Map[i][j]);
						break;
					}
					case 4: //ANTIAIRCRAFT
					{
						if(Map[i][j]->getStatusObj() == 1) // if a ship was destoyed
						{
							cout << ' ' << d << ' ';
						}//if
						else
						{ // A ship is life
							cout << ' ' << aac << ' ';
						}//else
						break;
					}
					default:
						break;
				}//switch
			}//else
		}//for
		cout << "  " << letters[i];
		cout << '\n'<< '\n';
	}//for
}// printMap

void printHeadOrBody(BaseMapObj *Object)
{
	char forward = 62;  // horizontal direct
	char d = 197;		// destroyed ship or part
	char water = 178;   // empty field

	char up = 94;		// vertical direct

	char *scoutShow = ">>"; // scout show
	char part = 219;		// part of ships
	char aac = 208;			// antiaircraft

	if(Object->getStatusObj() == 1) // if a ship was destoyed
	{
		cout << ' ' << d << ' ';
	}//if
	else
	{ // A ship is life
		if(Object->getDirectObj() == VERTICAL)
		{
			if(Object->getHeadOrBody() == HEAD)
				cout << ' ' << up << ' ';
			else
				cout << ' ' << part << ' ';
		}//if
		else
		{
			if(Object->getHeadOrBody() == HEAD)
				cout << forward << "  ";
			else
				cout << ' ' << part << ' ';
		}
	}//else
}// printHeadOrBody()

// print the ii's map ///////////////////////////
void printiiMap(BaseMapObj *aMap[12][12])
{
	char *letters = "abcdefghijkl"; // letters
	char forward = 62;  // horizontal direct
	char d = 197;		// destroyed ship or part
	char water = 178;   // empty field
	char miss = '*';	// miss shot
	char up = 94;		// vertical direct

	char *scoutShow = ">>"; // scout show
	char part = 219;		// part of ships
	char aac = 208;			// antiaircraft

	cout << '\n' << '\n' << "Enemy's field \n";

	for(int i = 0; i < 12; i++) // integers
	{
		if(i > 9)
		{
			cout << i << ' ';
		}
		else
		{
			cout << ' ' << i << ' ';
		}
	}//for

	cout << '\n';
	cout << '\n';

	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			if(plShots[i][j] == 0)
			{
				cout << water << water << ' '; // show water
			}//if
			else if(plShots[i][j] == 1)
			{
				cout << miss << miss << ' '; // show water
			}//eise if
			else if(plShots[i][j] == 4) // 4 - Life enemy's ship
			{
				if(iiMap[i][j]->getHeadOrBody() == HEAD)
					cout << forward << "  ";
				else
					cout << ' ' << part << ' ';
				
			}// else if
			else
			{
				cout << ' ' << d << ' ';
			}//else
		}//for
		cout << "  " << letters[i];
		cout << '\n'<< '\n';
	}//for
}// printiiMap()

int isPlaceFull(BaseMapObj *Object) // + //
{
	if(Object == 0)
	{
		return 1;
	}//if
	return 0;
}// isPlaceFull()

void createStrategyIi()
{
	firstStrategy[11].sX = 3;
	firstStrategy[11].sY = 2;
	firstStrategy[11].sDirect = 1;

	firstStrategy[10].sX = 3;
	firstStrategy[10].sY = 8;
	firstStrategy[10].sDirect = 1;

	firstStrategy[9].sX = 0;
	firstStrategy[9].sY = 0;
	firstStrategy[9].sDirect = 1;

	firstStrategy[8].sX = 0;
	firstStrategy[8].sY = 11;
	firstStrategy[8].sDirect = 1;

	firstStrategy[7].sX = 11;
	firstStrategy[7].sY = 0;
	firstStrategy[7].sDirect = 1;

	firstStrategy[6].sX = 11;
	firstStrategy[6].sY = 11;
	firstStrategy[6].sDirect = 1;

	firstStrategy[5].sX = 5;
	firstStrategy[5].sY = 0;
	firstStrategy[5].sDirect = 0;

	firstStrategy[4].sX = 9;
	firstStrategy[4].sY = 9;
	firstStrategy[4].sDirect = 1;

	firstStrategy[3].sX = 11;
	firstStrategy[3].sY = 4;
	firstStrategy[3].sDirect = 1;

	firstStrategy[2].sX = 1;
	firstStrategy[2].sY = 7;
	firstStrategy[2].sDirect = 0;

	firstStrategy[1].sX = 8;
	firstStrategy[1].sY = 1;
	firstStrategy[1].sDirect = 0;

	firstStrategy[0].sX = 7;
	firstStrategy[0].sY = 5;
	firstStrategy[0].sDirect = 1;

	//--------------------------------//

	//secondStrategy
	secondStrategy[11].sX = 4;
	secondStrategy[11].sY = 9;
	secondStrategy[11].sDirect = 1;

	secondStrategy[10].sX = 8;
	secondStrategy[10].sY = 8;
	secondStrategy[10].sDirect = 1;

	secondStrategy[9].sX = 0;
	secondStrategy[9].sY = 0;
	secondStrategy[9].sDirect = 1;

	secondStrategy[8].sX = 0;
	secondStrategy[8].sY = 11;
	secondStrategy[8].sDirect = 1;

	secondStrategy[7].sX = 11;
	secondStrategy[7].sY = 0;
	secondStrategy[7].sDirect = 1;

	secondStrategy[6].sX = 11;
	secondStrategy[6].sY = 11;
	secondStrategy[6].sDirect = 1;

	secondStrategy[5].sX = 4;
	secondStrategy[5].sY = 4;
	secondStrategy[5].sDirect = 1;

	secondStrategy[4].sX = 6;
	secondStrategy[4].sY = 2;
	secondStrategy[4].sDirect = 0;

	secondStrategy[3].sX = 9;
	secondStrategy[3].sY = 4;
	secondStrategy[3].sDirect = 1;

	secondStrategy[2].sX = 10;
	secondStrategy[2].sY = 7;
	secondStrategy[2].sDirect = 1;

	secondStrategy[1].sX = 10;
	secondStrategy[1].sY = 9;
	secondStrategy[1].sDirect = 1;

	secondStrategy[0].sX = 6;
	secondStrategy[0].sY = 7;
	secondStrategy[0].sDirect = 0;

	//thirdStrategy[12];
}// createStrategyIi


void shooting(int whoShoot, int aX, char aY, BaseMapObj *friendShip, BaseMapObj *enemyMap[12][12])
{
	int avelableShots = friendShip->getBullets();
	int x = aX;
	int y = 0;

	y = charToInt(aY);

	while(avelableShots > 0)
	{
		if(enemyMap[y][x] == 0)
		{
			if(!whoShoot)
				plShots[y][x] = 1;
			else
				iiShots[y][x] = 1;
		}// if
		else
		{
			if(enemyMap[y][x]->getStatusObj() == DESTROYED)
			{
				if(!whoShoot)
					destroyediiShips--;
				else
					destroyediiShips--;
			}

			enemyMap[y][x]->setStatusObj(DESTROYED);
			if(!whoShoot)
			{
				plShots[y][x] = 3;
				destroyediiShips++;
			}//if
			else
			{
				destroyedPlShips++;
				iiShots[y][x] = 3;

				if(parts)
				{
					if(PlayerMap[startY][startX]->getShipId() == PlayerMap[currentY][currentX]->getShipId())
						parts--;
				}

				if(PlayerMap[y][x]->getSizeObj() > 1)
				{
					if(startX == -1 && startY == -1)
					{
						startX = x;
						startY = y;
						currentX = x;
						currentY = y;
						parts = PlayerMap[y][x]->getSizeObj()-1;
						iiLogicWork = 1; // switch logic on
					}
				}//if
			}//else
		}// else

		avelableShots--;

		if(!whoShoot && avelableShots != 0)
		{
			system("cls");
			printPlMap(PlayerMap);
			printiiMap(iiMap);

			cout << "You have " << avelableShots << " shots more! \n";
			cout << "Integer: ";
			cin >> x;
			cout << "Letter: ";
			cin >> aY;
			y = charToInt(aY);
		}//if
		else if(whoShoot && avelableShots != 0)
		{
			srand(time(NULL));
			char *letters = "abcdefghijkl";
			x = rand() % 12;
			aY = letters[rand() % 12];

			iiSeesPlShip(iiShots, x, aY);

			y = charToInt(aY);

			if(parts == 0 && currentDirect != UNKNOWN)
			{
				iiLogicWork = 0;
				startX = -1;
				startY = -1;
				currentDirect = -1; //UNKNOWN
				currentX = -1;
				currentY = -1;
			}//if

			if(!iiLogicWork)
			{
				char *letters = "abcdefghijkl";
				x = rand() % 12;
				aY = letters[rand() % 12];

				iiSeesPlShip(iiShots, x, aY);

				y = charToInt(aY);
			}// if
			else
			{
				iiShootingLogic(PlayerMap, iiShots, &startX, &startY);
				x = currentX;
				y = charToInt(letters[currentY]);
			}// else
		}// else if
	}// while

}// shooting

int charToInt(char aCh)
{
	int y = 0;
	switch (aCh)
		{
		case 'a':{y = 0; break;}
		case 'b':{y = 1; break;}
		case 'c':{y = 2; break;}
		case 'd':{y = 3; break;}
		case 'e':{y = 4; break;}
		case 'f':{y = 5; break;}
		case 'g':{y = 6; break;}
		case 'h':{y = 7; break;}
		case 'i':{y = 8; break;}
		case 'j':{y = 9; break;}
		case 'k':{y = 10; break;}
		case 'l':{y = 11; break;}
		default:
			return -1;
	}//switch
	return y;
}// charToInt

void iiSeesPlShip(int (&_array)[12][12], int &aX, char &aY)
{
	char *letters = "abcdefghijkl"; // letters
	for(int i = 0; i < 12; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			if(_array[i][j] == 4)
			{
				aX = j;
				aY = letters[i];
				return;
			}//if
		}//for
	}//for
}// iiSeesPlShip

void getupPlane(BaseMapObj *PlShip, BaseMapObj *aMap[12][12], int (&aShots)[12][12], int aX, char aY) // + //
{
	// check antiairctafts ////////////
	int y = charToInt(aY);
	int x = aX;

	for(int j = 0; j < 12; j++)
	{
		if(aMap[y][j] != 0)
		{
			if(aMap[y][j]->getTypeObj() == ANTIAIRCRAFT)
			{
				int planes = PlShip->getBullets();
				planes--;
				PlShip->setBullets(planes);
				return;
			}
		}// if
	}// for

	if(x == 0 && y == 0)
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(0, 1, aMap, aShots);
		checkAircraftShot(1, 0, aMap, aShots);
	}// if
	else if(x == 0 && y == 11)
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(0, 10, aMap, aShots);
		checkAircraftShot(1, 11, aMap, aShots);
	}// else if
	else if(x == 11 && y == 0)
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(10, 0, aMap, aShots);
		checkAircraftShot(11, 1, aMap, aShots);
	}// else if
	else if(x == 11 && y == 11)
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(10, 11, aMap, aShots);
		checkAircraftShot(11, 10, aMap, aShots);
	}// else if
	else if(x == 0 && (y > 0 && y < 11))
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(x+1, y, aMap, aShots);
		checkAircraftShot(x, y-1, aMap, aShots);
		checkAircraftShot(x, y+1, aMap, aShots);
	}// else if

	else if(x == 11 && (y > 0 && y < 11))
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(x-1, y, aMap, aShots);
		checkAircraftShot(x, y-1, aMap, aShots);
		checkAircraftShot(x, y+1, aMap, aShots);
	}// else if
	else if(y == 0 && (x > 0 && x < 11))
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(x, y+1, aMap, aShots);
		checkAircraftShot(x-1, y, aMap, aShots);
		checkAircraftShot(x+1, y, aMap, aShots);
	}// else if
	else if(y == 11 && (x > 0 && x < 11))
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(x, y-1, aMap, aShots);
		checkAircraftShot(x-1, y, aMap, aShots);
		checkAircraftShot(x+1, y, aMap, aShots);
	}// else if
	else
	{
		checkAircraftShot(x, y, aMap, aShots);
		checkAircraftShot(x, y-1, aMap, aShots);
		checkAircraftShot(x, y+1, aMap, aShots);
		checkAircraftShot(x-1, y, aMap, aShots);
		checkAircraftShot(x+1, y, aMap, aShots);
	}

}// getupPlane()

int checkCoords(int aX, char aY) // + //
{
	if(aX > 11 || aX < 0)
		return 0;
	else if(charToInt(aY) > 11 || charToInt(aY) < 0)
		return 0;
	else
		return 1;

}//checkCoords()

void checkAircraftShot(int aX, int aY, BaseMapObj *aMap[12][12], int (&aShots)[12][12]) // + //
{
	if(aMap[aY][aX] == 0)
	{
		aShots[aY][aX] = 1; // empty field
	}// if
	else if(aMap[aY][aX]->getStatusObj() == LIFE)
	{
		aShots[aY][aX] = 4; // live ship
	}// else if
	else
	{
		aShots[aY][aX] = 3; // destroyed ship
	}// else
}// checkAircraftShot()

void iiShootingLogic(BaseMapObj *aMap[12][12], int (&aShots)[12][12], int *aX, int *aY)
{
	int TypeOfShip = aMap[startY][startX]->getTypeObj();

	if(currentDirect == UNKNOWN)
	{
		setiiLogicDirect(aX, aY);
	}// if

	int stopWhile = 1;
	while(stopWhile)
	{
		switch (currentDirect)
		{
			case 0: //UP
			{
				if(!iiIsRightTarget(aMap[startY][startX], aMap[currentY][currentX]))
					break;

				if(currentY < 0 || aShots[currentY][currentX] == 1)
				{
					currentDirect = RIGHT;
					currentX = startX;
					currentY = startY;
				}//if
				else
				{
					currentY--;
					stopWhile = 0;
				}
				break;
			}
			case 1: //RIGHT
			{
				if(!iiIsRightTarget(aMap[startY][startX], aMap[currentY][currentX]))
					break;

				if(currentY < 0 || aShots[currentY][currentX] == 1)
				{
					currentDirect = DOWN;
					currentX = startX;
					currentY = startY;
				}//if
				else
				{
					currentX++;
					stopWhile = 0;
				}
				break;
			}
			case 2: //DOWN
			{
				if(!iiIsRightTarget(aMap[startY][startX], aMap[currentY][currentX]))
					break;

				if(currentY < 0 || aShots[currentY][currentX] == 1)
				{
					currentDirect = LEFT;
					currentX = startX;
					currentY = startY;
				}//if
				else
				{
					currentY++;
					stopWhile = 0;
				}
				break;
			}
			case 3: //LEFT
			{
				if(!iiIsRightTarget(aMap[startY][startX], aMap[currentY][currentX]))
					break;

				if(currentY < 0 || aShots[currentY][currentX] == 1)
				{
					currentDirect = UP;
					currentX = startX;
					currentY = startY;
				}//if
				else
				{
					currentX--;
					stopWhile = 0;
				}
				break;
			}
			default:
				break;
		}//switch
	}//while
}// iiShootingLogic()

int isOldShot(int testValue)
{
	if(testValue == 1 || testValue == 3)
		return 0; //it is an old shot of "i.i."
	else
		return 1;
}// isOldShot()

void setiiLogicDirect(int *aX, int *aY)
{
	if(*aX == 0 && *aY == 0)
	{
		currentDirect = RIGHT;
	}//if
	else if(*aX == 11 && *aY == 0)
	{
		currentDirect = DOWN;
	}//else if
	else if(*aX == 11 && *aY == 11)
	{
		currentDirect = LEFT;
	}//else if
	else if(*aX == 0 && *aY == 11)
	{
		currentDirect = UP;
	}//else if
	else if(*aX == 0 && (*aY > 0 && *aY < 11))
	{
		currentDirect = UP;
	}//else if
	else if(*aX == 11 && (*aY > 0 && *aY < 11))
	{
		currentDirect = UP;
	}//else if
	else if(*aY == 0 && (*aX > 0 && *aX < 11))
	{
		currentDirect = RIGHT;
	}//else if
	else if(*aY == 11 && (*aX > 0 && *aX < 11))
	{
		currentDirect = RIGHT;
	}//else if
	else
	{
		currentDirect = UP;
	}//else
}//setiiLogicDirect()

int isLastShootGood(int *aX, int *aY, int (&aShots)[12][12])
{
	if(aShots[currentY][currentX] == 3)
		return 1;
	else
		return 0;
}//isLastShootGood()

int iiIsRightTarget(BaseMapObj *mainShip, BaseMapObj *shootedShip)
{	// ѕопал ли "и.и." по обстрелеваему кораблю
	if(shootedShip != 0)
	{
		if(mainShip->getShipId() == shootedShip->getShipId())
		{
			return 1;
		}//if
		else{
			currentX = startX;
			currentY = startY;
			currentDirect++;

			if(currentDirect > 3)
				currentDirect = UP;

			return 0;
		}//else
	}//if
	else
	{
		currentX = startX;
		currentY = startY;
		currentDirect++;

		if(currentDirect > 3)
			currentDirect = UP;

		return 0;
	}//else
}// iiIsRightTarget()

void whatShipIsReady(string *Type, int *aI)
{
	switch (PlayerShips[*aI]->getTypeObj())
	{
		case 0:
		{
			*Type = "SCOUT";
			break;
		}
		case 1:
		{
			*Type = "FREGAT";
			break;
		}
		case 2:
		{
			*Type = "LINKOR";
			break;
		}
		case 3:
		{
			*Type = "AIRCRAFT";
			break;
		}
		default:
			break;
	}//switch
}//whatShipIsReady()