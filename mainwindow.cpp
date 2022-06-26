#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSizeGrip>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSizeGrip *sg = new QSizeGrip(ui->sizeGripLabel);
    serialPort = new MySerialPort();
    setWindowFlag(Qt::FramelessWindowHint);
    ui->sendTabWidget->setAttribute(Qt::WA_StyledBackground);
    ui->sendTabWidget->setStyleSheet("QTabWidget:pane {border:0px solid #e8f3f9;background: transparent; }");
    mouse_bDrag = false;
    timer_bDrag = false;
    sendFT_bDrag = false;
    timer = new QTimer();

    sg->setStyleSheet("width:24px;height:24px;background: transparent;");

    if(serialPort->serial_search().count()!=0)
    {
        ui->serialPortComboBox->addItems(serialPort->serial_search());
        serialPort->setPortName(ui->serialPortComboBox->currentText());
    }else
    {
        QMessageBox::information(this, "提示", "没有找到可用串口");
    }
    sp = serialPort->default_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        mouse_clickPos = e->pos();
        if(ui->titleBarWidget->rect().contains(e->pos()))
        {
            mouse_bDrag = true;
        }else
        {
            mouse_bDrag = false;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(mouse_bDrag)
    {
        this->move(e->pos()+this->pos()-mouse_clickPos);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    mouse_bDrag = false;
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::on_maximizeButton_clicked()
{
    this->isMaximized()?this->showNormal():this->showMaximized();
}

void MainWindow::on_minimizeButton_clicked()
{
    this->showMinimized();
}

void MainWindow::on_serialPortComboBox_currentTextChanged(const QString &arg1)
{
    serialPort->setPortName(ui->serialPortComboBox->currentText());
}

void MainWindow::on_baudRateComboBox_currentTextChanged(const QString &arg1)
{
    serialPort->setBaudRate(ui->baudRateComboBox->currentText().toInt());
}

void MainWindow::on_dataBitComboBox_currentIndexChanged(int index)
{
    serialPort->setDataBits(index);
}


void MainWindow::on_stopComboBox_currentIndexChanged(int index)
{
    serialPort->setStopBits(index);
}

void MainWindow::on_parityComboBox_currentIndexChanged(int index)
{
    serialPort->setParity(index);
}

void MainWindow::read_data_show()
{
    ui->receivePlainTextEdit->appendPlainText(serialPort->receive_data());
}

void MainWindow::on_openSerialPortButton_clicked()
{
    switch (serialPort->openSerialPort()) {
    case 0:
        QMessageBox::information(this, "提示", "串口已经打开");
        break;
    case 1:

        connect(sp, &QSerialPort::readyRead, this, &MainWindow::read_data_show);
        QMessageBox::information(this, "提示", "串口打开成功");
        ui->openSerialPortButton->setStyleSheet("background-color:#E66355; color:#fed71a");
        ui->sendButton->setEnabled(true);
        ui->sendFileButton->setEnabled(true);
        break;
    case 2:
        QMessageBox::warning(this, "错误", "串口打开失败");
        ui->openSerialPortButton->setStyleSheet("background-color: #475164;");
        break;
    }
}

void MainWindow::on_closeSerialPortButton_clicked()
{
    if(serialPort->closeSerialPort())
    {
        QMessageBox::information(this, "提示", "串口关闭成功");
        ui->sendButton->setEnabled(false);
        ui->sendFileButton->setEnabled(false);
        ui->openSerialPortButton->setStyleSheet("background-color: #475164;");
    }else
    {
        QMessageBox::warning(this, "提示", "串口关闭失败");
    }
}

void MainWindow::on_clearWindowButton_clicked()
{
    ui->receivePlainTextEdit->clear();
}

void MainWindow::on_saveWindowButton_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QFileDialog::saveFileContent(ui->receivePlainTextEdit->toPlainText().toLocal8Bit(), "bb.txt");
}

void MainWindow::on_sendButton_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    sendFT_bDrag = !sendFT_bDrag;

    if(sendFT_bDrag == true){
        ui->sendButton->setStyleSheet("background-color: rgb(255, 255, 128); color: #ff557f;");
        ui->sendButton->setText("停止发送");
        ui->timerCheckBox->setEnabled(false);
        if(timer_bDrag == true){
            timer->setInterval(ui->timerCycleLineEdit->text().toInt());
            timer->start();
            connect(timer, &QTimer::timeout, this, [=]() -> void {
//                qDebug()<<ui->sendPlainTextEdit->toPlainText().toLocal8Bit();
                serialPort->sendSerialData(ui->sendPlainTextEdit->toPlainText().toLocal8Bit());
            });
        }else{
//            qDebug()<<ui->sendPlainTextEdit->toPlainText().toLocal8Bit();
            serialPort->sendSerialData(ui->sendPlainTextEdit->toPlainText().toLocal8Bit());
        }

    }else{
        timer_bDrag = false;
        timer->stop();
        ui->sendButton->setText("发送");
        ui->sendButton->setStyleSheet("background-color: #475164; color: rgb(198, 213, 98);");
        ui->timerCheckBox->setEnabled(true);
    }
}

void MainWindow::on_closeSendButton_clicked()
{
    ui->sendPlainTextEdit->clear();
}


void MainWindow::on_openFileButton_clicked()
{
    serialPort->fileName = QFileDialog::getOpenFileName(this, "打开读取文件", "/", tr("文件 (*.txt)"));
    ui->openFileLineEdit->setPlaceholderText(serialPort->fileName);
}

void MainWindow::on_sendFileButton_clicked()
{
    if(serialPort->sendSerialFileData())
    {
        serialPort->sendSerialFileData();
        QMessageBox::information(this, "提示", "发送成功");
    }else
    {
        QMessageBox::information(this, "提示", "发送失败");
    }
}

void MainWindow::on_timerCheckBox_clicked(bool checked)
{
    if(ui->timerCycleLineEdit->text().isEmpty() && checked == true)
    {
        checked = false;
        timer_bDrag = false;
        ui->timerCheckBox->setChecked(checked);
        QMessageBox::information(this, "周期时间设置提示", "没有设置周期时间");
    }else if (!ui->timerCycleLineEdit->text().isEmpty() && checked == true) {
        timer_bDrag = true;
    }
}
