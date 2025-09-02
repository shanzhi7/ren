#ifndef RULEDIALOG_H
#define RULEDIALOG_H
/***********************************************************************************
*
* @file         ruledialog.h
* @brief        规则窗口类,显示游戏玩法
*
* @author       shanzhi
* @date         2025/09/02
* @history
***********************************************************************************/
#include <QDialog>
#include <QPainter>
#include <QLinearGradient>
#include <QGraphicsDropShadowEffect>
#include <QTextEdit>
#include <QPushButton>

namespace Ui {
class RuleDialog;
}

class RuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuleDialog(QWidget *parent = nullptr);
    ~RuleDialog();

    //设置规则文本
    void setRuleText(const QString &text);

protected:
    //自定义绘制
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::RuleDialog *ui;
    QTextEdit *textEdit; //用于显示规则文本
    QPushButton *closeButton;
};

#endif //RULEDIALOG_H
