// Project 5

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <stdio.h>
#include <termios.h>
#include <ncurses.h>

using namespace std;

class maze
{
   public:
      maze(ifstream &fin);
      void print(int,int,int,int);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);
      int getMap(int i, int j) const;
      void mapMazeToGraph(graph &g);
      void findPath(int currentX, int currentY, graph &g);
      void findPathRec();

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
      matrix<int> map;      // Mapping from maze (i,j) values to node index values
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.4
{
    map[i][j] = n;
    return ;
}

int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
    return map[i][j];
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;

   char x;

   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
	 fin >> x;
	 if (x == 'O')
            value[i][j] = true;
	 else
	    value[i][j] = false;
      }

   map.resize(rows,cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
      {
	 if (i == goalI && j == goalJ)
	    cout << "*";
	 else
	    if (i == currI && j == currJ)
	       cout << "+";
	    else
	       if (value[i][j])
		  cout << " ";
	       else
		  cout << "X";
      }
      cout << endl;
   }
   cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    int nodeNumber = -1;
    int nodeKey;
    int diagCounter = 0;
    for (int j = 0; j <= rows-1; j++)
      for (int k = 0; k <= cols-1; k++)
      {
          if (value[j][k] == true)
          {
              nodeNumber++;
              node newNode = node();
              newNode.setId(nodeNumber);
              newNode.setWeight(1);
              g.addNode(newNode);
              map[j][k] = nodeNumber;
              if ((j > 0) && (k > 0))
              {
                  if (value[j-1][k-1])
                  {
                    nodeKey = map[j-1][k-1];
                    g.addEdge(nodeKey, nodeNumber, 1);
                    g.addEdge(nodeNumber, nodeKey, 1);
                  }
              }


              if (k>0)
              {
                  if (value[j][k-1])
                  {
                      nodeKey = map[j][k-1];
                      edge(nodeKey,nodeNumber);
                      g.addEdge(nodeKey, nodeNumber, 1);
                      g.addEdge(nodeNumber, nodeKey, 1);
                  }
              }

              if (j>0)
              {
                  if (value[j-1][k])
                  {
                      nodeKey = map[j-1][k];
                      g.addEdge(nodeKey, nodeNumber, 1);
                      g.addEdge(nodeNumber, nodeKey, 1);
                  }
              }

/*              if (j< cols-2)
              {
                  cout << "to the right";
                  if (value[j+1][k])
                  {
                      nodeKey = map[j+1][k];
                      g.addEdge(nodeKey, nodeNumber);
                      g.addEdge(nodeNumber, nodeKey);
                  }
              }
              if (k < rows-2)
              {
                  if (value[j][k+1])
                  {
                      nodeKey = map[j][k+1];
                      g.addEdge(nodeKey, nodeNumber);
                      g.addEdge(nodeNumber, nodeKey);
                  }
              }

              if ((k < rows-2) && (j < cols -2))
              {
                  if (value[j+1][k+1])
                  {
                      nodeKey = map[j+1][k+1];
                      g.addEdge(nodeKey, nodeNumber);
                      g.addEdge(nodeNumber, nodeKey);
                  }
              }
*/
          } else {
              map[j][k] = -1;
          }
      }

}

void maze::findPath(int currentX, int currentY, graph &g)
{
    cout << "push enter for next move";
    getchar();
    print(rows,cols,currentX,currentY);

  //  cin >> donut;
    bool mazeComplete;
    if (currentX == rows-1 && cols-1 == currentY)
    {
        cout << "maze complete!!!!";
        return;
    }

    int currentNode = getMap(currentX, currentY);

    g.visit(currentNode);
    int nextNode;//


    if (currentX+1 < rows && currentY+1 < cols)
    {
    nextNode = getMap(currentX+1, currentY+1);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked() && !g.getNode(nextNode).isVisited())
    {
        cout << "Go Down and to the right!";
        findPath(currentX+1, currentY+1, g);
        return;
    }
    }

    if (currentY+1 < cols)
    {
    nextNode = getMap(currentX, currentY+1);

    if ((nextNode != -1) && !g.getNode(nextNode).isMarked() && !g.getNode(nextNode).isVisited())
    {
        cout << "Go right!";
        findPath(currentX, currentY+1, g);
        return;
    }
    }

    if (currentX+1 < rows)
    {
    nextNode = getMap(currentX+1, currentY);
    if ((nextNode != -1) && !g.getNode(nextNode).isVisited() && !g.getNode(nextNode).isMarked())
    {
        cout << "Go down!";
        findPath(currentX+1, currentY, g);
        return;
    }
    }


    if (currentY > 0)
    {
    nextNode = getMap(currentX, currentY-1);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked() && !g.getNode(nextNode).isVisited())
    {
        cout << "Go left!";
        findPath(currentX, currentY-1, g);
        return;
    }
    }

    if (currentX > 0){
    nextNode = getMap(currentX-1, currentY);

    if ((nextNode != -1) && !g.getNode(nextNode).isMarked() && !g.getNode(nextNode).isVisited())
    {
        cout << "Go up!";
        findPath(currentX-1, currentY, g);
        return;
    }
    }

    if (currentX > 0 && currentY > 0){
    nextNode = getMap(currentX-1, currentY-1);

    if ((nextNode != -1) && !g.getNode(nextNode).isMarked() && !g.getNode(nextNode).isVisited())
    {
        cout << "Go up-left!";
        findPath(currentX-1, currentY-1, g);
        return;
    }
    }

    g.getNode(currentNode).mark();
    cout << endl << "This is a deadend!! Mark it for future reference" << endl;


    if (currentX+1 < rows && currentY+1 < cols)
    {
    nextNode = getMap(currentX+1, currentY+1);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go Down and to the right!";
        findPath(currentX+1, currentY+1, g);
        return;
    }
    }

    if (currentY+1 < cols)
    {
    nextNode = getMap(currentX, currentY+1);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go to the right!";
        findPath(currentX, currentY+1, g);
        return;
    }
    }

    if (currentX+1 < rows)
    {
    nextNode = getMap(currentX+1, currentY);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go Down!";
        findPath(currentX+1, currentY, g);
        return;
    }
    }


    if (currentY-1 > 0)
    {
    nextNode = getMap(currentX, currentY-1);
    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go left!";
        findPath(currentX, currentY-1, g);
        return;
    }
    }


    if (currentX - 1 > 0)
    {
    nextNode = getMap(currentX-1, currentY);

    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go up!";
        findPath(currentX-1, currentY, g);
        return;
    }
    }

    if (currentX - 1 > 0 && currentY - 1 > 0)
    {
    nextNode = getMap(currentX-1, currentY-1);

    if ((nextNode != -1) && !g.getNode(nextNode).isMarked())
    {
        cout << "Go up and to the left!";
        findPath(currentX-1, currentY-1, g);
        return;
    }
    }

    cout << "Go up!";
    findPath(currentX-1, currentY, g);
    }


int main()
{
   char x;
   ifstream fin;

    cout << "Main Maze Menu!" << endl << endl;
    cout << "[1] Maze1" << endl;
    cout << "[2] Maze2" << endl;
    cout << "[3] Maze3" << endl << endl;
    cout << "Enter the number of your choice :";

    int menuChoice;
    cin >> menuChoice;
    string fileName = "maze2.txt";

    if (menuChoice < 2)
    {
        fileName = "maze1.txt";
    }

    if (menuChoice == 2)
    {
        fileName = "maze2.txt";
    }

    if (menuChoice > 2)
    {
        fileName = "maze3.txt";
    }

   // Read the maze from the file.

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
//      exit(1);
   }

   try
   {

      graph g;
      while (fin && fin.peek() != 'Z')
      {
         maze m(fin);
         m.mapMazeToGraph(g);
         //cout << "here";
         m.findPath(0,0, g);
         cout << "here";
         //m.findPathRec();
      }
   }
   catch (indexRangeError &ex)
   {
//      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
  //    cout << ex.what() << endl; exit(1);
   }


   try {
           fileName = "maze3.txt";

      graph g2;

   fin.open(fileName.c_str());
      while (fin && fin.peek() != 'Z')
      {
         maze m(fin);
         m.mapMazeToGraph(g2);
         //cout << "here";
         m.findPath(0,0, g2);
         cout << "here";
         //m.findPathRec();
      }
   }

   catch (indexRangeError &ex)
   {
//      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
  //    cout << ex.what() << endl; exit(1);
   }
       //

      //cout << g;



//         cout >> m.cols;



}
