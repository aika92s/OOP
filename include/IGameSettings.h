#ifndef MINESWEEPERLAB_IGAMESETTINGS_H
#define MINESWEEPERLAB_IGAMESETTINGS_H

class IGameSettings {
public:
    virtual ~IGameSettings() = default;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getBombsCount() const = 0;
};

#endif //MINESWEEPERLAB_IGAMESETTINGS_H