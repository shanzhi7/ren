#include "healthbar.h"

HealthBar::HealthBar(QWidget *parent)
    : QWidget{parent},maxValue(1000),curValue(1000)
{

}

void HealthBar::setValue(int value)
{
    if(this->curValue != value)
    {
        curValue = qBound(0,value,maxValue);    //设置血量最大最小值
        this->curValue = value;
        emit valueChanged(this->curValue);
        update();                               //触发重绘
    }
}

void HealthBar::setMaxValue(int maxValue)
{
    this->maxValue = maxValue;
    if(curValue > maxValue)
        curValue = maxValue;
    update();

}

int HealthBar::value()
{
    return this->curValue;
}

void HealthBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);          //启用抗锯齿

    //绘制背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0,150));                    //半透明黑色画刷
    painter.drawRoundedRect(rect(),3,3);

    //绘制血条
    int barWidth = static_cast<int>(width() * curValue / maxValue);
    QLinearGradient gradient(0,0,barWidth,0);
    gradient.setColorAt(1, QColor(0, 255, 0));              //绿色
    gradient.setColorAt(0.5, QColor(255, 255, 0));          //黄色
    gradient.setColorAt(0, QColor(255, 0, 0));              //红色

    painter.setBrush(gradient);
    painter.drawRoundedRect(5, 5, barWidth - 10, height() - 10, 2, 2);

    // 绘制边框
    painter.setPen(QPen(QColor(150, 150, 150), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect(), 3, 3);

    // 绘制血量文本
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setWeight(QFont::Bold);
    font.setFamily("Microsoft YaHei");
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter,
                     QString("%1/%2").arg(curValue = qBound(0,curValue,maxValue)).arg(maxValue));
}
