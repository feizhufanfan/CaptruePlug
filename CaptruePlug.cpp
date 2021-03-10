#include "CaptruePlug.h"

#include <ActiveQt/QAxFactory>

CaptruePlug::CaptruePlug(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

QAXFACTORY_DEFAULT(CaptruePlug,
    "{95c1ec7f-cec0-492a-83f5-e2c2d4397da2}",
    "{f33206ef-65e1-4032-9bba-11d1bf04e680}",
    "{568880b7-b7da-4f0b-861d-aaaebf4faa4c}",
    "{0d878aad-341e-42e0-9944-7f7678627563}",
    "{4dadf630-ea0c-4dfd-a75f-55cb8aeb3eab}"
)
