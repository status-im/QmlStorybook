#pragma once

#include "Storybook/abstractpagessource.h"

class DirectoryFilesWatcher;

class LocalPagesSource : public AbstractPagesSource
{
    Q_OBJECT

public:
    explicit LocalPagesSource(const QString &path, QObject *parent = nullptr);

    // AbstractPagesSource interface
    QStringList pages() const override;
    QString page(const QString &page) const override;

private:
    QString m_path;
    DirectoryFilesWatcher* m_pagesWatcher;
};
