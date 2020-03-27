#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    whiteImage = new QImage;
    whiteImage->load(":/img/white.png");
    ui->paintBoard->setPixmap(QPixmap::fromImage(*whiteImage));

    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshButtonClicked()));
    connect(ui->setNumButton, SIGNAL(clicked()), this, SLOT(setDotsNumberButtonClicked()));

    this->setWindowTitle("Nearest dots");
    this->setFixedSize(1200, 800);
    numDots = 500;
    maxNumDots = 1000100;
    dotRadius = 4;
    dots = nullptr;
    dcTemp = nullptr;
    targetAX = -1;
    targetAY = -1;
    targetBX = -1;
    targetBY = -1;
    paintDots();
    quick_sort(dots, 0, numDots -1);
    findDots();
    ui->numDots->setNum(numDots);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete whiteImage;
    if (dots)
        delete[] dots;
    if (dcTemp)
        delete[] dcTemp;
}

void MainWindow::paintDots()
{
    whiteImage->load(":/img/white.png");
    QPainter painter(whiteImage);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    if (dots)
        delete[] dots;
    dots = new Circle[maxNumDots];
    int xDistance = ui->paintBoard->width();
    int yDistance = ui->paintBoard->height();
    if (numDots > 1000)
    {
        int tmp = numDots;
        int k = 1;
        while (tmp > 0)
        {
            tmp = tmp >> 1;
            k++;
        }
        if (numDots >= 10000)
            k = k << 1;
        xDistance = xDistance * k;
        yDistance = ui->paintBoard->height() * k;
    }
    int doubleX = xDistance * 2;
    int doubleY = yDistance * 2;
    int i = 0;
    srand((unsigned int)(time(nullptr)));
    while (i < numDots && i < 10)
    {
        int x = rand() % xDistance;
        int y = rand() % yDistance;
        dots[i].x = x;
        dots[i].y = y;
        painter.drawEllipse(dots[i].x, dots[i].y, dotRadius, dotRadius);
        i++;
    }
    while (i < numDots)
    {
        int x = rand() % doubleX - xDistance;
        int y = rand() % doubleY - yDistance;
        dots[i].x = x;
        dots[i].y = y;
        painter.drawEllipse(dots[i].x, dots[i].y, dotRadius, dotRadius);
        i++;
    }
    ui->paintBoard->setPixmap(QPixmap::fromImage(*whiteImage));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = event->pos();
        if (p.x() > ui->paintBoard->pos().x() + ui->paintBoard->width() ||
                p.y() > ui->paintBoard->pos().y() + ui->paintBoard->height())
            return;
        int x = p.x() - ui->paintBoard->pos().x();
        int y = p.y() - ui->paintBoard->pos().y();
        addDot(x, y);
        findDots();
    }
}

void MainWindow::findDots()
{
    QImage tmp(ui->paintBoard->pixmap()->toImage());
    QPainter painter(&tmp);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(targetAX, targetAY, dotRadius, dotRadius);
    painter.drawEllipse(targetBX, targetBY, dotRadius, dotRadius);

    ui->state->setText("状态：计算中......");
    // Test two algorithms
    clock_t start_sec;
    clock_t end_sec;
    start_sec = clock();
    NaiveAlgorithm();
    end_sec = clock();
    naiveUsingTime = end_sec - start_sec;

    painter.setPen(QColor(Qt::red));
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(targetAX, targetAY, dotRadius, dotRadius);
    painter.drawEllipse(targetBX, targetBY, dotRadius, dotRadius);
    ui->paintBoard->setPixmap(QPixmap::fromImage(tmp));

    QString targetPos = "(" + QString::number(targetAX) + ", " + QString::number(targetAY) + "), ("
            + QString::number(targetBX) + ", " + QString::number(targetBY) + ")";
    ui->targetPos->setText(targetPos);
    ui->minDistance->setNum(targetDistance);

    start_sec = clock();
    dcAlgorithm();
    end_sec = clock();
    dcUsingTime = end_sec - start_sec;
    ui->state->setText("状态：计算完成！");

    // Show result
    QString nt = QString::number(naiveUsingTime) + " ms";
    ui->naiveTime->setText(nt);
    QString dt = QString::number(dcUsingTime) + " ms";
    ui->dcTime->setText(dt);
}

void MainWindow::addDot(int x, int y)
{
    if (numDots >= maxNumDots)
        return;

    dots[numDots].x = x;
    dots[numDots].y = y;
    QString inputPos = "(" + QString::number(dots[numDots].x) + ", " + QString::number(dots[numDots].y) + ")";
    ui->inputPos->setText(inputPos);

    QImage tmp(ui->paintBoard->pixmap()->toImage());
    QPainter painter(&tmp);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(dots[numDots].x, dots[numDots].y, dotRadius, dotRadius);
    numDots++;

    ui->paintBoard->setPixmap(QPixmap::fromImage(tmp));
    ui->numDots->setNum(numDots);
}

void MainWindow::refreshButtonClicked()
{
    paintDots();
    findDots();
}

void MainWindow::setDotsNumberButtonClicked()
{
    QString inputNumber = ui->inputNumber->text();
    QByteArray ba = inputNumber.toLatin1();
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;
    if (*s)
        return;
    if (inputNumber == "")
        return;
    int num = inputNumber.toInt();
    if (num > maxNumDots)
    {
        return;
    }
    numDots = num;
    paintDots();
    ui->numDots->setNum(numDots);
    findDots();
}

void MainWindow::NaiveAlgorithm()
{
    int targetAIndex = 0, targetBIndex = 0;
    targetDistance = 2147483647;
    double dis2 = 0.0;
    int ax, ay, bx, by;
    for (int i = 0; i < numDots - 1; i++)
    {
        ax = dots[i].x;
        ay = dots[i].y;
        for (int j = i + 1; j < numDots; j++)
        {
            bx = dots[j].x;
            by = dots[j].y;
            dis2 = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
            if (dis2 <= 0.1)
                continue;
            if (dis2 < targetDistance)
            {
                targetDistance = dis2;
                targetAIndex = i;
                targetBIndex = j;
            }
        }
    }
    targetDistance = sqrt(targetDistance);
    targetAX = dots[targetAIndex].x;
    targetAY = dots[targetAIndex].y;
    targetBX = dots[targetBIndex].x;
    targetBY = dots[targetBIndex].y;
}

void MainWindow::quick_sort(Circle s[], int l, int r)
{
    if (l < r)
    {
        int i = l, j = r;
        Circle x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x)
                j--;
            if(i < j)
                s[i++] = s[j];

            while(i < j && s[i] < x)
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quick_sort(s, l, i - 1);
        quick_sort(s, i + 1, r);
    }
}

double MainWindow::devide(int left, int right)
{
    double d = 1<<30;
    if(left == right)
        return d;
    if(left+1 == right)
        return sqrt((dots[left].x - dots[right].x) * (dots[left].x - dots[right].x)
                    + (dots[left].y - dots[right].y) * (dots[left].y - dots[right].y));
    int mid = (left + right) / 2;
    double d1 = devide(left, mid);
    double d2 = devide(mid+1, right);
    d = d1 <= d2 ? d1 : d2;
    int k = 0;
    for (int i = left; i <= right; i++)
    {
        if ((dots[mid].x - dots[i].x <= d) && (dots[mid].x - dots[i].x >= -d))
            dcTemp[k++]=i;
    }
    if (k >= 2)
    {
        for (int i = 0; i < k - 1; i++)
        {
            for (int j = i + 1; j < k; j++)
            {
                if (dots[dcTemp[i]].y > dots[dcTemp[j]].y ||
                        ((dots[dcTemp[i]].y == dots[dcTemp[j]].y) && (dots[dcTemp[i]].x > dots[dcTemp[j]].x)))
                {
                    int temp = dcTemp[i];
                    dcTemp[i] = dcTemp[j];
                    dcTemp[j] = temp;
                }
            }
        }
    }

    for(int i = 0; i < k; i++)
    {
        for(int j = i+1; j < k && dots[dcTemp[j]].y-dots[dcTemp[i]].y < d; j++)
        {
            double d3 = sqrt((dots[dcTemp[i]].x - dots[dcTemp[j]].x) * (dots[dcTemp[i]].x - dots[dcTemp[j]].x)
                             + (dots[dcTemp[i]].y - dots[dcTemp[j]].y) * (dots[dcTemp[i]].y - dots[dcTemp[j]].y));
            if(d > d3)
            {
                d = d3;
            }
        }
    }
    return d;
}

void MainWindow::dcAlgorithm()
{
    if (dcTemp)
    {
        delete[] dcTemp;
    }
    dcTemp = new int[5000];
    quick_sort(dots, 0, numDots -1);
    targetDistance = devide(0, numDots - 1);
}

bool operator>=(const Circle& c1, const Circle& c2)
{
    return ((c1.x >= c2.x) || ((c1.x == c2.x) && (c1.y >= c2.y)));
}

bool operator<(const Circle& c1, const Circle& c2)
{
    return ((c1.x < c2.x) || ((c1.x == c2.x) && (c1.y < c2.y)));
}
