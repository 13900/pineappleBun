#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <iostream>
#include <QFileDialog>
#include <QTextCodec>
#include <QTimer>
#include "myserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


private slots:
    void on_closeButton_clicked();

    void on_maximizeButton_clicked();

    void on_minimizeButton_clicked();

    void on_serialPortComboBox_currentTextChanged(const QString &arg1);

    void on_baudRateComboBox_currentTextChanged(const QString &arg1);

    void on_dataBitComboBox_currentIndexChanged(int index);

    void on_stopComboBox_currentIndexChanged(int index);

    void on_parityComboBox_currentIndexChanged(int index);

    void on_openSerialPortButton_clicked();

    void on_closeSerialPortButton_clicked();

    void read_data_show();

    void on_clearWindowButton_clicked();

    void on_saveWindowButton_clicked();

    void on_closeSendButton_clicked();

    void on_openFileButton_clicked();

    void on_sendFileButton_clicked();

    void on_timerCheckBox_clicked(bool checked);

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    bool mouse_bDrag;
    QPoint mouse_clickPos;
    QSerialPort *sp;
    MySerialPort *serialPort;
    QTimer *timer;
    bool timer_bDrag;
    bool sendFT_bDrag;
};
#endif // MAINWINDOW_H
