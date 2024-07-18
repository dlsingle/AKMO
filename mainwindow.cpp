#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setMinimumSize(317,257);


    for (int i = 0; i < 7 ; i++){

        QCheckBox *enableCheckBox = new QCheckBox();
        ui->tableWidget_2->setCellWidget(i, 1 ,enableCheckBox);
    }


        for (int i = 0; i < 15; i++) {
            QComboBox *modeComboBox = new QComboBox();
            modeComboBox->addItems(QStringList() << "Input" << "Output");
            ui->tableWidget->setCellWidget(i, 0, modeComboBox);

            QComboBox *pullComboBox = new QComboBox();
            pullComboBox->addItems(QStringList() << "Pull-up" << "Pull-down" << "No pull");
            ui->tableWidget->setCellWidget(i, 1, pullComboBox);
        }



}

MainWindow::~MainWindow()
{
    delete ui;
}
