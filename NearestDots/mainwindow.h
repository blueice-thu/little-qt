#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <time.h>
#include <math.h>

namespace Ui {
class MainWindow;
}

typedef struct Circle
{
    int x, y;
}Circle;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintDots();
    void addDot(int x, int y);
    void findDots();
    void NaiveAlgorithm();
    void dcAlgorithm();
    void quick_sort(Circle s[], int l, int r);
    double devide(int left, int right);

public slots:
    void refreshButtonClicked();
    void setDotsNumberButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QImage* whiteImage;
    Ui::MainWindow *ui;
    Circle* dots;
    int numDots;
    int maxNumDots;
    int dotRadius;
    int targetAX, targetAY, targetBX, targetBY;
    double targetDistance;
    int naiveUsingTime;
    int dcUsingTime;
    int* dcTemp;
};

bool operator>=(const Circle& c1, const Circle& c2);
bool operator<(const Circle& c1, const Circle& c2);

#endif // MAINWINDOW_H
