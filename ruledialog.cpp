#include "ruledialog.h"
#include "ui_ruledialog.h"
#include <QFontDatabase>
#include <QPainterPath>

RuleDialog::RuleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RuleDialog)
{
    ui->setupUi(this);

    //设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(500, 400); //固定窗口大小

    //创建文本显示区域
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(30, 50, 440, 300); //位置和大小
    textEdit->setFrameStyle(QFrame::NoFrame); //无边框
    textEdit->setReadOnly(true); // 只读

    // 设置文本样式
    QFont font = QFont("PingFang SC", 12); //字体
    textEdit->setFont(font);
    textEdit->setStyleSheet(
        "QTextEdit {"
        "   background: transparent;"
        "   color: #333333;"
        "   padding: 10px;"
        "   border-radius: 8px;"
        "}"
        );

    //创建关闭按钮
    closeButton = new QPushButton("×", this);
    closeButton->setGeometry(460, 10, 30, 30);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ff5f57;"
        "   color: white;"
        "   border-radius: 15px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff3b30;"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &RuleDialog::close);

    //设置默认文本
    setRuleText("游戏规则说明：\n\n");
}

RuleDialog::~RuleDialog()
{
    delete ui;
}

void RuleDialog::setRuleText(const QString &text)
{
    if (textEdit)
    {
        textEdit->setText(text);
        //设置文本格式（居中显示）
        textEdit->setAlignment(Qt::AlignCenter);
    }
}

void RuleDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //创建圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(10, 10, -10, -10), 15, 15);

    //绘制阴影
    QPainterPath shadowPath;
    shadowPath.addRoundedRect(rect().adjusted(5, 5, -5, -5), 20, 20);
    painter.fillPath(shadowPath, QColor(0, 0, 0, 30));

    //绘制背景（渐变）
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(250, 250, 255));
    gradient.setColorAt(1, QColor(230, 230, 245));

    painter.fillPath(path, gradient);

    //绘制边框
    painter.setPen(QPen(QColor(200, 200, 210), 1));
    painter.drawPath(path);

    //绘制标题栏
    QLinearGradient headerGradient(0, 0, 0, 40);
    headerGradient.setColorAt(0, QColor(240, 240, 245));
    headerGradient.setColorAt(1, QColor(220, 220, 230));

    painter.fillRect(20, 20, width() - 40, 30, headerGradient);

    //绘制标题文本
    painter.setPen(QColor(80, 80, 100));
    QFont titleFont("PingFang SC", 12, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "游戏规则");
}
