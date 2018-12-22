#include <QPainter>
#include <QTimer>
//#include <QSound>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include "mfive.h"
static const QString CHESS_ONE_SOUND = ":/res/sound/chessone.wav";
static const QString WIN_SOUND = ":/res/sound/win.wav";
static const QString LOSE_SOUND = ":/res/sound/lose.wav";
static int m_sqrt(int val)
{
	int v = sqrt((double)val); 
	return v;
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	// �������̴�С
	setFixedSize(kBoardMargin * 2 + kBlockSize * ckeyBoardSizeNum, kBoardMargin * 2 + kBlockSize * ckeyBoardSizeNum);
	//    setStyleSheet("background-color:yellow;");

	// �������hover���ܣ�������һ��Ҫ����window��
	setMouseTracking(true);
	//    centralWidget()->setMouseTracking(true);

	// ��Ӳ˵�
	QMenu *gameMenu = menuBar()->addMenu(tr("Game")); // menuBarĬ���Ǵ��ڵģ�ֱ�ӼӲ˵��Ϳ�����
	QAction *actionPVP = new QAction("Person VS Person", this);
	connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
	gameMenu->addAction(actionPVP);

	QAction *actionPVE = new QAction("Person VS Computer", this);
	connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
	gameMenu->addAction(actionPVE);

	// ��ʼ��Ϸ
	initGame();
}

MainWindow::~MainWindow()
{
	if (game)
	{
		delete game;
		game = nullptr;
	}
}

void MainWindow::initGame()
{   
	// ��ʼ����Ϸģ��
	game = new GameModel;
	initPVPGame();
}

void MainWindow::initPVPGame()
{
	game_type = PERSON;
	game->gameStatus = PLAYING;
	game->startGame(game_type);
	update();
}

void MainWindow::initPVEGame()
{
	game_type = BOT;
	game->gameStatus = PLAYING;
	game->startGame(game_type);
	update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	// ��������
	painter.setRenderHint(QPainter::Antialiasing, true); // �����
	//    QPen pen; // �����������
	//    pen.setWidth(2);
	//    painter.setPen(pen);
	for (int i = 0; i < ckeyBoardSizeNum + 1; i++)
	{
		painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
		painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
	}

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	// �������ӱ��(��ֹ������Խ��)
	if (clickPosRow > 0 && clickPosRow < ckeyBoardSizeNum &&
		clickPosCol > 0 && clickPosCol < ckeyBoardSizeNum &&
		game->gameMapVec[clickPosRow][clickPosCol] == 0)
	{
		if (game->playerFlag)
			brush.setColor(Qt::white);
		else
			brush.setColor(Qt::black);
		painter.setBrush(brush);
		painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
	}

	// �������� 
	for (int i = 0; i < ckeyBoardSizeNum; i++)
		for (int j = 0; j < ckeyBoardSizeNum; j++)
		{
			if (game->gameMapVec[i][j] == 1)
			{
				brush.setColor(Qt::white);
				painter.setBrush(brush);
				painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
			else if (game->gameMapVec[i][j] == -1)
			{
				brush.setColor(Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
		}

		// �ж���Ӯ
		if (clickPosRow > 0 && clickPosRow < ckeyBoardSizeNum &&
			clickPosCol > 0 && clickPosCol < ckeyBoardSizeNum &&
			(game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
			game->gameMapVec[clickPosRow][clickPosCol] == -1))
		{
			if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
			{
				qDebug() << "win";
				game->gameStatus = WIN;
				//QSound::play(WIN_SOUND);
				QString str;
				if (game->gameMapVec[clickPosRow][clickPosCol] == 1)
					str = "white player";
				else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
					str = "black player";
				QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + " win!");

				// ������Ϸ״̬������������ѭ��
				if (btnValue == QMessageBox::Ok)
				{
					game->startGame(game_type);
					game->gameStatus = PLAYING;
				}
			}
		}


		// �ж�����
		if (game->isDeadGame())
		{
			//QSound::play(LOSE_SOUND);
			QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
			if (btnValue == QMessageBox::Ok)
			{
				game->startGame(game_type);
				game->gameStatus = PLAYING;
			}

		}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{   
	// ͨ������hoverȷ�����ӵı��
	int x = event->x();
	int y = event->y();

	// ���̱�Ե��������
	if (x >= kBoardMargin + kBlockSize / 2 &&
		x < size().width() - kBoardMargin &&
		y >= kBoardMargin + kBlockSize / 2 &&
		y < size().height()- kBoardMargin)
	{
		// ��ȡ��������Ͻǵĵ�
		int col = x / kBlockSize;
		int row = y / kBlockSize;

		int leftTopPosX = kBoardMargin + kBlockSize * col;
		int leftTopPosY = kBoardMargin + kBlockSize * row;

		// ���ݾ���������ʵĵ��λ��,һ���ĸ��㣬���ݰ뾶����ѡ�����
		clickPosRow = -1; // ��ʼ�����յ�ֵ
		clickPosCol = -1;
		int len = 0; // �������ȡ���Ϳ�����

		// ȷ��һ������ڷ�Χ�ڵĵ㣬��ֻ����ȷ��һ������
		len = m_sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickPosRow = row;
			clickPosCol = col;
		}
		len = m_sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickPosRow = row;
			clickPosCol = col + 1;
		}
		len = m_sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickPosRow = row + 1;
			clickPosCol = col;
		}
		len = m_sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickPosRow = row + 1;
			clickPosCol = col + 1;
		}
	}

	// ���������ҲҪ�ػ�
	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	// �����壬���Ҳ�������������
	if (!(game_type == BOT && !game->playerFlag))
	{
		chessOneByPerson();
		// ������˻�ģʽ����Ҫ����AI����
		if (game->gameType == BOT && !game->playerFlag)
		{
			// �ö�ʱ����һ���ӳ�
			QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
		}
	}

}

void MainWindow::chessOneByPerson()
{
	// ���ݵ�ǰ�洢����������
	// ֻ����Ч��������ӣ����Ҹô�û����
	if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
	{
		game->actionByPerson(clickPosRow, clickPosCol);
		//QSound::play(CHESS_ONE_SOUND);

		// �ػ�
		update();
	}
}

void MainWindow::chessOneByAI()
{
	game->actionByAI(clickPosRow, clickPosCol);
//	QSound::play(CHESS_ONE_SOUND);
	update();
}

