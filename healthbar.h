#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QPropertyAnimation>

class HealthBar : public QWidget
{
    Q_OBJECT

    //声明value属性，让QPropertyAnimation 访问
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit HealthBar(QWidget *parent = nullptr);

    void setValue(int value);                           //设置血量值
    void setMaxValue(int maxValue);                     //设置最大血量值

    int value();                                        //获取当前值

protected:
    void paintEvent(QPaintEvent* event) override;
private:
    int maxValue;
    int curValue;
signals:
    void valueChanged(int value);                       //属性幻化信号
};

#endif // HEALTHBAR_H
