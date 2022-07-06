#ifndef CONSOLE_TETRIS_CONFIGHELPER_H
#define CONSOLE_TETRIS_CONFIGHELPER_H

#define CONFIG_PATH   ".config/console-tetris/console-tetris.conf"

class ConfigHelper {
public:
    /**
     * Get the highscore of the player
     * @return highscore
     */

    [[nodiscard]] static int getHighscore();

    /**
     * Get the amount of rounds played
     * @return amount of rounds played
     */

    [[nodiscard]] static int getRounds();

    /**
     * Get the over all destroyed lines
     * @return the over all destroyed lines
     */

    [[nodiscard]] static int getDestroyedLines();

    /**
     * Get the highest reached level
     * @return highest reached level
     */

    [[nodiscard]] static int getHighestLevel();

    /**
     * Set the highscore
     * @param highscore new highscore
     */

    static void setHighscore(unsigned int highscore);

    /**
     * Add a round
     */

    static void addRound();

    /**
     * Add an amount of destroyed lines
     * @param lines amount of destroyed lines to add
     */

    static void addDestroyedLines(unsigned int lines);

    /**
     * Set a new highest reached level
     * @param level new highest reached level
     */

    static void setHighestLevel(int level);

    /**
     * Initialize the config helper
     */

    static void initialize();

private:
    static inline int highScore = 0;
    static inline int rounds = 0;
    static inline int destroyedLines = 0;
    static inline int highestLevel = 0;

    /**
     * Read the config file and update the values
     */

    static void readConfig();

    /**
     * Write the updated values to the config file
     */

    static void writeConfig();
};


#endif //CONSOLE_TETRIS_CONFIGHELPER_H
