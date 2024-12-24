#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr,QPlainTextEdit * textedit=nullptr);
    ~ReplaceDialog();

private slots:
    void on_btnFindNext_clicked();

    void on_btnReplace_clicked();

    void on_btnReplaceAll_clicked();

    void on_btnCancel_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *pText;
};

#endif // REPLACEDIALOG_H
