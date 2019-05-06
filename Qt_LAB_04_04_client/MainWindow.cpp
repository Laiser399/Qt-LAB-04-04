#include "MainWindow.h"
#include <QDebug>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>

//-------------|
//   private   |
//-------------|
void MainWindow::setWidgets() {
    QWidget *w = new QWidget;
    QGridLayout *mainLay = new QGridLayout;
        QHBoxLayout *layInputColumn = new QHBoxLayout;
            QLabel *pLabel1 = new QLabel("Номер столбца:");
            editColumn = new QLineEdit;
        QPushButton *buttonRemoveRow = new QPushButton("Удалить строку");
        QPushButton *buttonAppendRow = new QPushButton("Добавить строку");
        QPushButton *buttonRemoveColumn = new QPushButton("Удалить столбец");
        QPushButton *buttonAppendColumn = new QPushButton("Добавить столбец");
        QTabWidget *tabMatrix = new QTabWidget;
            QTableView *mtxViewIn = new QTableView;
                mtxModelIn = new MatrixTableModel;
            QTableView *mtxViewOut = new QTableView;
                mtxModelOut = new MatrixTableModel;
        QPushButton *buttonExec = new QPushButton("Выполнить");



    setCentralWidget(w);
    w->setLayout(mainLay);
        mainLay->addLayout(layInputColumn, 0, 0, 1, 4);
            layInputColumn->addWidget(pLabel1);
            layInputColumn->addWidget(editColumn);
        mainLay->addWidget(buttonRemoveRow, 1, 0);
        mainLay->addWidget(buttonAppendRow, 1, 1);
        mainLay->addWidget(buttonRemoveColumn, 1, 2);
        mainLay->addWidget(buttonAppendColumn, 1, 3);
        mainLay->addWidget(tabMatrix, 2, 0, 1, 4);
            tabMatrix->addTab(mtxViewIn, "Входная матрица");
                mtxViewIn->setModel(mtxModelIn);
            tabMatrix->addTab(mtxViewOut, "Результат");
                mtxViewOut->setModel(mtxModelOut);
        mainLay->addWidget(buttonExec, 3, 0, 1, 4, Qt::AlignCenter);

    mtxViewOut->setEditTriggers(QTableView::NoEditTriggers);
    buttonExec->setMaximumWidth(120);



    connect(buttonRemoveRow, SIGNAL(pressed()), mtxModelIn, SLOT(slot_removeLastRow()));
    connect(buttonAppendRow, SIGNAL(pressed()), mtxModelIn, SLOT(slot_appendRow()));
    connect(buttonRemoveColumn, SIGNAL(pressed()), mtxModelIn, SLOT(slot_removeLastColumn()));
    connect(buttonAppendColumn, SIGNAL(pressed()), mtxModelIn, SLOT(slot_appendColumn()));

    connect(buttonExec, SIGNAL(pressed()), this, SLOT(slot_exec()));
//    Matrix mtx;
//    mtx.readFromStr("23.4 0 3 ||0 22");
//    mtxModel->setMatrix(mtx);
}

void MainWindow::setMenu() {
    QMenu *mFile = new QMenu("&Файл");
    mFile->addAction("&Открыть матрицу", this, SLOT(slot_openMatrix()));
    mFile->addSeparator();
    mFile->addAction("&Выйти", this, SLOT(close()));

    QMenu *mInfo = new QMenu("&?");
    mInfo->addAction("&Автор",this, SLOT(slot_author()));


    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mInfo);
}

//-------------------|
//   private slots   |
//-------------------|
void MainWindow::slot_openMatrix() {
    QString filename = QFileDialog::getOpenFileName();
    if (filename.length() == 0)
        return;

    QFile fin(filename);
    if (!fin.open(QFile::ReadOnly)) {
        QMessageBox::information(this, "Ошибка", "Ошибка открытия файла!");
        return;
    }
    QTextStream in(&fin);
    QString str = in.readAll();
    fin.close();

    Matrix mtx;
    mtx.readFromStr(str);
    mtxModelIn->setMatrix(mtx);
}

void MainWindow::slot_author() {
    QMessageBox::information(this, "Автор", "Студент МАИ\nГруппы М8О-213Б-17\nСеменов Сергей");
}

void MainWindow::slot_exec() {
    bool ok;
    quint16 column = editColumn->text().toUInt(&ok);
    if (!ok || (column == 0)) {
        QMessageBox::information(this, "Ошибка", "Указан неверный столбец!");
        return;
    }
    column--;
    client->send(column, mtxModelIn->getMatrix());
}

void MainWindow::slot_receivedMatrix(const Matrix &mtx) {
    mtxModelOut->setMatrix(mtx);
}

//------------|
//   public   |
//------------|
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMenu();
    setWidgets();

    //s = new Server(2323, this);
    client = new Client("localhost", 2323, this);
    connect(client, SIGNAL(received(Matrix)), this, SLOT(slot_receivedMatrix(const Matrix &)));

    Matrix mtx;
    mtx.readFromStr("1 3 8.2||23 0 2 5");
    mtxModelIn->setMatrix(mtx);
}

MainWindow::~MainWindow()
{

}
