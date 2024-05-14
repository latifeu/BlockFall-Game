#include <fstream>
#include <iostream>
#include <sstream>
#include "GameController.h"
#include <algorithm>
#include <memory>
#define occupiedCellChar "⬜"
#define unoccupiedCellChar "⬛"
bool GameController::play(BlockFall& game, const string& commands_file){

    std::ifstream file(commands_file);

    std::string line;
    std::vector<std::string> commandVector;

    //creates matrix vector
    while (std::getline(file, line)){
        std::istringstream iss(line);
        std::string command;
        while (iss >> command) {
            commandVector.push_back(command);
        }
    }
    int moveR=0;
    int moveL=0;
    int rotaR=0;
    int rotaL=0;
    for (int i=0;i<commandVector.size();i++){
        if(commandVector[i]=="PRINT_GRID" & (i!=commandVector.size()-1)){
            if((moveR!=0 || moveL!=0|| rotaL!=0|| rotaR!= 0) & (i!=0) ){
                std::cout<<"Score: "<<game.current_score<<std::endl;
                int z=0;
                if (game.leaderboard.head_leaderboard_entry) {
                    z= game.leaderboard.head_leaderboard_entry->score;
                } else {
                    z=0;
                }
                std::cout<<"High Score: "<<z<<std::endl;
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (game.grid[i][j]==1){
                            std::cout << occupiedCellChar;
                        }
                        else if (game.grid[i][j]==0){
                            std::cout << unoccupiedCellChar;
                        }
                    }
                    std::cout<<'\n';
                }
            }
            if(checkFree(game.grid,*game.active_rotation)){
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==true){
                            game.grid[h][k]=1;
                        }
                    }
                }
                int z=0;
                if (game.leaderboard.head_leaderboard_entry) {
                    z= game.leaderboard.head_leaderboard_entry->score;
                } else {
                    z=0;
                }
                std::cout<<"Score: "<<game.current_score<<std::endl;
                std::cout<<"High Score: "<<z<<std::endl;
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (game.grid[i][j]==1){
                            std::cout << occupiedCellChar;
                        }
                        else if (game.grid[i][j]==0){
                            std::cout << unoccupiedCellChar;
                        }
                    }
                    std::cout<<'\n';
                }
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==1){
                            game.grid[h][k]=0;
                        }

                    }
                }
                std::cout<<'\n';
                std::cout<<'\n';

            }
        }

        else if (commandVector[i] =="ROTATE_RIGHT") {
            rotaR+=1;
            int oldX = game.active_rotation->locationX;
            int oldY = game.active_rotation->locationY;

            // Clear the old position of the block in the grid
            for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                    game.grid[h + oldX][k + oldY] = 0;
                }
            }

            // Create a temporary block for the rotation

            std::unique_ptr<Block> temp_block(new Block(*game.active_rotation->right_rotation));
            temp_block->locationX=game.active_rotation->locationX;
            temp_block->locationY=game.active_rotation->locationY;

            if(temp_block->shape==game.active_rotation->shape){
                std::vector<std::vector<bool>> transposedMatrix(temp_block->shape[0].size(), std::vector<bool>(temp_block->shape.size(), 0));
                for (int i = 0; i < temp_block->shape.size(); ++i) {
                    for (int j = 0; j < temp_block->shape[0].size(); ++j) {
                        transposedMatrix[j][i] = temp_block->shape[i][j];
                    }
                }
                // Her bir satırı ters çevir
                for (int i = 0; i < temp_block->shape[0].size(); ++i) {
                    std::reverse(transposedMatrix[i].begin(), transposedMatrix[i].end());
                }
                temp_block->shape=transposedMatrix;
            }


            // Check if the rotation is valid
            if (checkFree(game.grid, *temp_block)) {

                // Rotate the block to the right
                game.active_rotation->shape = temp_block->shape;
                game.active_rotation->right_rotation = temp_block->right_rotation;
                game.active_rotation->left_rotation = temp_block->left_rotation;

                if(game.active_rotation->locationY+game.active_rotation->shape[0].size()<=game.grid[0].size()){
                    for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                        for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                            if (game.active_rotation->shape[h][k]) {
                                game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                            }
                        }
                    }

                }
                else{
                    for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                        for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                            if (game.active_rotation->shape[h][k]) {
                                game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                            }
                        }
                    }

                }

            } else {

                // Restore the old position if the new position is not free
                for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                    for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                        if (game.active_rotation->shape[h][k]) {
                            game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                        }
                    }
                }
            }

        }
        else if (commandVector[i] =="ROTATE_LEFT") {
            rotaL+=1;
            int oldX = game.active_rotation->locationX;
            int oldY = game.active_rotation->locationY;

            // Clear the old position of the block in the grid
            for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                    game.grid[h + oldX][k + oldY] = 0;
                }
            }

            // Create a temporary block for the rotation
            std::unique_ptr<Block> temp_block(new Block(*game.active_rotation->left_rotation));
            temp_block->locationX=game.active_rotation->locationX;
            temp_block->locationY=game.active_rotation->locationY;

            if(temp_block->shape==game.active_rotation->shape){
                std::vector<std::vector<bool>> transposedMatrix(temp_block->shape[0].size(), std::vector<bool>(temp_block->shape.size(), 0));
                for (int i = 0; i < temp_block->shape.size(); ++i) {
                    for (int j = 0; j < temp_block->shape[0].size(); ++j) {
                        transposedMatrix[j][i] = temp_block->shape[i][j];
                    }
                }
                // Her bir satırı ters çevir
                for (int i = 0; i < temp_block->shape[0].size(); ++i) {
                    std::reverse(transposedMatrix[i].begin(), transposedMatrix[i].end());
                }
                std::vector<std::vector<bool>> transposedMatrix2(transposedMatrix[0].size(), std::vector<bool>(transposedMatrix.size(), 0));
                for (int i = 0; i < transposedMatrix.size(); ++i) {
                    for (int j = 0; j < transposedMatrix[0].size(); ++j) {
                        transposedMatrix2[j][i] = transposedMatrix[i][j];
                    }
                }
                // Her bir satırı ters çevir
                for (int i = 0; i < transposedMatrix[0].size(); ++i) {
                    std::reverse(transposedMatrix2[i].begin(), transposedMatrix2[i].end());
                }
                std::vector<std::vector<bool>> transposedMatrix3(transposedMatrix2[0].size(), std::vector<bool>(transposedMatrix2.size(), 0));
                for (int i = 0; i < transposedMatrix2.size(); ++i) {
                    for (int j = 0; j < transposedMatrix2[0].size(); ++j) {
                        transposedMatrix3[j][i] = transposedMatrix2[i][j];
                    }
                }
                // Her bir satırı ters çevir
                for (int i = 0; i < transposedMatrix2[0].size(); ++i) {
                    std::reverse(transposedMatrix3[i].begin(), transposedMatrix3[i].end());
                }
                temp_block->shape=transposedMatrix3;


            }


            // Check if the rotation is valid
            if (checkFree(game.grid, *temp_block)) {
                // Rotate the block to the left
                game.active_rotation->shape = temp_block->shape;
                game.active_rotation->right_rotation = temp_block->right_rotation;
                game.active_rotation->left_rotation = temp_block->left_rotation;
                if(game.active_rotation->locationY+game.active_rotation->shape[0].size()<=game.grid[0].size()& game.active_rotation->locationY>=0){
                    // Update the grid with the new position of the rotated block
                    for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                        for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                            if (game.active_rotation->shape[h][k]) {
                                game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                            }
                        }
                    }
                }
                else {
                    // Restore the old position if the new position is not free
                    for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                        for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                            if (game.active_rotation->shape[h][k]) {
                                game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                            }
                        }
                    }
                }
            } else {
                // Restore the old position if the new position is not free
                for (int h = 0; h < game.active_rotation->shape.size(); h++) {
                    for (int k = 0; k < game.active_rotation->shape[0].size(); k++) {
                        if (game.active_rotation->shape[h][k]) {
                            game.grid[game.active_rotation->locationX + h][game.active_rotation->locationY + k] = 1;
                        }
                    }
                }
            }

        }
        else if(commandVector[i]=="MOVE_LEFT"){
            moveL+=1;
            int oldX=game.active_rotation->locationX;
            int oldY=game.active_rotation->locationY;
            //eski yerini temizle
            for(int h=0;h<game.active_rotation->shape.size();h++){
                for(int k=0;k<game.active_rotation->shape[0].size();k++){
                    game.grid[h+oldX][k+oldY]=0;
                }
            }
            //sola kaydır
            game.active_rotation->locationY=game.active_rotation->locationY-1;
            if(checkFree(game.grid,*game.active_rotation)){
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==true){
                            game.grid[game.active_rotation->locationX+h][game.active_rotation->locationY+k]=1;
                        }

                    }
                }
            }else{
                game.active_rotation->locationY=game.active_rotation->locationY+1;
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==true){
                            game.grid[game.active_rotation->locationX+h][game.active_rotation->locationX+k]=1;
                        }

                    }
                }
            }
        }
        else if(commandVector[i]=="MOVE_RIGHT") {
            moveR+=1;
            int oldX=game.active_rotation->locationX;
            int oldY=game.active_rotation->locationY;
            //eski yerini temizle
            for(int h=0;h<game.active_rotation->shape.size();h++){
                for(int k=0;k<game.active_rotation->shape[0].size();k++){
                    game.grid[h+oldX][k+oldY]=0;
                }
            }
            //sağa kaydır
            game.active_rotation->locationY=game.active_rotation->locationY+1;
            if(checkFree(game.grid,*game.active_rotation)){
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==true){
                            game.grid[game.active_rotation->locationX+h][game.active_rotation->locationY+k]=1;
                        }

                    }
                }
            }else{
                game.active_rotation->locationY=game.active_rotation->locationY-1;
                for(int h=0;h<game.active_rotation->shape.size();h++){
                    for(int k=0;k<game.active_rotation->shape[0].size();k++){
                        if(game.active_rotation->shape[h][k]==true){
                            game.grid[game.active_rotation->locationX+h][game.active_rotation->locationY+k]=1;
                        }

                    }
                }
            }

        }
        else if(commandVector[i]=="DROP"){
            rotaL=0;
            rotaR=0;
            moveL=0;
            moveR=0;
            int oldX=game.active_rotation->locationX;
            int oldY=game.active_rotation->locationY;

            std::vector<int> bottomOnes;
            for (int col = 0; col < game.active_rotation->shape[0].size(); ++col) {
                // Başlangıçta en alt satırdan başla ve yukarı doğru tara
                for (int row = game.active_rotation->shape.size() - 1; row >= 0; --row) {
                    if (game.active_rotation->shape[row][col] == 1) {
                        bottomOnes.push_back(row);
                        break;
                    }
                }
            }

            int minFirst = *std::max_element(bottomOnes.begin(), bottomOnes.end());
            int y = game.active_rotation->locationX;
            int count = 0;
            while (y < game.grid.size()) {
                count = 0; // Her döngüde count'u sıfırla

                for (int i = 0; i < game.active_rotation->shape[0].size(); i++) {
                    if ( bottomOnes[i] + 1 < game.grid.size() &&
                         game.grid[ bottomOnes[i] +1][i + game.active_rotation->locationY] == 0) {
                        count += 1;
                    }
                }

                if (count == game.active_rotation->shape[0].size()){
                    y += 1;

                    // bottomOnes'i güncelle
                    for (int k = 0; k < bottomOnes.size(); k++) {
                        bottomOnes[k] = bottomOnes[k] + 1;
                    }
                } else {
                    break;
                }
            }
            for(int h=0;h<game.active_rotation->shape.size();h++){
                for(int k=0;k<game.active_rotation->shape[0].size();k++){
                    if( game.active_rotation->shape[h][k]==1){
                        game.grid[h+oldX][k+oldY]=0;
                    }
                }
            }
            int minLast = *std::max_element(bottomOnes.begin(), bottomOnes.end());
            int filled=0;
            for(int h=0;h<game.active_rotation->shape.size();h++){
                for(int k=0;k<game.active_rotation->shape[0].size();k++){
                    if( game.active_rotation->shape[h][k]==1){
                        filled+=1;
                    }
                }
            }
            game.current_score+=filled*(minLast-minFirst);
            game.active_rotation->locationX =game.active_rotation->locationX+ (minLast-minFirst );
            for(int h=0;h<game.active_rotation->shape.size();h++){
                for(int k=0;k<game.active_rotation->shape[0].size();k++){
                    if(game.active_rotation->shape[h][k]==true){
                        game.grid[game.active_rotation->locationX+h][game.active_rotation->locationY+k]=1;
                    }
                }
            }

            if (game.gravity_mode_on) {
                for (int i = game.grid.size() - 2; i >= 0; i--) {
                    for (int k = 0; k < game.grid[0].size(); k++) {
                        if (game.grid[i][k] == 1) {
                            int j = i;
                            while (j + 1 < game.grid.size() && game.grid[j + 1][k] == 0) {
                                // Bloğu bir birim aşağı kaydır.
                                game.grid[j + 1][k] = game.grid[j][k];
                                game.grid[j][k] = 0;
                                j++;
                            }
                        }
                    }
                }
            }
            int anyDelete=0;
            bool check=true;
            std::vector<int> deleteRow;
            for(int j=0;j<game.rows;j++){
                int newCount=0;
                for(int i=0;i<game.cols;i++){
                    if(game.grid[j][i]==1){
                        newCount+=1;
                    }
                }
                if(newCount==game.cols){
                    anyDelete+=1;
                    deleteRow.push_back(j);
                }
            }
            if(anyDelete!=0){
                std::cout<<"Before clearing:"<<std::endl;
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (game.grid[i][j]==1){
                            std::cout << occupiedCellChar;
                        }
                        else if (game.grid[i][j]==0){
                            std::cout << unoccupiedCellChar;
                        }
                    }
                    std::cout<<'\n';
                }
                std::cout<<'\n';
                std::cout<<'\n';
                for (int k=0;k<deleteRow.size();k++){
                    if (deleteRow[k] >= 0 &&  deleteRow[k]< game.grid.size()) {
                        game.grid.erase(game.grid.begin() +deleteRow[k]);
                        game.grid.insert(game.grid.begin(), std::vector<int>(game.grid[0].size(), 0));
                    }
                }
                game.current_score+=game.cols*deleteRow.size();

            }
            std::vector<std::vector<std::vector<bool>>> submatrices;
            for (int i = 0; i <= game.grid.size() - game.power_up.size(); ++i) {
                for (int j = 0; j <= game.grid[0].size() - game.power_up[0].size(); ++j) {
                    // Alt matrisi vektöre ekle
                    std::vector<std::vector<bool>> submatrix;
                    for (int x = i; x < i + game.power_up.size(); ++x) {
                        std::vector<bool> row;
                        for (int y = j; y < j + game.power_up.size(); ++y) {
                            if(game.grid[x][y]==1){
                                row.push_back(true);
                            }
                            else if(game.grid[x][y]==0){
                                row.push_back(false);
                            }
                        }
                        submatrix.push_back(row);
                    }
                    submatrices.push_back(submatrix);
                }
            }
            bool cntrl= false;
            for (int i=0;i<submatrices.size();i++){
                if(submatrices[i]==game.power_up){
                    cntrl=true;
                    break;
                }
            }
            if(cntrl){
                game.current_score+=1000;
                std::cout<<"Before clearing:"<<std::endl;
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (game.grid[i][j]==1){
                            std::cout << occupiedCellChar;
                        }
                        else if (game.grid[i][j]==0){
                            std::cout << unoccupiedCellChar;
                        }
                    }
                    std::cout<<'\n';
                }
                std::cout<<'\n';
                std::cout<<'\n';
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        game.grid[i][j]=0;
                    }
                }


            }

            if (game.active_rotation->next_block != nullptr) {
                game.active_rotation = game.active_rotation->next_block;
                if(!checkFree(game.grid,*game.active_rotation)){
                    std::cout<<"GAME OVER!\n""Next block that couldn't fit:\n";
                    for (int i=0;i<game.active_rotation->shape.size();i++){
                        for (int j=0;j<game.active_rotation->shape[0].size();j++){
                            if (game.active_rotation->shape[i][j]==1){
                                std::cout << occupiedCellChar;
                            }
                            else if (game.active_rotation->shape[i][j]==0){
                                std::cout << unoccupiedCellChar;
                            }
                        }
                        std::cout<<'\n';

                    }
                    std::cout<<'\n';
                    int z=0;
                    if (game.leaderboard.head_leaderboard_entry) {
                        z= game.leaderboard.head_leaderboard_entry->score;
                    } else {
                        z=0;
                    }
                    std::cout<<"Final grid and score:\n";
                    std::cout<<"Score: "<<game.current_score<<std::endl;
                    std::cout<<"High Score: "<<z<<std::endl;
                    for(int i=0;i<game.rows;i++){
                        for(int j=0;j<game.cols;j++){
                            if (game.grid[i][j]==1){
                                std::cout << occupiedCellChar;
                            }
                            else if (game.grid[i][j]==0){
                                std::cout << unoccupiedCellChar;
                            }
                        }
                        std::cout<<'\n';
                    }
                    std::cout<<'\n';
                    std::cout<<'\n';
                    game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
                    game.leaderboard.print_leaderboard();
                    game.leaderboard.write_to_file("final_leaderboard_after_all_plays.txt");
                    return false;
                    break;

                }
            }else{
                std::cout<<"YOU WIN!\n"
                           "No more blocks.\n";
                int z=0;
                if (game.leaderboard.head_leaderboard_entry) {
                    z= game.leaderboard.head_leaderboard_entry->score;
                } else {
                    z=0;
                }

                std::cout<<"Final grid and score:"<<std::endl;
                std::cout<<" "<<std::endl;
                std::cout<<"Score: "<<game.current_score<<std::endl;
                std::cout<<"High Score: "<<z<<std::endl;
                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (game.grid[i][j]==1){
                            std::cout << occupiedCellChar;
                        }
                        else if (game.grid[i][j]==0){
                            std::cout << unoccupiedCellChar;
                        }
                    }
                    std::cout<<'\n';
                }
                std::cout<<'\n';
                std::cout<<'\n';
                game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
                game.leaderboard.print_leaderboard();
                game.leaderboard.write_to_file("final_leaderboard_after_all_plays.txt");
                return true;
                break;
            }
        }
        else if(commandVector[i] == "GRAVITY_SWITCH") {
            game.gravity_mode_on = !game.gravity_mode_on;
            if (game.gravity_mode_on) {
                for(int i=0;i<game.active_rotation->shape.size();i++){
                    for (int j=0;j<game.active_rotation->shape[0].size();j++){
                        game.grid[game.active_rotation->locationX+i][game.active_rotation->locationY+j]=0;
                    }
                }
                for (int i = game.grid.size() - 2; i >= 0; i--) {
                    for (int k = 0; k < game.grid[0].size(); k++) {
                        if (game.grid[i][k] == 1) {
                            int j = i;
                            while (j + 1 < game.grid.size() && game.grid[j + 1][k] == 0) {
                                // Bloğu bir birim aşağı kaydır.
                                game.grid[j + 1][k] = game.grid[j][k];
                                game.grid[j][k] = 0;
                                j++;
                            }
                        }
                    }
                }
                bool check=true;
                while(check){
                    int newCount=0;
                    for(int i=0;i<game.cols;i++){
                        if(game.grid[game.grid.size()-1][i]==1){
                            newCount+=1;
                        }
                    }
                    if(newCount==game.cols){

                        game.current_score+=game.cols;
                        for(int i=game.grid.size()-1;i>0;i--){
                            game.grid[i]=game.grid[i-1];

                        }
                        for (int j = 0; j < game.grid[0].size(); ++j) {
                            game.grid[0][j] = 0;
                        }
                    }else{
                        check= false;
                    }

                }
            }

        }
        else if(i==commandVector.size()-1){
            int z=0;
            if (game.leaderboard.head_leaderboard_entry) {
                z= game.leaderboard.head_leaderboard_entry->score;
            } else {
                z=0;
            }
            std::cout<<"Score: "<<game.current_score<<std::endl;
            std::cout<<"High Score: "<<z<<std::endl;
            for(int i=0;i<game.rows;i++){
                for(int j=0;j<game.cols;j++){
                    if (game.grid[i][j]==1){
                        std::cout << occupiedCellChar;
                    }
                    else if (game.grid[i][j]==0){
                        std::cout << unoccupiedCellChar;
                    }
                }
                std::cout<<'\n';
            }
            std::cout<<'\n';
            std::cout<<'\n';
            std:cout<<"GAME FINISHED!\n""No more commands.\n";
            if (game.leaderboard.head_leaderboard_entry) {
                z= game.leaderboard.head_leaderboard_entry->score;
            } else {
                z=0;
            }
            std::cout<<"Final grid and score:"<<std::endl;
            std::cout<<" "<<std::endl;
            std::cout<<"Score: "<<game.current_score<<std::endl;
            std::cout<<"High Score: "<<z<<std::endl;
            for(int i=0;i<game.rows;i++){
                for(int j=0;j<game.cols;j++){
                    if (game.grid[i][j]==1){
                        std::cout << occupiedCellChar;
                    }
                    else if (game.grid[i][j]==0){
                        std::cout << unoccupiedCellChar;
                    }
                }
                std::cout<<'\n';
            }
            std::cout<<'\n';
            std::cout<<'\n';
            game.leaderboard.insert_new_entry(new LeaderboardEntry(game.current_score, time(nullptr), game.player_name));
            game.leaderboard.print_leaderboard();
            game.leaderboard.write_to_file("final_leaderboard_after_all_plays.txt");
            return true;
            break;
        }
        else{
            std::cout<<"Unknown command: "<<commandVector[i]<<std::endl;
        }




    }
    return false;
}


bool GameController::checkFree(const std::vector<std::vector<int>>& table, const Block& block) {
    int counterOne = 0;
    int counterTwo = 0;
    if(block.locationY+block.shape[0].size()<=table[0].size()){
        for (int i = 0; i < block.shape.size(); ++i) {
            for (int j = 0; j < block.shape[0].size(); ++j) {
                if (block.shape[i][j]) {
                    counterOne += 1;
                    if (block.locationX + i >= 0 && block.locationX + i < table.size() &&
                        block.locationY + j >= 0 && block.locationY + j < table[0].size()) {
                        if (table[block.locationX + i][block.locationY + j] == 0) {
                            counterTwo += 1;
                        }
                    }


                    // Kontrol et: bloğun grid
                }
            }
        }
        return (counterOne == counterTwo);
    }else{
        return false;
    }


}

GameController::~GameController() {
    // Check if active_rotation is not nullptr before deleting
    if (game.active_rotation != nullptr) {
        delete game.active_rotation;
        game.active_rotation = nullptr;  // Set to nullptr after deletion to avoid dangling pointer
    }
}













