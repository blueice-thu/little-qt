#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("://res/RPN.ico"));
    setWindowTitle("RPN");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(initWindow()));
    connect(ui->pushButton_2, &QPushButton::clicked,
            [=]()
        {
            QByteArray ba = ui->lineEdit->text().toLatin1();
            char* expre;
            expre = ba.data();
            dealexpression* dp = new dealexpression(expre);
            result = dp->GetResult();
            //qDebug("Result: %s", result);
            showResult(result);
        });
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton_2, SIGNAL(clicked()), Qt::UniqueConnection);
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(copyResult()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::showResult(char* result)
{
    ui->label->setText(result);
}
void MainWindow::initWindow()
{
    ui->lineEdit->clear();
    ui->label->setText("The result will be shown here");
}
void MainWindow::copyResult()
{
    QString source = ui->label->text();
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    clipboard->setText(source);
}
