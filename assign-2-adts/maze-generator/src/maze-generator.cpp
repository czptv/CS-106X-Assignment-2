/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"
#include "maze-types.h"
#include "vector.h"
#include "set.h"

static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
	while (true) {
		int response = getInteger(prompt);
		if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
		cout << "Please enter a number between "
			 << minDimension << " and " 
		     << maxDimension << ", inclusive." << endl;
	}
}

/*
 * generate all the inner walls
 */

static void generateWall(int dimension, int i, int j, cell & c, Vector<wall> & walls, MazeGeneratorView & maze) {
    wall w;
    w.one = c;
    if ((j + 1) % dimension != 0) { //vertical walls
        cell c2;
        c2.row = i;
        c2.col = j + 1;
        w.two = c2;
        walls.add(w);
        maze.drawWall(w);
    }
    if ((i + 1) % dimension != 0) { // horizontal walls
        cell c2;
        c2.row = i + 1;
        c2.col = j;
        w.two = c2;
        walls.add(w);
        maze.drawWall(w);
    }
}

/*
 * Generate all the cells and organize them into chambers
 */

static void generateChambers(int dimension, Vector<Set<cell>> & chambers, Vector<wall> & walls, MazeGeneratorView & maze) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            cell c;
            c.row = i;
            c.col = j;
            generateWall(dimension, i, j, c, walls, maze);
            Set<cell> chamber;
            chamber.add(c);
            chambers.add(chamber);
        }
    }
    maze.drawBorder();
}

/*
 * check whether two cells are contained in the same chamber
 */

static bool checkChamber (Vector<Set<cell>> & chambers, Vector<wall> & walls, int n, int & wallOne, int & wallTwo) {
    for (int i = 0; i < chambers.size() - 1; i++) {
        if(chambers[i].contains(walls[n].one)) {
            wallOne = i;
        }
        if(chambers[i].contains(walls[n].two)) {
            wallTwo = i;
        }
    }
    if (wallOne == wallTwo) return true;
    return false;
}

/*
 * Iterate through all the walls and remove all of those who separate two chambers
 */

static void completeMaze (Vector<Set<cell>> & chambers, Vector<wall> & walls, MazeGeneratorView & maze) {
    int a = 0;
    while (chambers.size() > 1) {
        int n = randomInteger(0, walls.size() - 1);
        int wallOne;
        int wallTwo;
        if (!checkChamber(chambers, walls, n, wallOne, wallTwo)) { //in different chamber
            a++;
            maze.removeWall(walls[n]);
            walls.remove(n);
            chambers[wallOne] += chambers[wallTwo];
            chambers.remove(wallTwo);
        }
        cout << a << "     " << chambers.size() << endl;
    }
}

int main() {
	while (true) {
		int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
		if (dimension == 0) break;
        MazeGeneratorView maze;
        maze.setDimension(dimension);
        Vector<Set<cell>> chambers;
        Vector<wall> walls;
        generateChambers(dimension, chambers, walls, maze);
        completeMaze(chambers, walls, maze);
	}
	return 0;
}
