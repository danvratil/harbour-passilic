/*
 *   Copyright (C) 2019  Daniel Vrátil <dvratil@kde.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "imageprovider.h"

#include <QPainter>
#include <QColor>
#include <QMap>

#include <sailfishapp.h>

namespace {

static const auto schemaMap = QMap<QString, QString>{
    {{QStringLiteral("passIcon"), QStringLiteral("icons")},
     {QStringLiteral("passImage"), QStringLiteral("images")}}
};

static QString pathForSchema(const QString &schema)
{
    return schemaMap.value(schema);
}

}

ImageProvider::ImageProvider(const QString &schema)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
    , mSchema(schema)
{
    Q_ASSERT(schemaMap.contains(schema));
}

// Based on https://together.jolla.com/question/44325/iconbutton-how-to-use-own-icons-with-highlight/
QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const auto parts = id.splitRef(QLatin1Char('?'));

    auto source = findSource(parts.at(0));
    if (size) {
        *size = source.size();
    }

    if (parts.size() > 1) {
        const auto color = parts.at(1).toString();
        if (QColor::isValidColor(color)) {
            QPainter painter(&source);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(source.rect(), color);
            painter.end();
        }
    }

    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        return source.scaled(requestedSize, Qt::IgnoreAspectRatio);
    } else {
        return source;
    }
}

QPixmap ImageProvider::findSource(const QStringRef &name)
{
    const QString imgPath = QStringLiteral("qml/") % pathForSchema(mSchema) % QStringLiteral("/") % name;
    return QPixmap(SailfishApp::pathTo(imgPath).toString(QUrl::RemoveScheme));
}
