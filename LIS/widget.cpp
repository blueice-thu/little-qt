#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);
    this->setWindowTitle("求解最长递增序列");
    connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(LIS()));
    connect(ui->copyButton, SIGNAL(clicked()), this, SLOT(copyResult()));
}

Widget::~Widget()
{
    delete ui;
}

int BiSearch(int* nums, int len, int target) {
    int left = 0, right = len - 1;
    int mid;
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (nums[mid] > target)
            right = mid - 1;
        else if (nums[mid] < target)
            left = mid + 1;
        else
            return mid;
    }
    return left;
}

void Widget::LIS() {
    QString input = ui->textEdit->toPlainText();
    QStringList inputList = input.split(" ");
    int len = inputList.length();
    if (len <= 0)
        return;
    int* nums = new int[len]{0};
    int* prel = new int[len]{0};
    int* M = new int[len]{0};
    int* res = new int[len]{0};
    int* temp = new int[len] {0};
    for (int j = 0; j < len; j++) {
        nums[j] = inputList[j].toInt();
    }
    temp[0] = nums[0];
    prel[0] = -1;
    int lenTemp = 1;
    int i = 1;
    int insertPos = 0;
    while (i < len) {
        if (nums[i] <= temp[lenTemp - 1]) {
            insertPos = BiSearch(temp, lenTemp, nums[i]);
            temp[insertPos] = nums[i];
        }
        else if (nums[i] > temp[lenTemp - 1]) {
            insertPos = lenTemp;
            temp[lenTemp++] = nums[i];
        }
        M[insertPos] = i;
        if (insertPos > 0)
            prel[i] = M[insertPos-1];
        else
            prel[i] = -1;
        i++;
    }
    insertPos = M[lenTemp - 1];
    for (i = lenTemp - 1; i >= 0 && insertPos != -1; i--) {
        res[i] = nums[insertPos];
        insertPos = prel[insertPos];
    }
    ui->lengthLabel->setText(QString("长度：") + QString::number(lenTemp));
    result = "";
    for (int j = 0; j < lenTemp; j++) {
        result += QString::number(res[j]);
        result += " ";
    }
    ui->resultLabel->setText(result);
    delete[] nums;
    delete[] prel;
    delete[] M;
    delete[] res;
    delete[] temp;
}

void Widget::copyResult() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(result);
}
