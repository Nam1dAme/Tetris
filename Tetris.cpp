/* Tetris.cpp */
#include "Tetris.h"
#include "graphics.h"
#include "Blocks.h"

// 以下为七种方块的类，继承自Blocks类
class LongRetangle : public Blocks {
public: LongRetangle(int x, int y) : Blocks(CYAN, x, y) { 
    shapes = { 
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}} 
        }; 
    }
};
class Square : public Blocks {
public: Square(int x, int y) : Blocks(RED, x, y) { 
    shapes = { 
        {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}} 
        };
    }
};
class LShape : public Blocks {
public: LShape(int x, int y) : Blocks(GREEN, x, y) { 
    shapes = { 
        {{1,0,0,0},{1,0,0,0},{1,1,0,0},{0,0,0,0}},
        {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}} 
        }; 
    }
};
class TShape : public Blocks {
public: TShape(int x, int y) : Blocks(YELLOW, x, y) { 
    shapes = { 
        {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}} 
        }; 
    }
};
class ZShape : public Blocks {
public: ZShape(int x, int y) : Blocks(BROWN, x, y) { 
    shapes = { 
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}} 
        }; 
    }
};
class ReverseLShape : public Blocks {
public: ReverseLShape(int x, int y) : Blocks(BLUE, x, y) { 
    shapes = { 
        {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}},
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0}},
        {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}} 
        }; 
    }
};
class ReverseZShape : public Blocks {
public: ReverseZShape(int x, int y) : Blocks(MAGENTA, x, y) { 
    shapes = { 
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}} 
        }; 
    }
};

// Tetris ctor
Tetris::Tetris()
    : currentBlock(nullptr), nextBlock(nullptr), score(0),
    highScore(0), level(1), linesClearedTotal(0), gameOver(false),
    lastDropTime(0), dropDelay(1000) {
    initBoard();
    loadSettings();
    loadHighScore();
    randomize();      // ege库函数,用于设置初始随机种子
    selectNextBlock();
    spawnNewBlock();
}
// Tetris dtor 
Tetris::~Tetris() {
    delete currentBlock;
    delete nextBlock;
    saveHighScore();
}

void Tetris::initBoard() {  // 初始化游戏区域
    board.assign(BOARD_HEIGHT, std::vector<color_t>(BOARD_WIDTH, 0));   // 往board中填充30个(10,0)
}


void Tetris::loadSettings() {
    std::ifstream diffFile("difficulty.txt");
    int diffLevelFromFile = 1; // 默认难度为1
    if (diffFile.is_open()) {
        try {
            diffFile >> diffLevelFromFile;
            if (diffFile.fail()) {
                throw std::ios_base::failure("无法读取难度文件");
            }
            if (diffLevelFromFile == 0) dropDelay = 1200;       // 简单
            else if (diffLevelFromFile == 1) dropDelay = 1000;  // 普通
            else if (diffLevelFromFile == 2) dropDelay = 800;   // 困难
            else dropDelay = 1000;  // 默认为普通难度

            level = diffLevelFromFile + 1;
        }
        catch (...) {
            setfont(12, 0, "宋体");
            outtextxy(10, SCREEN_HEIGHT - 15, "难度文件错误，将使用默认难度");
            delay_ms(100);
        }
        diffFile.close();
    }
}


void Tetris::loadHighScore() {
    std::ifstream file("record.txt");
    if (file.is_open()) {
        file >> highScore;
        if (file.fail()) {
            highScore = 0;
            std::cerr << "无法读取record.txt" << std::endl;
        }
        file.close();
    }
    else {
        highScore = 0;
    }
}

void Tetris::saveHighScore() {
    std::ofstream file("record.txt");
    if (file.is_open()) {
        file << highScore;
        if (file.fail()) {
            std::cerr << "无法写入record.txt" << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "无法打开record.txt" << std::endl;
    }
}

void Tetris::selectNextBlock() {
    int type = random(7);
    int initialX = BOARD_WIDTH / 2 - 2;
    switch (type) {
    case 0: nextBlock = new LongRetangle(initialX, 0); break;
    case 1: nextBlock = new Square(initialX, 0); break;
    case 2: nextBlock = new LShape(initialX, 0); break;
    case 3: nextBlock = new TShape(initialX, 0); break;
    case 4: nextBlock = new ZShape(initialX, 0); break;
    case 5: nextBlock = new ReverseLShape(initialX, 0); break;
    case 6: nextBlock = new ReverseZShape(initialX, 0); break;
    default: nextBlock = new Square(initialX, 0); break; // Fallback
    }
}


void Tetris::spawnNewBlock() {
    delete currentBlock;
    currentBlock = nextBlock;
    if (currentBlock) {
        currentBlock->x = BOARD_WIDTH / 2 - 2; 
        currentBlock->y = 0;
    }

    selectNextBlock();

    if (currentBlock && checkCollision(currentBlock, currentBlock->getX(), currentBlock->getY(), currentBlock->getRotation())) {
        gameOver = true;
    }
    lastDropTime = fclock() * 1000;
}

/**
* @brief 检查方块是否碰撞
* @param block 方块指针
* @param testX, testY 方块的x,y坐标
* @param testRotation 方块的旋转状态
* @return 是否碰撞
* @details
* 1.获取当前方块的形状数组，遍历每个元素
* 2.对每个存在的元素，计算其在游戏区域中的位置
* 3.检查是否超出游戏区域边界，或者是否与已落下的方块碰撞
*/
bool Tetris::checkCollision(const Blocks* block, int testX, int testY, int testRotation) const {
    if (!block) return true;    // 空指针默认为碰撞
    const auto& shape = block->shapes[testRotation];    // 直接访问友元类Blocks的私有成员

    for (size_t r = 0; r < shape.size(); ++r) {
        for (size_t c = 0; c < shape[r].size(); ++c) {
            if (shape[r][c]) {  // 仅处理非空元素
                int boardX = testX + static_cast<int>(c);   // 静态类型转换
                int boardY = testY + static_cast<int>(r);
                // 检查是否与边界碰撞
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return true;
                }
                // 检查是否与其他方块碰撞
                if (boardY >= 0 && board[boardY][boardX] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}
/**
* @brief 将方块合并到游戏区域
* @details
* 
*/
void Tetris::mergeBlockToBoard() {
    if (!currentBlock) return;
    const auto& shape = currentBlock->getCurrentShape();
    color_t blockColor = currentBlock->getBlockColor();
    for (size_t r = 0; r < shape.size(); ++r) {
        for (size_t c = 0; c < shape[r].size(); ++c) {
            if (shape[r][c]) {
                int boardX = currentBlock->getX() + static_cast<int>(c);
                int boardY = currentBlock->getY() + static_cast<int>(r);
                if (boardY >= 0 && boardY < BOARD_HEIGHT && boardX >= 0 && boardX < BOARD_WIDTH) {
                    board[boardY][boardX] = blockColor;
                }
            }
        }
    }
}

/**
* @brief 消除填满的行
* @return 消除的行数
*/
int Tetris::clearFullLines() {
    int linesClearedThisTurn = 0;
    for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
        bool lineIsFull = true;
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (board[r][c] == 0) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            linesClearedThisTurn++;
            // 将消行上方的方块下移
            for (int moveR = r; moveR > 0; --moveR) {
                board[moveR] = board[moveR - 1];
            }
            board[0].assign(BOARD_WIDTH, 0);
            r++;
        }
    }
    return linesClearedThisTurn;
}
/**
* @brief 设置分数
* @param newScore 新分数
*/
void Tetris::setScore(int newScore) {
    if (newScore > score + 10000 && newScore > highScore + 10000 && score > 0) { 
        throw GameLogicError("分数异常");
    }
    score = newScore;
    if (score > highScore) {
        highScore = score;
    }
    
}

/*
* @brief 根据消行数更新分数，每行100分
* @param numLines 消行数
*/
void Tetris::updateScore(int numLines) {
    if (numLines <= 0 || numLines > 4) return;
    try {
        setScore(score + 100 * numLines);
    }
    catch (const GameLogicError& e) {
        score += 100;
        if (score > highScore) highScore = score;
        setfont(12, 0, "Consolas");
        outtextxy(360, 180, "分数异常");
    }
    linesClearedTotal += numLines;
    // 根据消行数提高等级，等级越高下落越快
    if (linesClearedTotal / (level * 5) >= 1 && level < 15) {
        dropDelay = (std::max)(100L, dropDelay - 50);
    }
}

// 重载运算符+=，用于增加分数
Tetris& Tetris::operator+=(int lines) {
    updateScore(lines);
    return *this;
}


void Tetris::handlePlayerInput() {
    if (keystate(VK_LEFT)) {    // 左移
        moveBlock(-1, 0);
        delay_ms(100);
    }
    else if (keystate(VK_RIGHT)) {  // 右移
        moveBlock(1, 0);
        delay_ms(100);
    }
    else if (keystate(VK_DOWN)) {   // 加速下落
        moveBlock(0, 1);
        lastDropTime = fclock() * 1000;
    }
    else if (keystate(VK_UP) || keystate(VK_SPACE)) {   // 旋转
        rotateCurrentBlock();
        delay_ms(150);
    }
}
/**
* @brief 移动方块
* @param dx 方块的x方向移动量
* @param dy 方块的y方向移动量
*/
void Tetris::moveBlock(int dx, int dy) {
    if (!currentBlock || gameOver) return;
    if (dx > 0) {   // dx > 0 表示向右移动
        if (!checkCollision(currentBlock, currentBlock->getX() + 1, currentBlock->getY(), currentBlock->getRotation())) {
            ++(*currentBlock);
        }
    }
    else if (dx < 0) {  // dx < 0 表示向左移动
        if (!checkCollision(currentBlock, currentBlock->getX() - 1, currentBlock->getY(), currentBlock->getRotation())) {
            --(*currentBlock);
        }
    }
    if (dy > 0) {   // dy > 0 表示向下移动
        if (!checkCollision(currentBlock, currentBlock->getX(), currentBlock->getY() + 1, currentBlock->getRotation())) {
            currentBlock->drop();
        }
        else { // 与下方的方块碰撞，将当前方块合并到游戏区域
            mergeBlockToBoard();
            *this += clearFullLines();  // 使用+=运算符增加分数
            spawnNewBlock();
        }
    }
}
/**
* @brief 旋转当前方块
*/
void Tetris::rotateCurrentBlock() {
    if (!currentBlock || gameOver) return;
    // 保存原始旋转状态，以便恢复
    int originalRotation = currentBlock->getRotation();
    currentBlock->rotateBlock();
    if (checkCollision(currentBlock, currentBlock->getX(), currentBlock->getY(), currentBlock->getRotation())) {
        // 检查右移是否碰撞
        if (!checkCollision(currentBlock, currentBlock->getX() + 1, currentBlock->getY(), currentBlock->getRotation())) {
            currentBlock->move(1, 0);
        }
        // 检查左移是否碰撞
        else if (!checkCollision(currentBlock, currentBlock->getX() - 1, currentBlock->getY(), currentBlock->getRotation())) {
            currentBlock->move(-1, 0);
        }
        else {  // 如果无法满足，则向反方向旋转回原状态
            currentBlock->rotation = originalRotation;
        }
    }
}
/*
* @brief 绘制游戏画面
* 依次绘制游戏区域、当前方块、下一个方块、UI信息、游戏结束画面
*/
void Tetris::renderGame() const {
    cleardevice();
    drawBoard();
    if (!gameOver) {
        if (currentBlock) currentBlock->draw(30, 30);
        if (nextBlock) {
            // 在info区域绘制下一个方块的预览
            setfont(16, 0, "Consolas");
            outtextxy(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET, "Next:");
            Blocks tempNext = *nextBlock;   // 创建临时方块对象，将其坐标设置为(0,0)，用于预览
            tempNext.x = 0;
            tempNext.y = 0;
            // 在info区域下方绘制临时方块
            tempNext.draw(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET + 30); 
        }
    }
    drawUI();
    if (gameOver) {
        drawGameOverScreen();
    }
}

/**
* @brief 绘制游戏区域
*/
void Tetris::drawBoard() const {
    // 设置背景色为亮灰，绘制游戏区域边框
    setcolor(DARKGRAY);
    rectangle(30 - 1, 30 - 1,
        30 + GAME_AREA_WIDTH + 1, 30 + GAME_AREA_HEIGHT + 1);

    // 绘制游戏区域内的方块
    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (board[r][c] != 0) {
                setfillcolor(board[r][c]);
                int x1 = 30 + c * BLOCK_SIZE;
                int y1 = 30 + r * BLOCK_SIZE;
                bar(x1, y1, x1 + BLOCK_SIZE, y1 + BLOCK_SIZE);
            }
        }
    }
}

/**
* @brief 绘制游戏UI，用于显示分数、等级、行数
* 每条信息间隔30个像素
*/
void Tetris::drawUI() const {
    setfont(18, 0, "Consolas");
    setcolor(BLACK);
    char buffer[50];    // 定义缓冲区用于格式化字符串
    // 当前得分
    sprintf(buffer, "Score: %d", score);
    outtextxy(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET + 120, buffer);
    // 最高分
    sprintf(buffer, "High Score: %d", highScore);
    outtextxy(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET + 150, buffer);
    // 当前等级
    sprintf(buffer, "Level: %d", level);
    outtextxy(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET + 180, buffer);
    // 累计已消除行数
    sprintf(buffer, "Lines: %d", linesClearedTotal);
    outtextxy(INFO_AREA_X_OFFSET, INFO_AREA_Y_OFFSET + 210, buffer);
}

/**
* @brief 绘制游戏结束画面
*/
void Tetris::drawGameOverScreen() const {
    setfont(48, 0, "Consolas");
    setcolor(RED);
    int textWidth = textwidth("GAME OVER");
    outtextxy(SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2 - 50, "GAME OVER");

    setfont(20, 0, "Consolas");
    char scoreStr[50];
    sprintf(scoreStr, "Final Score: %d", score);
    textWidth = textwidth(scoreStr);
    outtextxy(SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2 + 10, scoreStr);
    outtextxy(SCREEN_WIDTH / 2 - textwidth("Press ESC to Exit") / 2, SCREEN_HEIGHT / 2 + 40, "Press ESC to Exit");
}
/**
* @brief 游戏运行
* 初始化图形界面，进入游戏主循环
*/
void Tetris::run() {
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT); 
    setbkcolor(WHITE);
    // 游戏主循环
    while (!gameOver) { 
        if (!gameOver) {
            handlePlayerInput();
            updateGameLogic();
            renderGame();
            delay_ms(30);   // 默认游戏帧率为30fps
        } else { 
            break;  // 游戏结束，跳出循环
        }
    }
    closegraph();
}

/**
* @brief 更新游戏状态，实现游戏随时间进行
*/
void Tetris::updateGameLogic() {
    if (gameOver) return;
    long long currentTime = fclock() * 1000;
    if (currentTime - lastDropTime > dropDelay) {
        moveBlock(0, 1);
        lastDropTime = currentTime;
    }
}
