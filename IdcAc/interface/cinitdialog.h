#ifndef CINITDIALOG_H
#define CINITDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
namespace Ui {
class CInitDialog;
}

class CInitDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CInitDialog(QWidget *parent = 0);
    ~CInitDialog();
    
private:
    Ui::CInitDialog *ui;

signals:
    void signal_change_child_dialog(int ind);
    void signal_is_auto_2_warn_dialog(bool is_auto_2_warn_dialog);
private slots:
    void slot_to_main_page_clicked();
private:
    void InitUi();
};

#endif // CINITDIALOG_H
