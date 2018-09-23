#ifndef CKEYBUTTON_H
#define CKEYBUTTON_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <vector>
using namespace std;

#include "common_qt.h"
#include "myqpushbutton.h"
#include "csqlite.h"

typedef enum
{
    E_VALID_INT = 0,//int
    E_VALID_FLOAT,//预留
    E_VALID_ALL_TYPE//所有类型
}e_valid_type;

namespace Ui {
class CKeyBoard;
}

class CKeyBoard : public QDialog
{
    Q_OBJECT
    
public:
    explicit CKeyBoard(QWidget *parent = 0);
    virtual ~CKeyBoard();
    
signals:
    void signal_character_selected(QString key_val);
    void signal_edit_finished(QString text);
    void signal_edit_delete();
private slots:
    void slot_bt_key_clicked(const st_bt_inf& bt_inf);
    void slot_bt_finish_input();
    void slot_bt_delete_input();
    void slot_bt_to_chinese();
    void slot_bt_delete_pinyin();
    void slot_bt_choose_input();
    void slot_line_pinyin_text_change(QString str);
public:
    void SetLine(QString val, QString lab = "");
    void SetValid(MU_INT32 type);
    void SetShowText(QLineEdit::EchoMode mode);
    void SetFont(QFont* p_QFont);
private:
    void ChineseShow();
private:
    Ui::CKeyBoard *ui;
private:
    CMyQPushButton* btArrKey;
    CMyQPushButton* btArrChinese;
    QLabel* labInput;
    QLineEdit* lineInput;
    QPushButton* btFinishInput;
    QPushButton* btDeleteInput;
    QPushButton* btChooseInput;
    QLabel* labPinyin;
    QLineEdit* linePinyin;
    QPushButton* btDeletePinyin;

    CSqlite* pCSqlite;

    MU_INT32 keyNum;
    MU_INT32 chineseNumShow;//num of show chinese
    vector<void*> vecWidget;
    vector<string> vecChinese;
    int startIndShow;//chinese show ind
    bool isEnglishInput;
};

#endif // CKEYBUTTON_H
