#pragma once

#include <QtWidgets/QWidget>
#include <ActiveQt/QAxBindable>

#include "ui_CaptruePlug.h"

class CaptruePlug : public QWidget, public QAxBindable
{
    Q_OBJECT

public:
    CaptruePlug(QWidget *parent = Q_NULLPTR);

private:
    Ui::CaptruePlugClass ui;
};
