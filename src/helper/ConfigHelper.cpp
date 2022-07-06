#include "ConfigHelper.h"
#include "fstream"
#include "iostream"
#include "filesystem"
#include "istream"
#include "unordered_map"

int ConfigHelper::getHighscore() {
    return highScore;
}

int ConfigHelper::getRounds() {
    return rounds;
}

int ConfigHelper::getDestroyedLines() {
    return destroyedLines;
}

int ConfigHelper::getHighestLevel() {
    return highestLevel;
}

void ConfigHelper::setHighscore(unsigned int highscore) {
    highScore += static_cast<int>(highscore);
    ConfigHelper::writeConfig();
}

void ConfigHelper::addRound() {
    rounds++;
    ConfigHelper::writeConfig();
}

void ConfigHelper::addDestroyedLines(unsigned int lines) {
    destroyedLines += static_cast<int>(lines);
}

void ConfigHelper::setHighestLevel(int level) {
    highestLevel = level;
    ConfigHelper::writeConfig();
}

void ConfigHelper::initialize() {
    ConfigHelper::readConfig();
}


void ConfigHelper::readConfig() {
    std::string path = std::string(getenv("HOME")).append("/.config/console-tetris");
    if(!std::filesystem::exists(path)) ConfigHelper::writeConfig();
    std::ifstream input(path.append("/console-tetris.conf"));
    std::string line;
    std::unordered_map<std::string, std::string> config;
    while(getline(input, line)) {
        std::string key;
        if(getline(std::istringstream(line), key)) {
            std::string value;
            if(getline(std::istringstream(), value)) config.insert_or_assign(key, value);
        }
    }
    input.close();

    auto readItem = [&config](std::string key, int &replace) {
        try {
            std::string value = config.at(key);
            int number = replace;
            try {
                number = std::stoi(value);
            } catch(std::invalid_argument &exception) {}
        } catch(const std::out_of_range &exception) {}
    };
    readItem("highscore", highScore);
    readItem("rounds", rounds);
    readItem("destroyedlines", destroyedLines);
    readItem("highestlevel", highestLevel);
}

void ConfigHelper::writeConfig() {
    std::string path = std::string(getenv("HOME")).append("/.config/console-tetris");
    if(!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    std::ofstream output(path.append("/console-tetris.conf"));
    output << "highscore=" << highScore << "\n";
    output << "rounds=" << rounds << "\n";
    output << "destroyedlines=" << destroyedLines << "\n";
    output << "highestlevel=" << highestLevel << "\n";
    output.close();
}