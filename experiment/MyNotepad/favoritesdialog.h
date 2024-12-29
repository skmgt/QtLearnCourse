#ifndef FAVORITESDIALOG_H
#define FAVORITESDIALOG_H

#include <QDialog>
#include <QObject>
#include <QListWidget>
#include <QPushButton>
#include "favoritesManager.h"
class FavoritesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FavoritesDialog(FavoritesManager *manager, QWidget *parent = nullptr);

private slots:
    void openSelectedFile(); // 打开选中的文件
    void removeSelectedFavorite(); // 删除选中的收藏路径
    void addFavorite(); // 添加新的收藏路径
    void updateFavoritesList(); // 更新收藏列表

signals:
    void openFileRequested(const QString &filePath); // 请求打开文件的信号
private:
    FavoritesManager *manager;

    QListWidget *favoritesList; // 显示收藏路径的列表
    QPushButton *addButton;     // 添加按钮
    QPushButton *removeButton;  // 删除按钮
    QPushButton *openButton;    // 打开按钮
};

#endif // FAVORITESDIALOG_H
