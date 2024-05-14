#include <iostream>
#include <fstream>
#include "Leaderboard.h"
#include <iomanip>
#include <ctime>


void Leaderboard::insert_new_entry(LeaderboardEntry* new_entry) {
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* current = head_leaderboard_entry;
        LeaderboardEntry* previous = nullptr;

        while (current != nullptr && new_entry->score <= current->score) {
            previous = current;
            current = current->next_leaderboard_entry;
        }

        previous->next_leaderboard_entry = new_entry;
        new_entry->next_leaderboard_entry = current;
    }
}
void Leaderboard::write_to_file(const string& filename) const {
    ofstream file(filename);

    LeaderboardEntry* current = head_leaderboard_entry;

    while (current != nullptr) {
        file << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next_leaderboard_entry;
    }
    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {//yoksa oluşturur
        ofstream file(filename);
    }

    while (head_leaderboard_entry != nullptr) {
        LeaderboardEntry *next = head_leaderboard_entry->next_leaderboard_entry;
        delete head_leaderboard_entry;
        head_leaderboard_entry = next;
    }
    LeaderboardEntry *last = nullptr;
    while (file.peek() != EOF) {
        unsigned long score;
        time_t timestamp;
        string player_name;

        file >> score >> timestamp >> player_name;

        LeaderboardEntry *new_entry = new LeaderboardEntry(score, timestamp, player_name);

        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_entry;
        } else {
            last->next_leaderboard_entry = new_entry;
        }

        last = new_entry;
    }

    file.close();
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
}


void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard" << endl;
    std::cout << "-----------" << endl;
    LeaderboardEntry *current = head_leaderboard_entry;
    int order = 1;
    while (current != nullptr) {
        std::time_t entryTime = std::time(nullptr);
        std::tm *localTime = std::localtime(&current->last_played);

        char timeStr[20]; // Adjust the size as needed
        std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S/%d.%m.%Y", localTime);

        std::cout << order << ". " << current->player_name << " " << current->score << " " << timeStr << std::endl;
        current = current->next_leaderboard_entry;
        order++;
    }   order++;

    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry *next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
}
