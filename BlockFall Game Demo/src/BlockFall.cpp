#include <fstream>
#include <sstream>
#include <iostream>
#include "BlockFall.h"
#include <algorithm>

BlockFall::BlockFall() : gravity_mode_on(false), leaderboard_file_name(""), player_name("") {

    initial_block = nullptr;
    active_rotation = nullptr;
    //gamecontrollerda destructor yapmak için boşuna oluşturdum

}


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {

    std::vector<std::vector<std::vector<bool>>> matrices;
    std::ifstream file(input_file);

    std::string line;
    //creates matrix vector
    while (std::getline(file, line)) {
        if (line.find('[') != std::string::npos) {
            std::vector<std::vector<bool>> matrix;

            do {
                std::vector<bool> row;
                for (char c : line) {
                    if (c == '0') {
                        row.push_back(false);
                    } else if (c == '1') {
                        row.push_back(true);
                    }
                }
                if (!row.empty()) {
                    matrix.push_back(row);
                }

                // satırın içinde ] karakteri bulunup bulunmadığını kontrol et
            } while (line.find(']') == std::string::npos && std::getline(file, line));


            if (!matrix.empty()) {
                matrices.push_back(matrix);
            }
        }
    }
    Block *previous = nullptr;  // önceki bloğun işaretçisi
    Block *current = nullptr;   // şu anki bloğun işaretçisi
    for (int i =0;i<matrices.size();i++){
        if(i==matrices.size()-1){
            power_up=matrices[i];
        }else{
            std::vector<std::vector<bool>> firstRotate;
            std::vector<std::vector<bool>> secondRotate;
            std::vector<std::vector<bool>> thirdRotate;
            std::vector<std::vector<bool>> original;
            rotate90(matrices[i]);
            firstRotate=matrices[i];
            Block * first= new Block;
            first->shape=firstRotate;


            rotate90(matrices[i]);
            secondRotate=matrices[i];
            Block * second =new Block;
            second->shape=secondRotate;

            rotate90(matrices[i]);
            thirdRotate=matrices[i];
            Block *  third =new Block ;
            third->shape=thirdRotate;


            rotate90(matrices[i]);
            original=matrices[i];
            Block * last =new Block;
            last->shape=original;
            if(*last != *first & *last == *second){
                first->right_rotation=last;
                first->left_rotation=last;
                last->right_rotation=first;
                last->left_rotation=first;
                if (previous == nullptr) {
                    initial_block = last;
                    active_rotation = initial_block;
                    current = initial_block;
                } else {
                    current->next_block = last;
                    current = last;
                }

                previous = last;

            }

            else if((*first!= *second) && (*second != *third) &&  (*third != *last ) && (*last != *first )){
                first->right_rotation=second;
                first->left_rotation=last;
                second->right_rotation=third;
                second->left_rotation=first;
                third->right_rotation=last;
                third->left_rotation=second;
                last->right_rotation=first;
                last->left_rotation=third;
                if (previous == nullptr) {
                    initial_block = last;
                    active_rotation = initial_block;
                    current = initial_block;
                } else {
                    current->next_block = last;
                    current = last;
                }

                previous = last;


            }
            else if((*first == *second)){
                last->right_rotation=last;
                last->left_rotation=last;
                if (previous == nullptr) {
                    initial_block = last;
                    active_rotation = initial_block;
                    current = initial_block;
                } else {
                    current->next_block = last;
                    current = last;
                }

                previous = last;

            }


        }}
}



void BlockFall::initialize_grid(const string &input_file) {
    std::ifstream file(input_file);

    if (!file.is_open()) {
        perror("Error opening file");
        std::cerr << "Error opening file: " << input_file << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int pixel;
        while (iss >> pixel) {
            row.push_back(pixel);
        }
        grid.push_back(row);
    }

    file.close();
    rows = static_cast<int>(grid.size());
    if (rows == 0) {
        std::cerr << "Error: Empty image data." << std::endl;
        return;
    }

    cols = static_cast<int>(grid[0].size());
    int** newGrid;


    
    newGrid = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        newGrid[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            newGrid[i][j] = grid[i][j];
        }
    }


}
void BlockFall::rotate90(std::vector<std::vector<bool>>& matrix) {
    int rows= matrix.size();
    int cols=matrix[0].size();
    // Transpoze al
    std::vector<std::vector<bool>> transposedMatrix(cols, std::vector<bool>(rows, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }
    // Her bir satırı ters çevir
    for (int i = 0; i < cols; ++i) {
        std::reverse(transposedMatrix[i].begin(), transposedMatrix[i].end());
    }

    // Döndürülmüş matrisi orijinal matrise kopyala
    matrix.swap(transposedMatrix);


}



BlockFall::~BlockFall() {
    Block *current = initial_block;
    while (current != nullptr) {
        Block *next = current->next_block;
        delete current; // Belleği temizle
        current = next;
    }
    // TODO: Free dynamically allocated memory used for storing game blocks
}
