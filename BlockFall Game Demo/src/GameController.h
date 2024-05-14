#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    ~GameController();

    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay


    bool checkFree(const vector<std::vector<int>> &table, const Block &block);



    bool patternExists(const vector<std::vector<char>> &bigVector, const vector<std::vector<char>> &pattern);

    bool patternExists(const vector<std::vector<int>> &bigVector, const vector<std::vector<bool>> &pattern);

private:
    BlockFall game;

};


#endif //PA2_GAMECONTROLLER_H
