/* Tetris.h */
#pragma once
#include "Blocks.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <exception>
#include <ctime>
class LongRetangle; class Square; class LShape; class TShape;
class ZShape; class ReverseLShape; class ReverseZShape;

class Tetris {
private:
    std::vector<std::vector<color_t>> board;
    Blocks* currentBlock;   // 当前方块
    Blocks* nextBlock;      // 下一个方块

    int score;
    int highScore;
    int level;
    int linesClearedTotal;

    bool gameOver;

    long long lastDropTime;
    long dropDelay;

    void initBoard();
    void spawnNewBlock();
    void selectNextBlock();

    bool checkCollision(const Blocks* block, int testX, int testY, int testRotation) const;
    void mergeBlockToBoard();
    int clearFullLines();

    void updateGameLogic();
    void renderGame() const;
    void drawBoard() const;     // 绘制游戏区域
    void drawUI() const;        // 绘制UI
    void drawGameOverScreen() const;    // 绘制游戏结束画面

    void handlePlayerInput();
    void moveBlock(int dx, int dy);
    void rotateCurrentBlock();

    // 文件处理
    void loadSettings();
    void loadHighScore();
    void saveHighScore();

    void setScore(int newScore);    // 设置分数，同时检查分数是否有异常
    void updateScore(int numLines); // 根据消行数更新分数
public:
    Tetris();
    ~Tetris();
    void run();
    Tetris& operator+=(int lines);
};
// 定义游戏异常类，继承自std::runtime_error，用于表示游戏逻辑错误
class GameLogicError : public std::runtime_error {
public:
    /**
    * @brief ctor接受一个错误消息并传递给基类构造函数
    * @param message 错误消息
    */
    GameLogicError(const std::string& message) : std::runtime_error(message) {}
};