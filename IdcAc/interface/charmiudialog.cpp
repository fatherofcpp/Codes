#include "charmiudialog.h"
#include "ui_charmiudialog.h"

#include <QScrollBar>
#include <QLabel>
#include "CommonFunction.h"

static int s_row = 1;
static int s_colum = 30;

CHarmIUDialog::CHarmIUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CHarmIUDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;
    memset(harmPercent, 0, sizeof(harmPercent));
    pQCPBars = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);

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

CHarmIUDialog::~CHarmIUDialog()
{
    delete ui;
}

void CHarmIUDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CHarmIUDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_HARM_U_I_DIALOG_A_UA)
    {
        ind = E_HARM_PERCENT_DIALOG;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_UB)
    {
        ind = E_HARM_U_I_DIALOG_A_UA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_UC)
    {
        ind = E_HARM_U_I_DIALOG_A_UB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IA)
    {
        ind = E_HARM_U_I_DIALOG_A_UC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IB)
    {
        ind = E_HARM_U_I_DIALOG_A_IA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IC)
    {
        ind = E_HARM_U_I_DIALOG_A_IB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UA)
    {
        ind = E_HARM_U_I_DIALOG_A_IC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UB)
    {
        ind = E_HARM_U_I_DIALOG_B_UA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UC)
    {
        ind = E_HARM_U_I_DIALOG_B_UB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IA)
    {
        ind = E_HARM_U_I_DIALOG_B_UC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IB)
    {
        ind = E_HARM_U_I_DIALOG_B_IA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IC)
    {
        ind = E_HARM_U_I_DIALOG_B_IB;
    }
    emit signal_change_child_dialog(ind);
}

void CHarmIUDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_HARM_U_I_DIALOG_A_UA)
    {
        ind = E_HARM_U_I_DIALOG_A_UB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_UB)
    {
        ind = E_HARM_U_I_DIALOG_A_UC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_UC)
    {
        ind = E_HARM_U_I_DIALOG_A_IA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IA)
    {
        ind = E_HARM_U_I_DIALOG_A_IB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IB)
    {
        ind = E_HARM_U_I_DIALOG_A_IC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_A_IC)
    {
        ind = E_HARM_U_I_DIALOG_B_UA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UA)
    {
        ind = E_HARM_U_I_DIALOG_B_UB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UB)
    {
        ind = E_HARM_U_I_DIALOG_B_UC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_UC)
    {
        ind = E_HARM_U_I_DIALOG_B_IA;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IA)
    {
        ind = E_HARM_U_I_DIALOG_B_IB;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IB)
    {
        ind = E_HARM_U_I_DIALOG_B_IC;
    }
    else if(dialogInd == E_HARM_U_I_DIALOG_B_IC)
    {
        ind = E_HARM_U_I_DIALOG_B_IC;
    }
    emit signal_change_child_dialog(ind);
}

void CHarmIUDialog::InitUi()
{
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    //table
    QTableWidget* table_arr[1];
    table_arr[0] = ui->tableWidget;

    for(int table_ind = 0; table_ind < 1; table_ind++)
    {
        QTableWidget* table = table_arr[table_ind];
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        int rows = s_row;
        int colums = s_colum;

        table->setRowCount(rows);
        table->setColumnCount(colums);

        //设置左上角文字
        QLabel*label = new QLabel(table);
        label->setText("");
        label->move(5,5);
        label->installEventFilter(this);

        //设置表格行标题
        QStringList header_labels;
        for(int i = 0; i < 30; i++)
        {
            header_labels.append(QString::number(i + 2));
        }
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
            //            table->setColumnWidth(i, 50);
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
            str = QString("U");
            row_labels.append(str);
        }
        table->setVerticalHeaderLabels(row_labels);

        //设置表格的内容
        for (int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < colums; j++)
            {
                table->setColumnWidth(j, 122);
                QTableWidgetItem* item = new QTableWidgetItem;
                QString txt = Uint2DoubleQStr(harmPercent[j], gTwoUnit);
                item->setText(txt);
                table->setItem(i, j, item);
            }
        }
    }

    //list
    QCustomPlot* custom_plot = ui->widget;
    custom_plot->setStyleSheet(gResBackGroud);
    //设置坐标轴标签名称
    custom_plot->xAxis->setLabel("(次)");
    custom_plot->yAxis->setLabel("(%)");

    //设置坐标轴显示范围
    custom_plot->xAxis->setRange(2, 31);
    custom_plot->yAxis->setRange(0, 100);

    //2-31
    QVector<double> values(s_colum);
    QVector<double> index(s_colum);
    for(int i = 0; i < s_colum; ++ i)
    {
        values[i] = harmPercent[i] * gTwoUnit;
        index[i] = i + 2;
    }

    pQCPBars->setData(index, values);
    custom_plot->replot();
}

void CHarmIUDialog::SetList()
{
    QTableWidget* table = ui->tableWidget;
    //设置表格的内容
    QString txt;
    QTableWidgetItem* item = NULL;

    for (int i = 0; i < s_row; ++i)
    {
        for (int j = 0; j < s_colum; ++j)
        {
            txt = Uint2DoubleQStr(harmPercent[j], gTwoUnit);
            item = table->item(i, j);
            item->setText(txt);
        }
    }

    //list
    QCustomPlot* custom_plot = ui->widget;
    QVector<double> values(s_colum);
    QVector<double> index(s_colum);
    for(int i = 0; i < s_colum; ++ i)
    {
        values[i] = harmPercent[i] * gTwoUnit;
        index[i] = i + 2;
    }
    pQCPBars->setData(index, values);
    custom_plot->replot();
}


static int pageValue = 6; // 一页显示条数
void CHarmIUDialog::slot_pre_list_clicked()
{
    int max_value = ui->tableWidget->horizontalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->horizontalScrollBar()->value();

    if(cur_scroller > 0)
    {
        ui->tableWidget->horizontalScrollBar()->setSliderPosition(cur_scroller - pageValue);
    }
    else
    {
        ui->tableWidget->horizontalScrollBar()->setSliderPosition(max_value);
    }
}

void CHarmIUDialog::slot_next_list_clicked()
{
    int max_value = ui->tableWidget->horizontalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->horizontalScrollBar()->value(); //获得当前scroller值

    if(cur_scroller < max_value)
    {
        ui->tableWidget->horizontalScrollBar()->setSliderPosition(pageValue + cur_scroller);
    }
    else
    {
        ui->tableWidget->horizontalScrollBar()->setSliderPosition(0);
    }
}

void CHarmIUDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int m_ind = -1;
    switch(dialogInd)
    {
    case E_HARM_U_I_DIALOG_A_UA:
    case E_HARM_U_I_DIALOG_A_UB:
    case E_HARM_U_I_DIALOG_A_UC:
    case E_HARM_U_I_DIALOG_A_IA:
    case E_HARM_U_I_DIALOG_A_IB:
    case E_HARM_U_I_DIALOG_A_IC:
        way_code = MAIN_A_CODE;
        m_ind = 0;
        break;
    case E_HARM_U_I_DIALOG_B_UA:
    case E_HARM_U_I_DIALOG_B_UB:
    case E_HARM_U_I_DIALOG_B_UC:
    case E_HARM_U_I_DIALOG_B_IA:
    case E_HARM_U_I_DIALOG_B_IB:
    case E_HARM_U_I_DIALOG_B_IC:
        way_code = MAIN_B_CODE;
        m_ind = 1;
        break;
    default:
        break;
    }
    if(way_code < 0)
    {
        return;
    }

    int dev_type = p_dev_inf->dev_type;
    if(dev_type != E_DEV_ELECTRIC)
    {
        return;
    }

    for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
    {
        bool is_way_use = IsWayUsedByMask(way_ind,
                                          p_dev_inf->way_inf.way_mask);
        if(is_way_use)
        {
            const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
            int way_id = p_st_way_data->way_id;
            //0主路,1-24支路
            if(way_id == way_code)
            {
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                const st_harm_param* p_st_harm_param = &p_ac_meter->harm_param[m_ind];
                if(dialogInd == E_HARM_U_I_DIALOG_A_UA || dialogInd == E_HARM_U_I_DIALOG_B_UA)
                {
                    memcpy(harmPercent, p_st_harm_param->u_harm_percent[0], sizeof(harmPercent));
                }
                if(dialogInd == E_HARM_U_I_DIALOG_A_UB || dialogInd == E_HARM_U_I_DIALOG_B_UB)
                {
                    memcpy(harmPercent, p_st_harm_param->u_harm_percent[1], sizeof(harmPercent));
                }
                if(dialogInd == E_HARM_U_I_DIALOG_A_UC || dialogInd == E_HARM_U_I_DIALOG_B_UC)
                {
                    memcpy(harmPercent, p_st_harm_param->u_harm_percent[2], sizeof(harmPercent));
                }
                else if(dialogInd == E_HARM_U_I_DIALOG_A_IA || dialogInd == E_HARM_U_I_DIALOG_B_IA)
                {
                    memcpy(harmPercent, p_st_harm_param->i_harm_percent[0], sizeof(harmPercent));
                }
                else if(dialogInd == E_HARM_U_I_DIALOG_A_IB || dialogInd == E_HARM_U_I_DIALOG_B_IB)
                {
                    memcpy(harmPercent, p_st_harm_param->i_harm_percent[1], sizeof(harmPercent));
                }
                else if(dialogInd == E_HARM_U_I_DIALOG_A_IC || dialogInd == E_HARM_U_I_DIALOG_B_IC)
                {
                    memcpy(harmPercent, p_st_harm_param->i_harm_percent[2], sizeof(harmPercent));
                }
            }
        }
    }
    SetList();
}

void CHarmIUDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}
