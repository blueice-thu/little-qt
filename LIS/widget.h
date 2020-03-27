#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QClipboard>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void LIS();
    void copyResult();
private:
    Ui::Widget *ui;
    QString result;
};

#endif // WIDGET_H
