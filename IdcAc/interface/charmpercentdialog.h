#ifndef CHARMPERCENTDIALOG_H
#define CHARMPERCENTDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"
namespace Ui {
class CHarmPercentDialog;
}

class CHarmPercentDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CHarmPercentDialog(QWidget *parent = 0);
    ~CHarmPercentDialog();
    
private:
    Ui::CHarmPercentDialog *ui;
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
};

#endif // CHARMPERCENTDIALOG_H
