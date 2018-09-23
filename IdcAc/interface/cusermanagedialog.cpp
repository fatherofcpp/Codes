#include "cusermanagedialog.h"
#include "ui_cusermanagedialog.h"
#include "CommonFunction.h"
#include "xmlAnalyze.h"

CUserManageDialog::CUserManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CUserManageDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;

    pStUserInf = new st_user_inf;
    memset(pStUserInf, 0, sizeof(*pStUserInf));
    pCUserInfDialog = new CUserInfDialog;
    pCUserGroupDialog = new CUserGroupDialog;
    deqStUserInf.clear();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));

    connect(ui->pushButton_modify_pwd, SIGNAL(clicked()),
            this, SLOT(slot_bt_modify_pwd_clicked()));
    connect(ui->pushButton_edit_user, SIGNAL(clicked()),
            this, SLOT(slot_bt_edit_user_clicked()));

    InitUi();
}

CUserManageDialog::~CUserManageDialog()
{
    delete ui;
    delete pCUserInfDialog;
    delete pCUserGroupDialog;
}

void CUserManageDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CUserManageDialog::slot_pre_page_clicked()
{
    emit signal_change_child_dialog(E_ClEAR_DIALOG);
}

void CUserManageDialog::slot_bt_edit_user_clicked()
{
    if(0 != strcmp(pStUserInf->user_descript, "管理员"))
    {
        QMessageBox::information(this, "操作", "无权限");
        return;
    }
    int ret = 0;
    pCUserGroupDialog->SetDeqUserInf(&deqStUserInf);
    ret = pCUserGroupDialog->exec();
    if (ret == QDialog::Accepted)
    {
        DEQ_ST_USER_INF* p_deqStUserInf_new = pCUserGroupDialog->GetDeqUserInf();
        DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
        *p_deqStUserInf = *p_deqStUserInf_new;
        deqStUserInf = *p_deqStUserInf_new;

        string user_group_file = gCurrentDir + "/config.xml";
        ret = gXmlAnalyze.SaveUserGroup(user_group_file.c_str());
        messagebox(this, ret);
    }
    else
    {
        ret = -1;
    }
}

void CUserManageDialog::slot_bt_modify_pwd_clicked()
{
    int ret = 0;
    pCUserInfDialog->SetUserInf(pStUserInf);
    pCUserInfDialog->SetUserInfType(E_PWD_MODIFY);
    ret = pCUserInfDialog->exec();
    if (ret == QDialog::Accepted)
    {
        st_user_inf* p_user_inf = pCUserInfDialog->GetUserInf();
        DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
        MU_INT32 cont_num = p_deqStUserInf->size();
        for(MU_INT32 i = 0; i < cont_num; i++)
        {
            st_user_inf* p_user_inf_cont = &((*p_deqStUserInf)[i]);
            if(0 == strcmp(p_user_inf->user_name, p_user_inf_cont->user_name))
            {
                strcpy(p_user_inf_cont->user_pwd, p_user_inf->user_pwd);
            }
        }
        slot_current_user(*p_user_inf);

        string user_group_file = gCurrentDir + "/config.xml";
        ret = gXmlAnalyze.SaveUserGroup(user_group_file.c_str());
        messagebox(this, ret);
    }
    else
    {
        ret = -1;
    }
}

void CUserManageDialog::slot_current_user(st_user_inf user_inf)
{
    SetUserInf(&user_inf);
    DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
    SetDeqUserInf(p_deqStUserInf);
}

void CUserManageDialog::SetUserInf(st_user_inf* p_st_user_inf)
{
    memcpy(pStUserInf, p_st_user_inf, sizeof(*p_st_user_inf));
}

void CUserManageDialog::SetDeqUserInf(DEQ_ST_USER_INF *p_deqStUserInf)
{
    deqStUserInf = *p_deqStUserInf;
}

void CUserManageDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CUserManageDialog::InitUi()
{
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->pushButton_edit_user->setText("编辑用户");
    ui->pushButton_modify_pwd->setText("修改密码");
}
