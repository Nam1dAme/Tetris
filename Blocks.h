/* Blocks.h */
#pragma once
#include "graphics.h"
#include <vector>
#include <stdexcept>
// 方块的边为30像素，游戏区域宽10格，高20格
const int BLOCK_SIZE = 30;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// 游戏区域的宽高，用于堆放方块
const int GAME_AREA_WIDTH = BOARD_WIDTH * BLOCK_SIZE;
const int GAME_AREA_HEIGHT = BOARD_HEIGHT * BLOCK_SIZE;

// 游戏信息区域的宽高，用于展示得分等
const int INFO_AREA_X_OFFSET = 30 + GAME_AREA_WIDTH + 30;
const int INFO_AREA_Y_OFFSET = 30;

// 整个游戏窗口的宽高，用于打印提示信息
const int SCREEN_WIDTH = INFO_AREA_X_OFFSET + 150;
const int SCREEN_HEIGHT = 30 + GAME_AREA_HEIGHT + 30;

// 虚拟键盘码，用于处理键盘输入
#ifndef VK_LEFT
#define VK_LEFT   0x25
#define VK_UP     0x26
#define VK_RIGHT  0x27
#define VK_DOWN   0x28
#define VK_SPACE  0x20
#define VK_ESCAPE 0x1B
#endif
/**
 * @brief 俄罗斯方块基类，对所有方块进行封装
 */
class Blocks {
protected:
    int rotation;   // 方块的旋转状态
    color_t* color; // 颜色
    int speed;      // 下落速度
    int x, y;       // 初始坐标

    // shapes数组存储不同形状方块及其旋转状态
    // shapes[rotation][row][col]
    std::vector<std::vector<std::vector<int>>> shapes;

    static int aliveBlocks; // 静态成员变量，记录当前存在的方块数量

public:
    Blocks(color_t c, int xPoint, int yPoint);
    Blocks(const Blocks& other); // Deep copy
    virtual ~Blocks();

    void drop();            // 方块下落
    void move(int dx, int dy); // 移动方块
    void rotateBlock();     // 旋转方块
    virtual void draw(int gameAreaX, int gameAreaY) const;

    // Setters
    void setSpeed(int s);
    void setColor(color_t c);

    // Getters
    int getX();
    int getY();
    int getRotation();
    int getSpeed();
    color_t getBlockColor() const;
    const std::vector<std::vector<int>>& getCurrentShape() const;
    static int getAliveBlocks();

    // 以下是Operator的重载
    Blocks& operator++();       // 右移前缀
    Blocks operator++(int);     // 右移后缀
    Blocks& operator--();       // 左移前缀
    Blocks operator--(int);     // 左移后缀

    friend class Tetris;    // 友元类Tetris，将用于检测方块碰撞
};