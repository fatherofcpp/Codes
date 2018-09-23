#include "cusergroupdialog.h"
#include "ui_cusergroupdialog.h"
#include "CommonFunction.h"
#include <QLabel>
#include <QScrollBar>

static MU_INT32 s_row_max = 128;
static MU_INT32 s_colum = 3;
CUserGroupDialog::CUserGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CUserGroupDialog)
{
    ui->setupUi(this);
    setWindowTitle("用户管理");

    pCUserInfDialog = new CUserInfDialog;
    deqStUserInf.clear();

    connect(ui->pushButton_add, SIGNAL(clicked()),
            this, SLOT(slot_bt_add_clicked()));
    connect(ui->pushButton_delete, SIGNAL(clicked()),
            this, SLOT(slot_bt_delete_clicked()));
    connect(ui->pushButton_ok, SIGNAL(clicked()),
            this, SLOT(slot_bt_ok_clicked()));
    connect(ui->pushButton_cancle, SIGNAL(clicked()),
            this, SLOT(slot_bt_cancel_clicked()));
    connect(ui->pushButton_pre_list, SIGNAL(clicked()),
            this, SLOT(slot_pre_list_clicked()));
    connect(ui->pushButton_next_list, SIGNAL(clicked()),
            this, SLOT(slot_next_list_clicked()));

    InitUi();
}

CUserGroupDialog::~CUserGroupDialog()
{
    delete ui;
    delete pCUserInfDialog;
}

void CUserGroupDialog::slot_bt_add_clicked()
{
    QTableWidget* table = ui->tableWidget;
    QTableWidgetItem* item = NULL;
    int ret = 0;
    pCUserInfDialog->SetUserInfType(E_USER_ADD);
    ret = pCUserInfDialog->exec();
    if (ret == QDialog::Accepted)
    {
        st_user_inf* p_user_inf = pCUserInfDialog->GetUserInf();
        int row_empty = -1;
        for (MU_INT32 row_ind = 0; row_ind < s_row_max; ++row_ind)
        {
            MU_INT32 column_ind = 0;
            item = table->item(row_ind, column_ind);
            QString txt = item->text();
            if(txt.length() == 0)
            {
                row_empty = row_ind;
                break;
            }
            if(0 == strcmp(qstr_2_str(txt), p_user_inf->user_name))
            {
                goto ERR;
            }
        }

        //insert
        if(row_empty > 0)
        {
            for (MU_INT32 column_ind = 0; column_ind < s_colum; ++column_ind)
            {
                QString txt;
                if(column_ind == 0)
                {
                    txt = str_2_qstr(p_user_inf->user_name);
                }
                else if(column_ind == 1)
                {
                    txt = str_2_qstr(p_user_inf->user_descript);
                }
                else if(column_ind == 2)
                {
                    txt = str_2_qstr(p_user_inf->user_pwd);
                }
                item = table->item(row_empty, column_ind);
                item->setText(txt);
            }
        }
        return;
    }
    else
    {
        return;
    }

ERR:
    QMessageBox::information(this, "操作", "新增用户已经存在");
    return;
}

void CUserGroupDialog::slot_bt_delete_clicked()
{
    QTableWidget* table = ui->tableWidget;
    QTableWidgetItem* item = NULL;
    int row_index = table->currentRow();
    if(row_index == -1)
    {
        return;
    }
    if(row_index == 0)
    {
        QMessageBox::information(this, "操作", "无权限");
        return;
    }
    for (MU_INT32 colum_ind = 0; colum_ind < s_colum; ++colum_ind)
    {
        item = table->item(row_index, colum_ind);
        item->setText("");
    }
}

void CUserGroupDialog::slot_bt_ok_clicked()
{
    QTableWidget* table = ui->tableWidget;
    QString txt;
    QTableWidgetItem* item = NULL;

    st_user_inf user_inf;
    memset(&user_inf, 0, sizeof(user_inf));

    deqStUserInf.clear();

    for (MU_INT32 row_ind = 0; row_ind < s_row_max; ++row_ind)
    {
        for (MU_INT32 column_ind = 0; column_ind < s_colum; ++column_ind)
        {
            item = table->item(row_ind, column_ind);
            txt = item->text();
            if(txt.length() == 0)
            {
                break;
            }
            if(column_ind == 0)
            {
                strcpy(user_inf.user_name, qstr_2_str(txt));
            }
            else if(column_ind == 1)
            {
                strcpy(user_inf.user_descript, qstr_2_str(txt));
            }
            else if(column_ind == 2)
            {
                strcpy(user_inf.user_pwd, qstr_2_str(txt));
            }

            if(column_ind == s_colum - 1)
            {
                deqStUserInf.push_back(user_inf);
            }
        }
    }

    this->done(QDialog::Accepted);
}

void CUserGroupDialog::slot_bt_cancel_clicked()
{
    this->done(QDialog::Rejected);
}

static int pageValue = 10; // 一页显示条数
void CUserGroupDialog::slot_pre_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value();

    if(cur_scroller - pageValue >= 0)
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(cur_scroller - pageValue);
    }
    else
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(max_value);
    }
}

void CUserGroupDialog::slot_next_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

    if(pageValue + cur_scroller <= max_value)
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue + cur_scroller);
    }
    else
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    }
}

void CUserGroupDialog::SetDeqUserInf(DEQ_ST_USER_INF *p_deqStUserInf)
{
    deqStUserInf = *p_deqStUserInf;
    SetList();
}

void CUserGroupDialog::InitUi()
{
    ui->pushButton_pre_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_list->setStyleSheet(gResArrowUp);
    ui->pushButton_next_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_list->setStyleSheet(gResArrowDown);

    //table
    QTableWidget* table_arr[1];
    table_arr[0] = ui->tableWidget;

    for(int table_ind = 0; table_ind < 1; table_ind++)
    {
        QTableWidget* table = table_arr[table_ind];
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        int rows = s_row_max;
        int colums = s_colum;

        table->setRowCount(rows);
        table->setColumnCount(colums);

        //设置左上角文字
        QLabel*label = new QLabel(table);
        label->setText("编号");
        label->move(5,5);
        label->installEventFilter(this);

        //设置表格行标题
        QStringList header_labels;
        header_labels.append("用户名");
        header_labels.append("用户描述");
        header_labels.append("用户密码");
        table->setHorizontalHeaderLabels(header_labels);

        //设置表格行标题的对齐方式
        table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

        //设置每一行的高度
        for(int i = 0; i < rows; i++)
        {
            //   table->setRowHeight(i, 22);
        }
        for(int i = 0; i < colums; i++)
        {
            int width = 180;
            table->setColumnWidth(i, width);
        }

        //自动调整最后一列的宽度使它和表格的右边界对齐
        table->horizontalHeader()->setStretchLastSection(true);

        //设置表格的选择方式
        table->setSelectionBehavior(QAbstractItemView::SelectRows);

        //设置编辑方式
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //设置宽度固定
        table->verticalHeader()->setResizeMode(QHeaderView::Fixed);
        table->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

        //设置行表题
        QStringList row_labels;
        for(int i = 0; i < rows; i++)
        {
            QString str;
            str = QString::number(i + 1) + QString("    ");
            row_labels.append(str);
        }
        table->setVerticalHeaderLabels(row_labels);

        //设置表格内容
        for (int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < colums; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText("");
                table->setItem(i, j, item);
            }
        }
    }
}

void CUserGroupDialog::SetList()
{
    QTableWidget* table = ui->tableWidget;
    //设置表格的内容
    QString txt;
    QTableWidgetItem* item = NULL;
    MU_INT32  cont_num = deqStUserInf.size();

    for (MU_INT32 i = 0; i < s_row_max; ++i)
    {
        for (MU_INT32 j = 0; j < s_colum; ++j)
        {
            txt = "";
            item = table->item(i, j);
            item->setText(txt);
        }
    }

    for (MU_INT32 i = 0; i < cont_num; ++i)
    {
        st_user_inf user_inf = deqStUserInf[i];
        int colum_ind = -1;
        int row_ind = i;

        colum_ind++;
        txt = str_2_qstr(user_inf.user_name);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(user_inf.user_descript);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(user_inf.user_pwd);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);
    }
}
