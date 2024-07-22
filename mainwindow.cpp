#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setMinimumSize(317,257);

    connect(ui->action_3, &QAction::triggered, this, &MainWindow::on_saveAction);
    //connect(ui->action_5, &QAction::triggered, this, &MainWindow::on_loadAction);

    connectsignal();
    for (int i = 0; i < 7 ; i++){

        QCheckBox *enableCheckBox = new QCheckBox();
        connect(enableCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_checkboxstatus);
        ui->tableWidget_2->setCellWidget(i, 1 ,enableCheckBox);
    }


    for (int i = 0; i < 15; i++) {
        QComboBox *modeComboBox = new QComboBox();
        modeComboBox->addItems(QStringList() << "Input" << "Output");
        ui->tableWidget->setCellWidget(i, 0, modeComboBox);
        connect(modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_GPIO);

        QComboBox *pullComboBox = new QComboBox();
        pullComboBox->addItems(QStringList() << "Pull-up" << "Pull-down" << "No pull");
        ui->tableWidget->setCellWidget(i, 1, pullComboBox);

        connect(pullComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_GPIO);
    }

}

void MainWindow::connectsignal()
{
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_family);
    connect(ui->comboBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_family);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_family);
    connect(ui->spinBox_2, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_family);
}

void MainWindow::on_family()
{
    console(">Family configuration changed");
}

void MainWindow::on_checkboxstatus()
{
    console(">Interface configuration changed");
}

void MainWindow::console(const QString &message)
{
    ui->plainTextEdit->appendPlainText(message);
}

void MainWindow::on_GPIO()
{
    console(">Ports table reconstructed");
}

void MainWindow::on_saveAction()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Configuration", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    savefile(filePath);
}
void MainWindow::savefile(const QString &filePath)
{
    QJsonObject interfaces;
        for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
            QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget_2->cellWidget(row, 1));
            interfaces[ui->tableWidget_2->item(row, 0)->text()] = checkBox->isChecked();
        }

        QJsonArray gpioArray;
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QJsonObject gpioObject;
            QComboBox *modeComboBox = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row, 0));
            QComboBox *pullComboBox = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(row, 1));
            gpioObject["Mode"] = modeComboBox->currentText();
            gpioObject["Pull"] = pullComboBox->currentText();
            gpioArray.append(gpioObject);
        }

        QJsonObject config;
        QJsonArray orderedConfig;

        QJsonObject seriesObj;
        seriesObj["Серия устройства"] = ui->comboBox->currentText();
        orderedConfig.append(seriesObj);

        QJsonObject modelObj;
        modelObj["Модель устройства"] = ui->comboBox_2->currentText();
        orderedConfig.append(modelObj);

        QJsonObject coreFreqObj;
        coreFreqObj["Частота тактирования ядра"] = ui->spinBox->value();
        orderedConfig.append(coreFreqObj);

        QJsonObject peripheralFreqObj;
        peripheralFreqObj["Частота тактирования переферейных устройств"] = ui->spinBox_2->value();
        orderedConfig.append(peripheralFreqObj);

        QJsonObject interfacesObj;
        interfacesObj["Задействованные интерфейсы"] = interfaces;
        orderedConfig.append(interfacesObj);

        QJsonObject gpioObj;
        gpioObj["Режим работы GPIO портов"] = gpioArray;
        orderedConfig.append(gpioObj);

        config["config"] = orderedConfig;

        QJsonDocument doc(config);
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Error", "Failed to open file for writing");
            return;
        }
        file.write(doc.toJson());
        file.close();

        console(">Configuration saved to: " + filePath);
}



MainWindow::~MainWindow()
{
    delete ui;
}
