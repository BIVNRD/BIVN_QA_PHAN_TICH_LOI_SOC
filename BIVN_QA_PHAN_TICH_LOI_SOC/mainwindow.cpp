#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardItemModel>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


#include "models/paper.h"
#include "processor/processor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow){
    ui->setupUi(this);
    progressBar->setMaximumWidth(300);
    ui->statusBar->addWidget(progressBar.get());
    setWindowState(Qt::WindowMaximized);
    enumFiles();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::enumFiles(){
    Paper::scan();

    QStandardItemModel *model = (QStandardItemModel*)ui->tableView->model();
    if(model == nullptr){
        model = new QStandardItemModel(0,1);
        model->setHorizontalHeaderLabels({"Files"});
        ui->tableView->setModel(model);

    }

    model->setRowCount(Paper::items.size());
    for(int row = 0; row < Paper::items.size(); row++){
        if(model->item(row,0) == nullptr){
            model->setItem(row, 0, new QStandardItem());
        }
        model->item(row,0)->setText(Paper::items[row]->name);
    }
}

void MainWindow::handleItem(int index){
    int row = index;
    auto paper = Paper::items[row];
    if(Processor::process(paper)){
        QImage image(Processor::imageBGR.data, Processor::imageBGR.cols, Processor::imageBGR.rows, Processor::imageBGR.step, QImage::Format_BGR888);
        QPixmap pix = QPixmap::fromImage(image);
        ui->imageView->setPixmap(pix);
    }
}

void MainWindow::on_tableView_pressed(const QModelIndex &index){
    int row = index.row();
    handleItem(row);
}


void MainWindow::on_actionAnalize_triggered(){
    int rows = ui->tableView->model()->rowCount();
    progressBar->setMaximum(rows);
    progressBar->setValue(0);
    for(int row = 0; row < rows; row++){
        handleItem(row);
        progressBar->setValue(row);
        QApplication::processEvents();
    }

}

