#include "favoritesmanager.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

FavoritesManager::FavoritesManager(QObject *parent)
    : QObject{parent}
{
    // 设置收藏夹存储路径
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir().mkpath(dataDir); // 确保目录存在
    favoritesFilePath = dataDir + "/favorites.txt";
    // qDebug()<<favoritesFilePath;
    loadFavorites(); // 程序启动时加载收藏夹
}

void FavoritesManager::addFavorite(const QString &filePath)
{
    if (!favorites.contains(filePath)) {
        favorites.append(filePath);
        emit favoritesUpdated(); // 发射收藏夹更新信号
        saveFavorites(); // 保存到磁盘
    }
}

void FavoritesManager::removeFavorite(const QString &filePath)
{
    if (favorites.contains(filePath)) {
        favorites.removeAll(filePath);
        emit favoritesUpdated(); // 发射收藏夹更新信号
        saveFavorites(); // 保存到磁盘
    }
}

QList<QString> FavoritesManager::getFavorites() const
{
    return favorites;
}

void FavoritesManager::loadFavorites()
{
    QFile file(favoritesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    favorites.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty())
            favorites.append(line);
    }
    file.close();
}

void FavoritesManager::saveFavorites()
{
    QFile file(favoritesFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const QString &filePath : favorites) {
        out << filePath << '\n';
    }
    file.close();
}
