#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QObject>
#include <QStandardItemModel>

class Panel : public QWidget
{
    Q_OBJECT

public:
    Panel();
    ~Panel();
    virtual void initialize(QStandardItemModel *dataModel) = 0;

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
};

#endif // PANEL_H
