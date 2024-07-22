#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void on_family();
    void on_checkboxstatus();
    void console(const QString &message);
    void connectsignal();
    void on_GPIO();
    void on_loadAction();
    void on_saveAction();
    void savefile(const QString &filePath);
    void loadfile(const QString &filePath);
};

#endif // MAINWINDOW_H
