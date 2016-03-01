#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>

using namespace std;

class gameMap
{
public:
    vector<string> stars, vecSpawn;
    int x, y, numStars, numSpawnpoints;
    void loadMap(fstream* mappath, string path)
    {
        string strstars, spawnpoints, xdem, ydem;
        int i;

        getline(*mappath, xdem);
        getline(*mappath, ydem);
        getline(*mappath, strstars);
        x = atoi(xdem.c_str());
        y = atoi(ydem.c_str());
        numStars = atoi(strstars.c_str());
        cout << path << " is " <<  x << " wide and " << y << " tall.\n";
        cout << "There are "<< numStars << " stars. Positions are as follows:\n";
        for(i = 0; i < numStars; i++)
        {
            getline(*mappath, strstars);
            stars.push_back(strstars);
            cout << strstars << endl;
        }
        getline(*mappath, spawnpoints);
        numSpawnpoints = atoi(spawnpoints.c_str());
        cout << "There are " << spawnpoints << " spawnpoints. Positions are as follows:\n";
        for(i = 0; i < numSpawnpoints; i++)
        {
            getline(*mappath, spawnpoints);
            vecSpawn.push_back(spawnpoints);
            cout << spawnpoints << endl;
        }

    }

    void createMap(fstream* mappath, string path)
    {

    }
};

int printDirContents()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] != '.')
                printf ("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
}

void commandLoop(fstream* mappath, string path, gameMap* theMap)
{

}

int main()
{
    string path;
    gameMap theMap;

    char* cwd = get_current_dir_name();
    free(cwd);
    cout << "Welcome to Mapper" << endl;
    cout << "Current working directory is: " << cwd << endl;
    cout << "The files in CWD are: " << endl;
    printDirContents();
    cout << "Enter your map filename to open or create, including path if necessary:\n: ";
    cout.flush();
    getline(cin, path);

    ifstream f(path.c_str());
    fstream mappath;
    if (f.good()) {
        f.close();
        mappath.open(path.c_str());
        theMap.loadMap(&mappath, path);

    } else {
        f.close();
        mappath.open(path.c_str());
        theMap.createMap(&mappath, path);
    }

    commandLoop(&mappath, path, &theMap);
    return 0;
}

