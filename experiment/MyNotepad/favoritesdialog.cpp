#include "favoritesdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

FavoritesDialog::FavoritesDialog(FavoritesManager *manager, QWidget *parent)
    : QDialog(parent), manager(manager)
{
    setWindowTitle("收藏夹管理");
    resize(400, 300);

    // 收藏列表
    favoritesList = new QListWidget(this);

    // 按钮
    addButton = new QPushButton("添加", this);
    removeButton = new QPushButton("删除", this);
    openButton = new QPushButton("打开", this);

    connect(addButton, &QPushButton::clicked, this, &FavoritesDialog::addFavorite);
    connect(removeButton, &QPushButton::clicked, this, &FavoritesDialog::removeSelectedFavorite);
    connect(openButton, &QPushButton::clicked, this, &FavoritesDialog::openSelectedFile);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(openButton);

    mainLayout->addWidget(favoritesList);
    mainLayout->addLayout(buttonLayout);

    connect(manager, &FavoritesManager::favoritesUpdated, this, &FavoritesDialog::updateFavoritesList);

    updateFavoritesList(); // 初始化收藏列表
}

void FavoritesDialog::openSelectedFile()
{
    QListWidgetItem *currentItem = favoritesList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "警告", "请选择一个文件！");
        return;
    }

    QString filePath = currentItem->text();
    if (!QFile::exists(filePath)) {
        QMessageBox::warning(this, "错误", "文件不存在！");
        return;
    }

    // 发射信号通知主窗口打开文件
    emit openFileRequested(filePath);


    accept(); // 关闭窗口
}

void FavoritesDialog::removeSelectedFavorite()
{
    QListWidgetItem *currentItem = favoritesList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "警告", "请选择一个收藏路径！");
        return;
    }

    QString filePath = currentItem->text();
    manager->removeFavorite(filePath); // 从收藏夹中删除
}

void FavoritesDialog::addFavorite()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件");
    if (!filePath.isEmpty()) {
        manager->addFavorite(filePath); // 添加到收藏夹
    }
}

void FavoritesDialog::updateFavoritesList()
{
    favoritesList->clear();
    QList<QString> favorites = manager->getFavorites();
    for (const QString &filePath : favorites) {
        favoritesList->addItem(filePath);
    }
}
