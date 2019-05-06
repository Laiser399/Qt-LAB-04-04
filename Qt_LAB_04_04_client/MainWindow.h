#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Matrix.h>
#include <QTableView>
#include <MatrixTableModel.h>

//#include <Server.h>
#include <Client.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void setWidgets();
    void setMenu();

    MatrixTableModel *mtxModelIn, *mtxModelOut;
    QLineEdit *editColumn;

    //Server *s;
    Client *client;
private slots:
    void slot_openMatrix();
    void slot_author();
    void slot_exec();
    void slot_receivedMatrix(const Matrix &mtx);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
