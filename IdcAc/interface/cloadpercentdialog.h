#ifndef CLOADPERCENTDIALOG_H
#define CLOADPERCENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include "ccommonchild.h"
#include "DataTypeDef.h"
namespace Ui {
class CLoadPercentDialog;
}

class CLoadPercentDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CLoadPercentDialog(QWidget *parent = 0);
    ~CLoadPercentDialog();
    
private:
    Ui::CLoadPercentDialog *ui;
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();
public:
    void SetUi(const st_dev_inf* p_dev_inf);
    void SetOuterDev(const str_outer_dev* outer_dev);
private:
    void InitUi();
private:
    const str_outer_dev* pOuterDev;
    QLineEdit* lineEdit[SUB_WAY_PARAM_NUM];
    QLabel* label[SUB_WAY_PARAM_NUM];
};

#endif // CLOADPERCENTDIALOG_H
