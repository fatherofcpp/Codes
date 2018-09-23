#ifndef CSWITCHSTATEDIALOG_H
#define CSWITCHSTATEDIALOG_H

#include <QDialog>
#include "keyboard/myqpushbutton.h"
#include "ccommonchild.h"
#include "DataTypeDef.h"
#include "DataTypeDef.h"

namespace Ui {
class CSwitchStateDialog;
}

class CSwitchStateDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CSwitchStateDialog(QWidget *parent = 0);
    ~CSwitchStateDialog();
    
private:
    Ui::CSwitchStateDialog *ui;
protected:
    virtual void paintEvent(QPaintEvent *);
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();
public:
    void SetOuterDev(const str_outer_dev* outer_dev);
    void SetUi(const st_dev_inf* p_dev_inf);
private:
    void InitUi();
    void SetLight(int ui_ind, bool is_on);
private:
    const str_outer_dev* pOuterDev;
    CMyQPushButton* lightBt[SUB_WAY_PARAM_NUM];
    QLabel* labelW[SUB_WAY_PARAM_NUM];
};

#endif // CSWITCHSTATEDIALOG_H
