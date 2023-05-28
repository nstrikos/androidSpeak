#include "myPushbutton.h"

MyPushButton::MyPushButton(QWidget *parent)
{
    this->setText(tr("Clear"));
}

int MyPushButton::count() const
{
    return m_count;
}

void MyPushButton::setCount(int newCount)
{
    m_count = newCount;
}

void MyPushButton::buttonClicked()
{
    emit updateCount(m_count);
}
