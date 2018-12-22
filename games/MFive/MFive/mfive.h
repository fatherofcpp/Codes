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
	// ����
	void paintEvent(QPaintEvent *event);
	// ��������ƶ��������������
	void mouseMoveEvent(QMouseEvent *event);
	// ʵ������
	void mouseReleaseEvent(QMouseEvent *event);

private:
	GameModel *game; // ��Ϸָ��
	GameType game_type; // �洢��Ϸ����
	int clickPosRow, clickPosCol; // �洢�������λ��
	void initGame();
	void checkGame(int y, int x);

	private slots:
		void chessOneByPerson(); // ��ִ��
		void chessOneByAI(); // AI����

		void initPVPGame();
		void initPVEGame();
private:
	static const int kBoardMargin = 30; // ���̱�Ե��϶
	static const int kRadius = 15; // ���Ӱ뾶
	static const int kMarkSize = 6; // ���ӱ�Ǳ߳�
	static const int kBlockSize = 40; // ���ӵĴ�С
	static const int kPosDelta = 20; // �������ģ����������
	static const int kAIDelay = 700; // AI�����˼��ʱ��
};

#endif // MFIVE_H
