/* Blocks.cpp */
#include "Blocks.h"
#include <vector>

// 存活方块数量
int Blocks::aliveBlocks = 0;

// Blocks ctor
Blocks::Blocks(color_t c, int xPoint, int yPoint) : 
    rotation(0), x(xPoint), y(yPoint), speed(0) {
    ++aliveBlocks;
    color = new color_t(c);
}

// 重载ctor用于深拷贝
Blocks::Blocks(const Blocks& other) {
    x = other.x;
    y = other.y;
    rotation = other.rotation;
    shapes = other.shapes;
    color = new color_t(*(other.color));
    speed = other.speed;
    ++aliveBlocks;
}

// Blocks dtor
Blocks::~Blocks() {
    --aliveBlocks;
    delete color;
}
/**
* @brief 绘制当前方块
* @param gameAreaX,gameAreaY 游戏区域的起始x，y坐标
*/
void Blocks::draw(int gameAreaX, int gameAreaY) const {
    if (!color) return;
    setfillcolor(*color);
    const auto& currentShape = getCurrentShape();
    for (size_t i = 0; i < currentShape.size(); ++i) {
        for (size_t j = 0; j < currentShape[i].size(); ++j) {
            if (currentShape[i][j]) {
                int drawX = (x + static_cast<int>(j)) * BLOCK_SIZE + gameAreaX;
                int drawY = (y + static_cast<int>(i)) * BLOCK_SIZE + gameAreaY;
                bar(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE);
            }
        }
    }
}
/**
* @brief 旋转方块
*/
void Blocks::rotateBlock() {
    if (!shapes.empty()) {
        rotation = (rotation + 1) % shapes.size();
    }
}
// 方块下落
void Blocks::drop() {
    y++;
}
/**
* @brief 方块移动
* @param dx,dy 方块的移动距离
*/
void Blocks::move(int dx, int dy) {
    x += dx;
    y += dy;
}
// getters
const std::vector<std::vector<int>>& Blocks::getCurrentShape() const { return shapes[rotation]; }
color_t Blocks::getBlockColor() const { return *color; }
int Blocks::getX() { return x; }
int Blocks::getY() { return y; }
int Blocks::getRotation() { return rotation; }
int Blocks::getSpeed() { return speed; }
int Blocks::getAliveBlocks() { return aliveBlocks; }

// setters
void Blocks::setSpeed(int s) { speed = s; }
void Blocks::setColor(color_t c) {
    // 此处考虑color为空的情况
    if (color) {
        *color = c;
    }
    else {
        color = new color_t(c);
    }
}

// 以下为运算符重载
Blocks& Blocks::operator++() {   // 前缀右移
    this->x++;
    return *this;
}
Blocks Blocks::operator++(int) {    // 后缀右移
    Blocks temp = *this;
    this->x++;
    return temp;
}
Blocks& Blocks::operator--() {  // 前缀左移
    this->x--;
    return *this;
}
Blocks Blocks::operator--(int) {    // 后缀左移
    Blocks temp = *this;
    this->x--;
    return temp;
}