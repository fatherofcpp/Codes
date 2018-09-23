#ifndef CELESUBWAYDIALOG_H
#define CELESUBWAYDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"

typedef struct
{
    MU_INT16 current;//电流
    MS_INT16 power[3];//有功,无功，视在功率
    MS_INT16 power_factor;//功率因数
    MU_INT32 energy[2];//有，无功电能
}st_sub_way_ele_param;

namespace Ui {
class CEleSubWayDialog;
}

class CEleSubWayDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CEleSubWayDialog(QWidget *parent = 0);
    ~CEleSubWayDialog();
    
private:
    Ui::CEleSubWayDialog *ui;
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
    void SetList(int way_ind);
private:
    const str_outer_dev* pOuterDev;
    st_sub_way_ele_param* eleParam[E_SUB_WAY_NUM][SUB_WAY_PARAM_NUM];
};

#endif // CELESUBWAYDIALOG_H
