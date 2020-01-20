//main.cpp
//bradley sundsbarm
//bsundsbarm
#include <iostream>
#include <limits.h>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>
#define INF INT_MAX

using namespace std;
int findTerrainCost(char t);
struct Node {
  int x;
  int y;
  char terrain;
  int cost;
  bool discovered;
  bool usedAsMove;
  int fn;
};

bool Astar(Node start, Node end, Node ** grid);
int heuristic(Node ** grid, int y, int x, int Ey, int Ex);
int height = 0;
int width = 0;
//moves is just a vector used for testing/debugging.
vector<int> moves;

int main (int argc, char *argv[]) {
  width = atoi(argv[1]);
  height = atoi(argv[2]);
  string g;
  string input;
  while (getline(cin, input)) {
    g = g + input;
  }
  int i = 0;
  //allocate memory
  Node ** grid = new Node*[height];
  for (i = 0; i < height; ++i)
    grid[i] = new Node[width];

  int counter = 0;
  for (i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      grid[i][j].terrain = g[counter];
      grid[i][j].x = j;
      grid[i][j].y = i;
      grid[i][j].discovered = false;
      grid[i][j].cost = findTerrainCost(g[counter]);
      grid[i][j].usedAsMove = false;
      grid[i][j].fn = 0;
      counter++;
    }
  }
  int Sx = 0; int Sy = 0; int Ex = 0; int Ey = 0;
  if (argc >= 3) {
    if (argc >= 5) {
      Sx = atoi(argv[3]);
      Sy = atoi(argv[4]);
    }
    Ex = width - 1;
    Ey = height - 1;
    if (argc == 7) {
      Ex = atoi(argv[5]);
      Ey = atoi(argv[6]);
    }
    grid[Sy][Sx].terrain = 'S';
    grid[Sy][Sx].usedAsMove = true;
    grid[Sy][Sx].cost = 0;
    moves.push_back(Sx); moves.push_back(Sy);
    grid[Ey][Ex].terrain = 'G';
    grid[Ey][Ex].cost = 0;
  }
  // for (i = 0; i < height; ++i) {
  //   for (int j = 0; j < width; ++j) {
  //     cout << grid[i][j].terrain;
  //   }cout << '\n';
  // }cout << '\n';cout << '\n';

  bool answer = Astar(grid[Sy][Sx], grid[Ey][Ex], grid);
  if (answer == false) {
    return -1;
  }
  for (i = 0; i < height; ++i)
    delete [] grid[i];
  delete[] grid;
  return 0;
}

//cur is the current position of the person moving on the map
bool Astar(Node start, Node end, Node ** grid) {
  int lastY = 0, lastX = 0;
  char lastmov = 'S';
  queue<Node> nodes_to_visit;
  queue<Node> evaluated;
  nodes_to_visit.push(start);
  while (!nodes_to_visit.empty()) {
    Node cur = nodes_to_visit.front();
    nodes_to_visit.pop();
    Node tmp;
    Node tmp2;
    char mov = ' ';
    //look down
    if (cur.y < height - 1 && grid[cur.y + 1][cur.x].usedAsMove == false) {
      tmp = grid[cur.y + 1][cur.x];
      tmp.fn = heuristic(grid, tmp.y, tmp.x, end.y, end.x);
      if (evaluated.empty()) {
        evaluated.push(tmp);
        mov = 'v';
      }
      else {
        tmp2 = evaluated.front();
        if (abs(tmp2.fn) > abs(tmp.fn)) {
          evaluated.pop();
          evaluated.push(tmp);
          mov = 'v';
        }
      }
    }
    //look left
    if (cur.x > 0 && grid[cur.y][cur.x - 1].usedAsMove == false) {
      tmp = grid[cur.y][cur.x - 1];
      tmp.fn = heuristic(grid, tmp.y, tmp.x, end.y, end.x);
      if (evaluated.empty()) {
        evaluated.push(tmp);
        mov = '<';
      }
      else {
        tmp2 = evaluated.front();
        if (abs(tmp2.fn) > abs(tmp.fn)) {
          evaluated.pop();
          evaluated.push(tmp);
          mov = '<';
        }
      }
      // cout << "left " << tmp.fn << endl;
    }
    //look right
    if (cur.x < width - 1 && grid[cur.y][cur.x + 1].usedAsMove == false) {
      tmp = grid[cur.y][cur.x + 1];
      tmp.fn = heuristic(grid, tmp.y, tmp.x, end.y, end.x);
      if (evaluated.empty()) {
        evaluated.push(tmp);
        mov = '>';
      }
      else {
        tmp2 = evaluated.front();
        if (abs(tmp2.fn) > abs(tmp.fn)) {
          evaluated.pop();
          evaluated.push(tmp);
          mov = '>';
        }
      }
      //cout << "right " << tmp.fn << endl;
    }
    //look up
    if (cur.y > 0 && grid[cur.y - 1][cur.x].usedAsMove == false) {
      tmp = grid[cur.y - 1][cur.x];
      tmp.fn = heuristic(grid, tmp.y, tmp.x, end.y, end.x);
      if (evaluated.empty()) {
        evaluated.push(tmp);
        mov = '^';
      }
      else {
        tmp2 = evaluated.front();
        if (abs(tmp2.fn) > abs(tmp.fn)) {
          evaluated.pop();
          evaluated.push(tmp);
          mov = '^';
        }
      }
      //cout << "up " << tmp.fn << endl;
    }
    tmp = evaluated.front();
    evaluated.pop();
    moves.push_back(tmp.y); moves.push_back(tmp.x);
    grid[tmp.y][tmp.x].usedAsMove = true;
    grid[tmp.y][tmp.x].terrain = mov;

    //change of previous terrain type if type of new move is different
    if ((lastmov != 'S') && (grid[tmp.y][tmp.x].terrain != lastmov)) {
     grid[lastY][lastX].terrain = grid[tmp.y][tmp.x].terrain;
    }

    lastmov = mov;
    lastY = tmp.y;
    lastX = tmp.x;

    //end case goal is reached.
    if (tmp.x == end.x && tmp.y == end.y) {
      grid[end.y][end.x].terrain = 'G';
      for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
          cout << grid[i][j].terrain;
        }
        cout << '\n';
      }
      return true;
    }

    // for (int i = 0; i < moves.size(); i++) {
    //   if (moves[i] != INT_MAX)
    //   cout << moves[i] << " ";
    // }
    // cout << '\n';

    //if (moves.size() < 24) {
    nodes_to_visit.push(tmp);
    //  } else {
    //    for (int i = 0; i < height; ++i) {
    //      for (int j = 0; j < width; ++j) {
    //        cout << grid[i][j].terrain;
    //     }
    //     cout << '\n';
    //   }
    // }
  }
  return false;
}

//for the heuristic function we first calculate the cost to get to the node
//then follow with the estimated cost to get to the goal node.  F(n) =  G(n) + H(n).
int heuristic(Node ** grid, int y, int x, int Ey, int Ex) {
  int fn = 0;
  int Gn = 0;
  int Hn = 0;
  Gn = grid[y][x].cost;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; j++) {
      if (grid[i][j].usedAsMove == true) {
        //cout << i << j << endl;
        Gn = Gn + grid[i][j].cost;
        //cout << "gn = " << Gn << " at " << i << j << endl;
      }
    }
  }
  fn = Gn;
  //now to get H(n) used 5 for my avg value since . and X are irrlevent
  //5 is the avg of 3 + 5 + 7 for the other terrain costs
  //since depending on the test we dont know if the position is below/above
  //or to the left or right.  So we test for each of these 4 cases and compute Hn.
  if (y < Ey) {
    for (int i = y; i < Ey + 1; i++) {
      Hn = Hn + 5;
    }
  }
  if (x < Ex) {
    for (int i = x; i < Ex + 1; i++) {
      Hn = Hn + 5;
    }
  }
  if (y > Ey) {
    for (int i = y; i > Ey + 1; i--) {
      Hn = Hn + 5;
    }
  }
  if (x > Ex) {
    for (int i = x; i > Ex + 1; i--) {
      Hn = Hn + 5;
    }
  }
  fn = fn + Hn;
  return fn;
}

//Pretty simple function takes a character matches it to the string and returns
//the value associated with the character
int findTerrainCost(char t) {
  string key = "!.!*!o!~x";
  int value = 0;
  int size = key.size();
  for (int i = 0; i < size; i++) {
    if (t == key[i])
      value = i;
  }
  if (value == 8)
    value = INF;
  return value;
}
