#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dealexpression.h"
#include <QMainWindow>
#include <QSignalMapper>
#include <QClipboard>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    char* result;

public slots:
    void showResult(char* result);
    void initWindow();
    void copyResult();
};

#endif // MAINWINDOW_H
