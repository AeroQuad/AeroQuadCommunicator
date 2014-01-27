#ifndef PANEL_TEST_H
#define PANEL_TEST_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class PanelTest;
}

class PanelTest : public QWidget
{
    Q_OBJECT
    
public:
    explicit PanelTest(QWidget *parent = 0);
    ~PanelTest();
    void initPanel(QStandardItemModel dataModel);
    
private:
    Ui::PanelTest *ui;
};

#endif // PANEL_TEST_H
