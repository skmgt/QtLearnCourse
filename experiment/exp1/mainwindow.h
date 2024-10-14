#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString operand;
    QString opcode;

    QStack<QString> operands;
    QStack<QString> opcodes;
    QMap<int,QPushButton*> btnNums;

    QString calculation();
private slots:

    void btnNumClicked();

    void on_btnPoint_clicked();

    void on_btnBack_clicked();

    void on_btnClearAll_clicked();

    void btnBinaryOperatorClicked();
    void btnUnaryOperatorClicked();

    void on_btnEqual_clicked();

    void on_btnOpp_clicked();

    virtual void keyPressEvent(QKeyEvent *event);

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
