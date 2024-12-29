#ifndef FAVORITESMANAGER_H
#define FAVORITESMANAGER_H

#include <QObject>
#include <QString>
#include <QList>

class FavoritesManager : public QObject
{
    Q_OBJECT
public:
    explicit FavoritesManager(QObject *parent = nullptr);
    void addFavorite(const QString &filePath); // 添加收藏路径
    void removeFavorite(const QString &filePath); // 删除收藏路径
    QList<QString> getFavorites() const; // 获取所有收藏路径

    void loadFavorites(); // 从磁盘加载收藏夹
    void saveFavorites(); // 保存收藏夹到磁盘

signals:
    void favoritesUpdated(); // 收藏夹更新信号
private:
    QList<QString> favorites; // 存储收藏路径
    QString favoritesFilePath; // 收藏夹数据存储文件路径
};

#endif // FAVORITESMANAGER_H
