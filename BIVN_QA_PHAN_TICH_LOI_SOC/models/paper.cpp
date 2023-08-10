#include "paper.h"
#include <QDir>
#include <QFileInfo>

Paper::Paper(){

}

std::vector<std::shared_ptr<Paper>> Paper::items;

void Paper::scan(){
    items.clear();
//    QString dir = "E:/QA_IMGS/GRAY";
//    QString dir = "E:/QA_IMGS/20230626";
    QString dir = "D:/BIVN/BIVN_QA_PHAN_TICH_LOI_SOC/Du lieu/2023-08-10/gray";
    QFileInfoList fileInf = QDir(dir).entryInfoList({"*.bmp"});
    for(auto &it : fileInf){
        auto paper = std::make_shared<Paper>(Paper());
        items.push_back(paper);
        paper->name = it.fileName();
        paper->path = it.absoluteFilePath();
    }
}
