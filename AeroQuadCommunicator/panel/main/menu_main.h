#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class MenuMain;
}

class MenuMain : public QWidget
{
    Q_OBJECT
    
public:
    explicit MenuMain(QWidget *parent = 0);
    ~MenuMain();

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);

private:
    Ui::MenuMain *ui;
};

#endif // MENU_MAIN_H
