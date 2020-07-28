#include <bits/stdc++.h>
#include<unistd.h>

using namespace std;

typedef enum GameStatus {progress, success , failed }gamestatus;
typedef enum BoxLocation {corner=3 ,edge = 5, middle = 8}BoxLocation;
class Mine
{
   bool found;

   public:
   bool getMinePresent()
   {
      return found;
   }
   void setMineFound(bool value)
   {
      found = value;
   }	
   void initializeMine(int);
   Mine() {}
};

class Tile
{
   bool suspect;
   int value;
   bool revealed;
   public:
   Mine *mine;
   bool getsuspect()
   {
      return suspect;
   }
   int getValue()
   {
      return value;
   }
   void setSuspect( bool set)
   {  
      suspect = set;
   }
   void setValue(int count)
   {
      value = count;
   }	
   bool getRevealedStatus()
   {
      return revealed;
   }
   void setTileRevealed(bool set)
   {
      revealed = set;
   }
   Tile ()  { }
   ~Tile()
   {
   }
};

class Minesweeper
{
   private:
      int rows, columns, numberOfMines;
      int *tilesOpened;
      static int opencount;
      int numberoftileleft;
      static Minesweeper *minesweep;
      Minesweeper() {}
   public:
      Tile *tiles;
      void decrementTileLeft()
      {
         numberoftileleft-=1;
      }
      void setTileSize(int row, int column)
      {
         rows = row;
         columns = column;
      }
      void addTilesOpened(int tileopened)
      {
         tilesOpened[opencount] = tileopened;
         opencount++;
      }
      int getTileDetails(int &row, int &column)
      {
         row = rows; 
         column = columns;
         return (rows*columns);
      }
      Tile* getTilePointer()
      {
         return tiles;
      }
      static Minesweeper* getInstance();
      static Minesweeper* deleteInstance();
      int getMineCount()
      {
         return numberOfMines;
      }	
      int * getTilesOpened()
      {
         return tilesOpened;
      }
      gamestatus getGameStatus()
      {	
         if (numberoftileleft == (numberOfMines) )
            return success;
         else 
            return progress;	
      }
      void initializeGame(int , int , int);
      void initializeTiles(Tile *);
      ~Minesweeper()
      {	
         delete [] tiles;
         delete [] tilesOpened;
         //delete minesweep;
      }
};

Minesweeper* Minesweeper :: minesweep = NULL;
Minesweeper* Minesweeper:: getInstance()
{
   if (minesweep == NULL)
      minesweep = new Minesweeper();
   return minesweep;
}

Minesweeper * Minesweeper :: deleteInstance()
{
   //  ~Minesweeper ();
   delete minesweep;

}

//Minesweeper *minesweep = Minesweeper::getInstance ();
int Minesweeper :: opencount = 0;

void Minesweeper :: initializeGame(int row, int column, int minecount)
{
   setTileSize (row, column);
   numberoftileleft = (row*column);
   numberOfMines = minecount;	
   tilesOpened= new int[row*column];
   for (int i=0; i<(row*column) ; i++)
      tilesOpened[i] = 0;

   tiles = new Tile[row*column];

   initializeTiles(tiles);

}
/*
   void Mine :: initializeMine(int minecount)
   {
   int i;
   for (i=1; i<= minecount; i++);
   this[i].found = false;
   }
   */
void Minesweeper :: initializeTiles(Tile *tile)
{
   int numberoftiles = rows*columns ;
   int i=0;
   for (i = 0; i<numberoftiles; i++)
   {
      tile[i].setSuspect( false);
      tile[i].setValue(-3);
      tile[i].mine = new Mine;
      tile[i].mine->setMineFound(false);	
      tile[i].setTileRevealed(false);
   }	
   //	mine= new Mine;
   //	mine->found = false;
   //	mine->initializeMine(minecount);
}

int getRandomNumber(int min, int max)
{
   sleep(1);
   srand(time(NULL));
   return (min + (rand() % (int)(max - min + 1)) );
}

int getBoxNumberFromRowandColumn( int row, int column)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ()) ->getTileDetails(rowtotal, columntotal);

   if( (row <= rowtotal) && (column <= columntotal) )
      return ( (columntotal * (row-1)) + column - 1) ; 
   return -1;
}

void getRowAndColumnFromBoxNumber(int tile, int &row, int &column)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   int diff;	
   /* to identify Row*/
   if (tile < columntotal)
      row =1;
   else
      row = (tile/columntotal) + 1;	
   /*else if ( (tile/columntotal) != 0)
     row = (tile/columntotal) + 1;
     else 
     row = (tile/columntotal); */

   /* to identify column*/

   if (((row*columntotal)-1) == tile )
      column = columntotal;
   else if ( ((row*columntotal)-1) > tile)
   {
      diff = (row*columntotal) - tile;
      column = columntotal - diff + 1;
   }
}
//GOPAL
BoxLocation getBoxPosition(int boxNumber)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   BoxLocation position;

   if ( (boxNumber == 0) || ( boxNumber == (columntotal-1)) || (boxNumber == (tiletotal-1)) || (boxNumber == (tiletotal-columntotal) ) ) 
      position = corner;
   else if ( ( (boxNumber % columntotal) == 0 ) || ( (boxNumber % columntotal) == (columntotal - 1) ) || (boxNumber < (columntotal-1)) || (boxNumber > (tiletotal-columntotal) ) )
      position = edge;
   else
      position = middle;

   return position;
}

void getAdjTilesForEdgeBox(int* adjTileList, int boxNumber)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);

   if((boxNumber % columntotal) == 0 )
   {
      adjTileList[0] = boxNumber + 1; 
      adjTileList[1] = boxNumber + columntotal;
      adjTileList[2] = boxNumber - columntotal;
      adjTileList[3] = boxNumber - columntotal + 1;
      adjTileList[4] = boxNumber + columntotal + 1;
   }
   else if((boxNumber % columntotal) == (columntotal - 1) )
   {
      adjTileList[0] = boxNumber - 1; 
      adjTileList[1] = boxNumber + columntotal;
      adjTileList[2] = boxNumber - columntotal;
      adjTileList[3] = boxNumber - columntotal - 1;
      adjTileList[4] = boxNumber + columntotal - 1;
   }	
   else if (boxNumber < (columntotal-1))
   {
      adjTileList[0] = boxNumber - 1; 
      adjTileList[1] = boxNumber + 1;
      adjTileList[2] = boxNumber + columntotal;
      adjTileList[3] = boxNumber + columntotal + 1;
      adjTileList[4] = boxNumber + columntotal - 1;
   }
   else if (boxNumber > (tiletotal-columntotal) )
   {
      adjTileList[0] = boxNumber - 1; 
      adjTileList[1] = boxNumber + 1;
      adjTileList[2] = boxNumber - columntotal;
      adjTileList[3] = boxNumber - columntotal + 1;
      adjTileList[4] = boxNumber - columntotal - 1;
   }
}

void getAdjTilesForMiddleBox(int* adjTileList, int boxNumber)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);

   adjTileList[0] = boxNumber + 1; 
   adjTileList[1] = boxNumber - 1;
   adjTileList[2] = boxNumber - columntotal;
   adjTileList[3] = boxNumber - columntotal + 1;
   adjTileList[4] = boxNumber - columntotal - 1;
   adjTileList[5] = boxNumber + columntotal ;
   adjTileList[6] = boxNumber + columntotal + 1;
   adjTileList[7] = boxNumber + columntotal - 1;
}
void getAdjTilesForCornerBox(int* adjTileList, int boxNumber)
{
   int rowtotal, columntotal;
   int tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   if (boxNumber == 0)
   {
      adjTileList[0] = boxNumber + 1;
      adjTileList[1] = boxNumber + columntotal;
      adjTileList[2] = boxNumber + columntotal + 1;
   }
   else if(boxNumber == (columntotal-1))
   {	
      adjTileList[0] = boxNumber - 1;
      adjTileList[1] = boxNumber + columntotal ;
      adjTileList[1] = boxNumber + columntotal - 1;
   }
   else if(boxNumber == (tiletotal-1))
   {
      adjTileList[0] = boxNumber - 1;
      adjTileList[1] = boxNumber - columntotal ;
      adjTileList[2] = boxNumber - columntotal - 1;
   }
   else if (boxNumber == (tiletotal-columntotal) )
   {
      adjTileList[0] = boxNumber + 1;
      adjTileList[1] = boxNumber - columntotal;
      adjTileList[2] = boxNumber - columntotal + 1;
   }
}
int* getAdjacentTiles(int boxnumber)
{
   int *adjTileList;	
   //int boxnumber = getBoxNumberFromRowandColumn(row, column);
   //int adjTilecount;
   BoxLocation position= getBoxPosition(boxnumber);
   switch (position)
   {
      case corner:
         adjTileList = new int [3];
         getAdjTilesForCornerBox(adjTileList, boxnumber);
         break;
      case edge:
         adjTileList = new int [5];
         getAdjTilesForEdgeBox(adjTileList, boxnumber);	
         break;
      case middle:
         adjTileList = new int [8];
         getAdjTilesForMiddleBox(adjTileList, boxnumber);
         break;
   }

   return adjTileList;
}

void setMines()
{
   int mineCount = (Minesweeper::getInstance ())->getMineCount();
   int rowtotal, columntotal;
   int gamesize = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   int i, *minelist;
   minelist = new int [mineCount];
   for (i=0; i<mineCount ; i++)
      minelist[i] = getRandomNumber(0, (gamesize-1));

   Tile *tile = (Minesweeper::getInstance ())->getTilePointer();
   for (i=0; i<mineCount; i++)
   {     tile[minelist[i]].mine->setMineFound(true);
      tile[minelist[i]].setValue(-1);
   }
   cout <<" Youve got "<<mineCount << " mines now, lets play. . . " <<endl;  

}

bool gettilestatus(int tilenumber)
{
   return ( (Minesweeper::getInstance ())->tiles[tilenumber].mine->getMinePresent() );
}

int getMinesAroundTheBox(int boxNumber)
{
   BoxLocation position;
   int i, count=0;
   int *adjTileList ;
   adjTileList = getAdjacentTiles(boxNumber);
   position = getBoxPosition(boxNumber);

   for (i=0; i<position ; i++)
   {
      if ( (Minesweeper::getInstance ())->tiles[ adjTileList[i] ].mine->getMinePresent() )
         ++count;
   }

   delete [] adjTileList;
   return count ;
}

void setValueForTile(int boxNumber)
{
   int value;
   value = getMinesAroundTheBox(boxNumber);
   if ((Minesweeper::getInstance ())->tiles[boxNumber].mine->getMinePresent() == false )
      (Minesweeper::getInstance ())->tiles[boxNumber].setValue(value);
}

void openCurrentBox(int boxNumber)
{
   if ((Minesweeper::getInstance ())->tiles[boxNumber].getRevealedStatus() == false )
   {
   (Minesweeper::getInstance ())->tiles[boxNumber].setTileRevealed(true);
   (Minesweeper::getInstance ())->decrementTileLeft();

   }
  // cout << "value of Box selected " << (Minesweeper::getInstance ())->tiles[boxNumber].getValue() <<endl;
}

void openAdjacentBox (int boxNumber)
{
   int * adjTileList;
   int i=0, row, column;
   adjTileList = getAdjacentTiles(boxNumber);
   BoxLocation position = getBoxPosition(boxNumber);
   for (i=0; i< position; i++)
   {
      if ( (Minesweeper::getInstance ())->tiles[adjTileList[i] ].getValue() != -1 )
      {
         if ((Minesweeper::getInstance ())->tiles[adjTileList[i]].getRevealedStatus() == false )
         {
         (Minesweeper::getInstance ())->decrementTileLeft();
         (Minesweeper::getInstance ())->tiles[adjTileList[i]].setTileRevealed(true);
         }
         //getRowAndColumnFromBoxNumber (adjTileList[i] , row, column);
 //        cout << "there are the adjacent box row = "<< row << "column = " << column << endl; 
   //      cout <<" their value is " << (Minesweeper::getInstance ())->tiles[adjTileList[i] ].getValue() <<endl;
      }
   }
   delete [] adjTileList;
}

void revealClues(int boxnumber)
{
   openCurrentBox(boxnumber);
   openAdjacentBox(boxnumber);
}

void printminedetails()
{
   int tiletotal, rowtotal, columntotal;
   cout<<"Rows count : "<<rowtotal<<"columns count : "<<columntotal<<endl;
   tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   for(int i=0; i<tiletotal; ++i)
   {   
      if(i%columntotal == 0)
         cout<<endl;
      if(((Minesweeper::getInstance())->tiles[i].getValue() == -1 ) || ((Minesweeper::getInstance ())->tiles[i].getRevealedStatus() == false))
         cout<<setw(2)<<-3<<" ";
      else
         cout<<setw(2)<<(Minesweeper::getInstance())->tiles[i].getValue()<<" ";
   }   
   cout<<endl;

}
void startGame()
{
   int userrow, usercolumn, tilenumber, i=0, tiletotal, rowtotal, columntotal;
   cout <<"please wait . . . initializing"<<endl;
   setMines();
   tiletotal = (Minesweeper::getInstance ())->getTileDetails(rowtotal, columntotal);
   for (i = 0; i<tiletotal; i++)
      setValueForTile(i);
   do
   {
      cout <<" Enter the row and column you want dig "<<endl;
      cout << "Row : ";
      cin >>userrow;
      cout <<endl <<"Column : ";
      cin >>usercolumn;
      cout <<endl;
      tilenumber = getBoxNumberFromRowandColumn(userrow,usercolumn);
      if( gettilestatus(tilenumber) )
      {
         cout <<" U digged a mine. Game Over. . . " <<endl;
         break;
      }
      else
      {
         revealClues(tilenumber);
      }
      printminedetails();
   }while((Minesweeper::getInstance () )->getGameStatus() == progress);

   if ((Minesweeper::getInstance () )->getGameStatus() == success)
         cout <<" Uve solved the mine... move to a greater size" <<endl;
}


int main()
{
   cout <<" Lets play Minesweeper for some time ....  "<<endl;
   cout <<" Enter rown and columns u want. " <<endl;
   int rows, columns, mines;
   cout << "Rows = ";
   cin >> 	rows;
   cout <<endl;
   cout << "Columns = ";
   cin >> 	columns;

   mines = rows;
   (Minesweeper::getInstance () )->initializeGame(rows,columns, mines);

   startGame();

   Minesweeper :: deleteInstance ();
   return 0;
}



