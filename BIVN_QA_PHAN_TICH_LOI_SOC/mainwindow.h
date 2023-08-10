#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableView_pressed(const QModelIndex &index);

    void on_actionAnalize_triggered();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QProgressBar> progressBar = std::make_shared<QProgressBar>(new QProgressBar());
    void enumFiles();
    void handleItem(int index);
};
#endif // MAINWINDOW_H
