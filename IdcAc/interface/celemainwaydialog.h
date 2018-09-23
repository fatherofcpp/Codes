#ifndef CELEMAINWAYDIALOG_H
#define CELEMAINWAYDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"

namespace Ui {
class CEleMainWayDialog;
}

class CEleMainWayDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CEleMainWayDialog(QWidget *parent = 0);
    ~CEleMainWayDialog();
    
private:
    Ui::CEleMainWayDialog *ui;
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
private:
    const str_outer_dev* pOuterDev;
};

#endif // CELEMAINWAYDIALOG_H
