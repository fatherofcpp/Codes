#include "cinitdialog.h"
#include "ui_cinitdialog.h"

#include "CommonFunction.h"

CInitDialog::CInitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInitDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));

    InitUi();

}

CInitDialog::~CInitDialog()
{
    delete ui;
}

void CInitDialog::slot_to_main_page_clicked()
{
    emit signal_is_auto_2_warn_dialog(false);
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CInitDialog::InitUi()
{
    QPalette pal;
    QFont ft;
    ui->pushButton->setStyleSheet(gResButtonBlue);

    pal.setColor(QPalette::ButtonText, Qt::white);
    ui->pushButton->setPalette(pal);
    pal.setColor(QPalette::WindowText,Qt::white);
    ui->label_main->setPalette(pal);
    ui->label_sub->setPalette(pal);

    ft.setPointSize(24);
    ui->label_main->setFont(ft);
    ft.setPointSize(12);
    ui->label_sub->setFont(ft);

    ui->label_main->setText("交流精密配电监控系统");
    ui->label_sub->setText("--苏州新宏博智能科技股份有限公司");
    ui->pushButton->setText("进入系统");

    ui->label_test->setText("2016-12-30 15:05:30");
    ui->label_test->setVisible(false);
}
