#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>


class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget *parent = nullptr);
    int count() const;
    void setCount(int newCount);

public slots:
    void buttonClicked();

signals:
    void updateCount(int i);

private:
    int m_count;
};

#endif // MYPUSHBUTTON_H
