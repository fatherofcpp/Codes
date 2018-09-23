#include "cwarnhistorydialog.h"
#include "ui_cwarnhistorydialog.h"
#include <QScrollBar>
#include <QLabel>
#include "CommonFunction.h"
#include "Log.h"

static int s_colum = 4;
static int s_row_max = 1024 * 10;
CWarnHistoryDialog::CWarnHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CWarnHistoryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pCProgressDialog = new CProgressDialog(this);
    memset(&signalParmHisWarn.sig_parm, 0, sizeof(signalParmHisWarn.sig_parm));
    signalParmHisWarn.warn_history_get.off_set = 0;
    signalParmHisWarn.warn_history_get.deq_st_warn_history.clear();

    InitUi();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_next_page, SIGNAL(clicked()),
            this, SLOT(slot_next_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));
    connect(ui->pushButton_pre_list, SIGNAL(clicked()),
            this, SLOT(slot_pre_list_clicked()));
    connect(ui->pushButton_next_list, SIGNAL(clicked()),
            this, SLOT(slot_next_list_clicked()));
}

CWarnHistoryDialog::~CWarnHistoryDialog()
{
    delete ui;
}

void CWarnHistoryDialog::slot_to_main_page_clicked()
{
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CWarnHistoryDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    ind = E_WARN_CURRENT_DIALOG;
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    emit signal_change_child_dialog(ind);
}

void CWarnHistoryDialog::slot_next_page_clicked()
{
//    int scroller = 0;
//    ui->tableWidget->verticalScrollBar()->setSliderPosition(scroller);
//    ClearWarnInf();

//    for(int i = 0; i < 10; i++)
//    {
//        st_warn_history_inf* warn_his_old = new st_warn_history_inf;
//        warn_his_old->dev_addr = 1;
//        warn_his_old->dev_type = E_DEV_ELECTRIC;
//        warn_his_old->is_warn = 1;
//        warn_his_old->obj_name = "obj";
//        warn_his_old->start_time = "19491001121314";
//        warn_his_old->stop_time = "19591001121314";
//        warn_his_old->warn_type = "1";
//        warn_his_old->way_code = MAIN_A_CODE + 1 ;
//        signalParmHisWarn.warn_history_get.deq_st_warn_history.push_back(warn_his_old);
//    }
//    SetList(scroller);
}

static int pageValue = 10; // 一页显示条数
void CWarnHistoryDialog::slot_pre_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum();
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value();
    int af_scroller = 0;
    if(cur_scroller - pageValue >= 0)
    {
        af_scroller = cur_scroller - pageValue;
    }
    else
    {
        af_scroller = max_value;
    }
    SetScrollPosition(af_scroller);
}

void CWarnHistoryDialog::slot_next_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum(); //
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值
    int af_scroller = 0;
    if(pageValue + cur_scroller <= max_value)
    {
        af_scroller = pageValue + cur_scroller;
    }
    else
    {
        af_scroller = 0;
    }
    SetScrollPosition(af_scroller);
}


void CWarnHistoryDialog::SetScrollPosition(int scroller)
{
    ui->tableWidget->verticalScrollBar()->setSliderPosition(scroller);

    ClearWarnInf();
    signalParmHisWarn.warn_history_get.off_set = scroller;
    int ret = 0;
    emit signal_read_warn_history(&signalParmHisWarn.sig_parm,
                                  &signalParmHisWarn.warn_history_get);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_READ_WARN_HISTORY_CMD));
    ret = signalParmHisWarn.sig_parm.is_suc;
    if(ret == 0)
    {
        SetList(scroller);
    }
    messagebox(this, ret, false);
}

int CWarnHistoryDialog::GetHistoryWarn()
{
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value();
    SetScrollPosition(cur_scroller);
    return 0;
}

void CWarnHistoryDialog::InitUi()
{
    //bt
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);
    ui->pushButton_pre_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_list->setStyleSheet(gResArrowUp);
    ui->pushButton_next_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_list->setStyleSheet(gResArrowDown);

    ui->widget_warn->setStyleSheet(gResBell);

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
        label->setText("序号");
        label->move(5,5);
        label->installEventFilter(this);

        //设置表格行标题
        QStringList header_labels;
        header_labels.append("发生时间");
        header_labels.append("结束时间");
        header_labels.append("对象名");
        header_labels.append("报警类型");
        header_labels.append("报警描述");
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
            table->setColumnWidth(i, 155);
        }

        //自动调整最后一列的宽度使它和表格的右边界对齐
        table->horizontalHeader()->setStretchLastSection(true);

        //设置表格的选择方式
        table->setSelectionBehavior(QAbstractItemView::SelectItems);
        table->setSelectionMode(QAbstractItemView::NoSelection);

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

        //设置表格的内容
        for (int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < colums; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem;
                QString txt = QString::number(10 * i + j);
                item->setText("");
                table->setItem(i, j, item);
            }
        }
    }
}

void CWarnHistoryDialog::SetList(int scroller)
{
    QTableWidget* table = ui->tableWidget;
    //设置表格的内容
    QString txt;
    QTableWidgetItem* item = NULL;
    MU_INT32  warn_num = signalParmHisWarn.warn_history_get.deq_st_warn_history.size();

    for (int i = 0; i < s_row_max; ++i)
    {
        for (int j = 0; j < s_colum; ++j)
        {
            txt = "";
            item = table->item(i, j);
            item->setText(txt);
        }
    }

    for (MU_INT32 i = 0; i < warn_num; ++i)
    {
        st_warn_history_inf *p_warn_history_inf =
                signalParmHisWarn.warn_history_get.deq_st_warn_history[i];
        int colum_ind = -1;
        int row_ind = i + scroller;

        colum_ind++;
        txt = str_2_qstr(p_warn_history_inf->start_time);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(p_warn_history_inf->stop_time);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(p_warn_history_inf->obj_name);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(p_warn_history_inf->warn_type);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);
    }
}

void CWarnHistoryDialog::ClearWarnInf()
{
    for (DEQ_ST_WARN_HISTORY_INF::iterator iter = signalParmHisWarn.warn_history_get.deq_st_warn_history.begin();
         iter != signalParmHisWarn.warn_history_get.deq_st_warn_history.end(); iter++)
    {
        st_warn_history_inf* warn_his_old = *iter;
        delete warn_his_old;
    }
    signalParmHisWarn.warn_history_get.deq_st_warn_history.clear();
}

void CWarnHistoryDialog::slot_read_warn_history_finish(st_signal_parm* p_sig_parm,
                                                       st_thread_param_warn_history_get *param_warn_history_get)
{
    close_progress(pCProgressDialog);

//    static int cnt = 0;
//    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}

