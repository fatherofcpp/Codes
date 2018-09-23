#include "keyboard.h"
#include "ui_keyboard.h"
#include "CommonFunction.h"

CKeyBoard::CKeyBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CKeyBoard)
{
    ui->setupUi(this);

    setWindowTitle("输入法");

    keyNum = KEYNUM;
    chineseNumShow = 16 - 2;
    startIndShow = 0;
    isEnglishInput = true;
    vecWidget.clear();
    vecChinese.clear();
    btArrKey = new CMyQPushButton[keyNum];
    btArrChinese = new CMyQPushButton[chineseNumShow + 2];
    lineInput = new QLineEdit(this);

    labInput = new QLabel(this);
    btFinishInput = new QPushButton(this);
    btDeleteInput = new QPushButton(this);
    btChooseInput = new QPushButton(this);
    labPinyin = new QLabel(this);
    linePinyin = new QLineEdit(this);
    btDeletePinyin = new QPushButton(this);
    pCSqlite = new CSqlite;

    labInput->setFixedWidth(40);
    labInput->setText("全文");
    lineInput->setFixedWidth(160);
    btFinishInput->setText("完成");
    btDeleteInput->setText("退格");
    btChooseInput->setText("英文/中文");

    labPinyin->setFixedWidth(40);
    labPinyin->setText("拼音");
    linePinyin->setFixedWidth(160);
    btDeletePinyin->setText("退格");
    btDeletePinyin->setEnabled(false);

    connect(btFinishInput, SIGNAL(clicked()),
            this, SLOT(slot_bt_finish_input()));
    connect(btDeleteInput, SIGNAL(clicked()),
            this, SLOT(slot_bt_delete_input()));
    connect(btDeletePinyin, SIGNAL(clicked()),
            this, SLOT(slot_bt_delete_pinyin()));
    connect(btChooseInput, SIGNAL(clicked()),
            this, SLOT(slot_bt_choose_input()));
    connect(linePinyin, SIGNAL(textChanged(QString)),
            this, SLOT(slot_line_pinyin_text_change(QString)));


    for(MU_INT32 i = 0; i < chineseNumShow + 2; i++)
    {
        int w = 35;
        int h = 35;
        btArrChinese[i].setText("");
        btArrChinese[i].setFixedSize(w, h);
        connect(&btArrChinese[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_bt_key_clicked(const st_bt_inf&)));
    }

    for(MU_INT32 i = 0; i < keyNum; i++)
    {
        int w = 35;
        int h = 35;
        btArrKey[i].setText(gKeyboardVal[i]);
        btArrKey[i].setFixedSize(w, h);
        connect(&btArrKey[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_bt_key_clicked(const st_bt_inf&)));
    }

    int spacing = 1;
    //label
    QHBoxLayout* layout_input = new QHBoxLayout();
    vecWidget.push_back(layout_input);
    layout_input->setSpacing(spacing);
    layout_input->addStretch();
    layout_input->addWidget(labInput);
    layout_input->addWidget(lineInput);
    layout_input->addWidget(btDeleteInput);
    layout_input->addWidget(btFinishInput);
    layout_input->addStretch();

    //pinyin
    QHBoxLayout* layout_pinyin = new QHBoxLayout();
    vecWidget.push_back(layout_pinyin);
    layout_pinyin->setSpacing(spacing);
    layout_pinyin->addStretch();
    layout_pinyin->addWidget(labPinyin);
    layout_pinyin->addWidget(linePinyin);
    layout_pinyin->addWidget(btDeletePinyin);
    layout_pinyin->addWidget(btChooseInput);
    layout_pinyin->addStretch();

    //chinese show
    QHBoxLayout* layout_chinese_show = new QHBoxLayout();
    vecWidget.push_back(layout_chinese_show);
    layout_chinese_show->setSpacing(1);
    layout_chinese_show->addStretch();
    for(MU_INT32 i = 0; i < chineseNumShow + 2; i++)
    {
        layout_chinese_show->addWidget(&btArrChinese[i]);
    }
    layout_chinese_show->addStretch();

    MU_INT32 columns = 16;
    MU_INT32 rows = 0;
    MU_INT32 remain = keyNum % columns;
    if(remain == 0)
    {
        rows = keyNum / columns;
    }
    else
    {
        rows = keyNum / columns + 1;
    }
    //key
    for(MU_INT32 i = 0; i < rows; i ++)
    {
        QHBoxLayout* layout_h = new QHBoxLayout();//shuipin
        vecWidget.push_back(layout_h);
        layout_h->setSpacing(1);
        layout_h->addStretch();
        for(MU_INT32 j = 0; j < columns; j++)
        {
            MU_INT32 bt_arr_ind = j + columns * i;
            if(bt_arr_ind < keyNum)
            {
                layout_h->addWidget(&btArrKey[bt_arr_ind]);
            }
        }
        layout_h->addStretch();
    }

    //layout
    QVBoxLayout *layout_v = new QVBoxLayout();//shuizhi
    layout_v->addStretch();
    for(unsigned int i = 0; i < vecWidget.size(); i ++)
    {
        layout_v->addLayout((QHBoxLayout*)vecWidget[i]);
    }
    layout_v->addStretch();

    setLayout(layout_v);

    //800*480
    setGeometry(120, 80, 600, 350);
}

CKeyBoard::~CKeyBoard()
{
    delete ui;

    delete[] btArrKey;
    delete[] btArrChinese;
    delete pCSqlite;
}

void CKeyBoard::slot_bt_key_clicked(const st_bt_inf& bt_inf)
{
    static int enter_cnt = 0;
    //qDebug("[%s] enter_cnt=%d, text=%s", __FUNCTION__, enter_cnt++, text.data());

    QString text = bt_inf.text;
    int pos = val_2_pos(text);
    if(pos < 0)
    {
        //chinese show
        if("<-" == text)
        {
            //pre
            startIndShow -= chineseNumShow;
            ChineseShow();
        }
        else if("->" == text)
        {
            //next
            startIndShow += chineseNumShow;
            ChineseShow();
        }
        else
        {
            lineInput->insert(text);
            emit signal_character_selected(text);
        }
    }
    else
    {
        //english char
        if(isEnglishInput)
        {
            //english input
            lineInput->insert(text);
            emit signal_character_selected(text);
        }
        else
        {
            //pinyin input
            linePinyin->insert(text);
        }


    }

}

void CKeyBoard::slot_bt_finish_input()
{
    QString text = lineInput->text();
    emit signal_edit_finished(text);
}

static int callback_chinese(void* param_cb ,int n_count,char** pp_value,char** pp_name)
{
    string s;
    for(int i = 0; i < n_count; i++)
    {
        s += pp_name[i];
        s += ":";
        s += pp_value[i];
        s += ";";
    }
    //TRACE("[%s][%s]\n", __FUNCTION__, s.c_str());

    int chinese_ind = 2;
    vector<string>* p_vec_chinese = (vector<string>*)param_cb;
    string chinese;
    chinese = pp_value[chinese_ind];
    p_vec_chinese->push_back(chinese);
    /*
    [id:0;py:a;word:安;sort:0;]
    [id:1;py:a;word:按;sort:0;]
    */
    return 0;
}

void CKeyBoard::slot_bt_delete_input()
{
    QString str = lineInput->text();
    str = str.left(str.length() - 1);
    lineInput->setText(str);
    emit signal_edit_delete();
}

void CKeyBoard::slot_bt_to_chinese()
{
    string sql_name;
    sql_name = gCurrentDir + "/zhpy_table.db";
    //qDebug("[%s] db_path=%s", __FUNCTION__, sql_name.c_str());
    pCSqlite->SetSqlName(sql_name.c_str());
    pCSqlite->SetTableName("zhcn");
    int ret = pCSqlite->sql_open();
    if(ret != 0)
    {
        return;
    }

    QString qkey = linePinyin->text();
    string key = string((const char *)qkey.toLocal8Bit());
    string sql_cmd = "select * from zhcn where py = \""+ key +"\"";
    vecChinese.clear();
    ret = pCSqlite->sql_exe(sql_cmd.c_str(), callback_chinese, &vecChinese);
    if (ret != 0)
    {
        return;
    }

    for(unsigned int i = 0; i < vecChinese.size(); i ++)
    {
        //    TRACE("[%s][%s]\n", __FUNCTION__, vecChinese[i].c_str());
    }
    //TRACE("[%s][vecChinese.size=%d]\n", __FUNCTION__, vecChinese.size());

    startIndShow = 0;
    ChineseShow();
    return;
}

void CKeyBoard::slot_bt_delete_pinyin()
{
    QString str = linePinyin->text();
    str = str.left(str.length() - 1);
    linePinyin->setText(str);
}

void CKeyBoard::slot_bt_choose_input()
{
    bool enable_pinyin = false;
    QString str_aft = "";
    QString str = btChooseInput->text();
    if("英文/中文" == str)
    {
        enable_pinyin = true;
        str_aft = "中文/英文";
    }
    else
    {
        enable_pinyin = false;
        str_aft = "英文/中文";
    }
    btChooseInput->setText(str_aft);
    btDeletePinyin->setEnabled(enable_pinyin);
    isEnglishInput = !enable_pinyin;
}

void CKeyBoard::slot_line_pinyin_text_change(QString str)
{
    slot_bt_to_chinese();
}

void CKeyBoard::SetLine(QString val, QString lab)
{
    labInput->setText(lab);
    lineInput->setText(val);
}

void CKeyBoard::SetValid(MU_INT32 type)
{
    if(type == E_VALID_INT)
    {
        QRegExp reg_exp("^(-?[0]|-?[1-9][0-9]{0,12})");
        QRegExpValidator* p_QReg = new QRegExpValidator(reg_exp, lineInput);
        lineInput->setValidator(p_QReg);
    }
    else if(type == E_VALID_FLOAT)
    {
        QRegExp reg_exp("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,1})?$|(^\\t?$)");
        QRegExpValidator* p_QReg = new QRegExpValidator(reg_exp, lineInput);
        lineInput->setValidator(p_QReg);
    }
    else if(type == E_VALID_ALL_TYPE)
    {

    }
}

void CKeyBoard::SetShowText(QLineEdit::EchoMode mode)
{
    if(lineInput)
    {
        lineInput->setEchoMode(mode);
    }
}

void CKeyBoard::SetFont(QFont *p_QFont)
{
    if(lineInput)
    {
        lineInput->setFont(*p_QFont);
    }
}

void CKeyBoard::ChineseShow()
{
    MU_INT32 num_chinese = vecChinese.size();

    int start_ind = startIndShow;
    MU_INT32 end_ind = 0;
    MU_INT32 remain = num_chinese - start_ind;
    if(remain <= 0 || start_ind < 0)
    {
        return;
    }

    btArrChinese[0].setText("<-");
    btArrChinese[chineseNumShow + 1].setText("->");
    for(MU_INT32 i = 0 + 1; i < chineseNumShow + 1; i ++)
    {
        btArrChinese[i].setText("");
    }

    end_ind = start_ind + chineseNumShow;
    if(end_ind >= num_chinese)
    {
        end_ind = num_chinese;
    }


    for(MU_INT32 i = start_ind; i < end_ind; i ++)
    {
        QString str = vecChinese[i].c_str();
        btArrChinese[i - start_ind + 1].setText(str);
    }

}
