#include "celesubwaydialog.h"
#include "ui_celesubwaydialog.h"
#include <QScrollBar>
#include <QLabel>
#include "CommonFunction.h"

CEleSubWayDialog::CEleSubWayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEleSubWayDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < SUB_WAY_PARAM_NUM; j++)
        {
            eleParam[i][j] = new st_sub_way_ele_param;
            memset(eleParam[i][j], 0, sizeof(st_sub_way_ele_param));
        }
    }

    pOuterDev = NULL;

    InitUi();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_next_page, SIGNAL(clicked()),
            this, SLOT(slot_next_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));
}

CEleSubWayDialog::~CEleSubWayDialog()
{
    delete ui;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < SUB_WAY_PARAM_NUM; j++)
        {
            delete eleParam[i][j];
        }
    }
}

void CEleSubWayDialog::slot_to_main_page_clicked()
{
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

static int pageValue = 10; // 一页显示条数
void CEleSubWayDialog::slot_pre_page_clicked()
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

void CEleSubWayDialog::slot_next_page_clicked()
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

void CEleSubWayDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CEleSubWayDialog::SetUi(const st_dev_inf* p_dev_inf)
{
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
            int sub_way_ind = -1;
            int sub_way_param_ind = -1;
            int real_data_ind = -1;
            //支路
            if(way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
            {
                sub_way_ind = 0;
                sub_way_param_ind = way_id- MAIN_A_CODE - 1;
                real_data_ind = sub_way_param_ind;
            }
            else if(way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
            {
                sub_way_ind = 1;
                sub_way_param_ind = way_id - MAIN_B_CODE - 1;
                real_data_ind = sub_way_param_ind + MAIN_B_OFF_ELECTRIC;
            }

            if(sub_way_ind >= 0)
            {
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                eleParam[sub_way_ind][sub_way_param_ind]->current
                        = p_ac_meter->i_out_line[real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->power[0]
                        = p_ac_meter->power_out_line[0][real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->power[1]
                        = p_ac_meter->power_out_line[1][real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->power[2]
                        = p_ac_meter->power_out_line[2][real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->power_factor
                        = p_ac_meter->power_factor_out_line[real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->energy[0]
                        = p_ac_meter->energy_out_line[0][real_data_ind];
                eleParam[sub_way_ind][sub_way_param_ind]->energy[1]
                        = p_ac_meter->energy_out_line[1][real_data_ind];
                SetList(sub_way_ind);
            }
        }
    }
}

void CEleSubWayDialog::InitUi()
{
    //bt
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

        int rows = SUB_WAY_PARAM_NUM * 2;
        int colums = 7;

        table->setRowCount(rows);
        table->setColumnCount(colums);

        //设置左上角文字
        QLabel*label = new QLabel(table);
        label->setText("编号");
        label->move(5,5);
        label->installEventFilter(this);

        //设置表格行标题
        QStringList header_labels;
        header_labels.append("电流A");
        header_labels.append("有功功率kW");
        header_labels.append("无功功率kvar");
        header_labels.append("视在功率kVA");
        header_labels.append("功率因数");
        header_labels.append("有功电能kWh");
        header_labels.append("无功电能kvarh");
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
            int width = 50;
            if(i == 0)
            {
                width = 70;
            }
            else if(i == 3)
            {
                width = 110;
            }
            else if(i == 4)
            {
                width = 80;
            }
            else if(i == 6 || i == 5 || i == 2)
            {
                width = 120;
            }            
            else
            {
                width = 100;
            }
            table->setColumnWidth(i, width);
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
            if(i < SUB_WAY_PARAM_NUM)
            {
                str = QString("A") + QString::number(i + 1);
            }
            else
            {
                str = QString("B") + QString::number(i - SUB_WAY_PARAM_NUM + 1);
            }
            row_labels.append(str);
        }
        table->setVerticalHeaderLabels(row_labels);

        //创建表格
        for (int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < colums; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem;
                QString txt = QString::number(10 * i + j);
                item->setText(txt);
                table->setItem(i, j, item);
            }
        }
        //设置表格内容
        for(int i = 0; i < E_SUB_WAY_NUM; i++)
        {
            SetList(i);
        }
    }

}

void CEleSubWayDialog::SetList(int way_ind)
{
    QTableWidget* table = ui->tableWidget;
    //设置表格的内容
    QString txt;
    QTableWidgetItem* item = NULL;
    for (int i = 0; i < SUB_WAY_PARAM_NUM; ++i)
    {
        int colum_ind = -1;
        int row_ind = i + way_ind * SUB_WAY_PARAM_NUM;

        colum_ind++;
        txt = Uint2DoubleQStr(eleParam[way_ind][i]->current, gTwoUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Sint2DoubleQStr(eleParam[way_ind][i]->power[0], gTwoUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Sint2DoubleQStr(eleParam[way_ind][i]->power[1], gTwoUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Sint2DoubleQStr(eleParam[way_ind][i]->power[2], gTwoUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Sint2DoubleQStr(eleParam[way_ind][i]->power_factor, gTwoUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Uint2DoubleQStr(eleParam[way_ind][i]->energy[0] * 10, gThreeUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = Uint2DoubleQStr(eleParam[way_ind][i]->energy[1] * 10, gThreeUnit);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);
    }
}
