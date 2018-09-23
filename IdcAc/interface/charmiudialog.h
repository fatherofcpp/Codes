#ifndef CHARMIUDIALOG_H
#define CHARMIUDIALOG_H

#include <QDialog>
#include "qcustomplot.h"
#include "ccommonchild.h"
#include "DataTypeDef.h"
namespace Ui {
class CHarmIUDialog;
}

class CHarmIUDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CHarmIUDialog(QWidget *parent = 0);
    ~CHarmIUDialog();
    
private:
    Ui::CHarmIUDialog *ui;
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();

    void slot_pre_list_clicked();
    void slot_next_list_clicked();
public:
    void SetUi(const st_dev_inf* p_dev_inf);
    void SetOuterDev(const str_outer_dev* outer_dev);
private:
    void InitUi();
    void SetList();
private:
    const str_outer_dev* pOuterDev;
    MU_INT16 harmPercent[30];
    QCPBars* pQCPBars;
};

#endif // CHARMIUDIALOG_H
