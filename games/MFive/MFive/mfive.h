#ifndef MFIVE_H
#define MFIVE_H

#include <QtWidgets/QMainWindow>
#include "ui_mfive.h"
#include "GameModel.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MFiveClass ui;


protected:
	// 绘制
	void paintEvent(QPaintEvent *event);
	// 监听鼠标移动情况，方便落子
	void mouseMoveEvent(QMouseEvent *event);
	// 实际落子
	void mouseReleaseEvent(QMouseEvent *event);

private:
	GameModel *game; // 游戏指针
	GameType game_type; // 存储游戏类型
	int clickPosRow, clickPosCol; // 存储将点击的位置
	void initGame();
	void checkGame(int y, int x);

	private slots:
		void chessOneByPerson(); // 人执行
		void chessOneByAI(); // AI下棋

		void initPVPGame();
		void initPVEGame();
private:
	static const int kBoardMargin = 30; // 棋盘边缘空隙
	static const int kRadius = 15; // 棋子半径
	static const int kMarkSize = 6; // 落子标记边长
	static const int kBlockSize = 40; // 格子的大小
	static const int kPosDelta = 20; // 鼠标点击的模糊距离上限
	static const int kAIDelay = 700; // AI下棋的思考时间
};

#endif // MFIVE_H
