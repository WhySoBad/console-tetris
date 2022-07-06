#include "ConfigHelper.h"
#include "fstream"
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
    highScore = static_cast<int>(highscore);
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
    std::unordered_map<std::string, int> config;
    while(getline(input, line)) {
        std::string key, value;
        key = line.substr(0, line.find('='));
        value = line.substr(line.find('=') + 1, line.length() - 1);
        try {
            config.insert_or_assign(key, static_cast<int>(std::stoi(value)));
        } catch(...) {
            config.insert_or_assign(key, 0);
        }
    }
    input.close();

    auto getItem = [&config](std::string key) -> int {
        try {
            return config.at(key);
        } catch(...) {
            return 0;
        }
    };

    highScore = getItem("highscore");
    rounds = getItem("rounds");
    destroyedLines = getItem("destroyedlines");
    highestLevel = getItem("highestlevel");

    ConfigHelper::writeConfig(); // make sure that the config is right formatted
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