/****************************************************************************
**
** Copyright (C) TERIFLIX Entertainment Spaces Pvt. Ltd. Bengaluru
** Author: Prashanth N Udupa (prashanth.udupa@teriflix.com)
**
** This code is distributed under GPL v3. Complete text of the license
** can be found here: https://www.gnu.org/licenses/gpl-3.0.txt
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QUrl>
#include <QRectF>
#include <QColor>
#include <QAction>
#include <QPalette>
#include <QUndoGroup>
#include <QUndoStack>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QVersionNumber>

#include "undoredo.h"
#include "errorreport.h"
#include "transliteration.h"

typedef QApplication QtApplicationClass;
class QSettings;
class QQuickItem;
class AutoUpdate;

class Application : public QtApplicationClass
{
    Q_OBJECT

public:
    static Application *instance();
    Application(int &argc, char **argv, const QVersionNumber &version);
    ~Application();

    QString installationId() const;
    QDateTime installationTimestamp() const;
    int launchCounter() const;

    Q_PROPERTY(QPalette palette READ palette CONSTANT)
    Q_PROPERTY(qreal devicePixelRatio READ devicePixelRatio CONSTANT)
    Q_PROPERTY(QFont font READ applicationFont NOTIFY applicationFontChanged)
    QFont applicationFont() const { return this->font(); }
    Q_SIGNAL void applicationFontChanged();

    Q_INVOKABLE QString urlToLocalFile(const QUrl &url) { return url.toLocalFile(); }

    enum Platform { LinuxDesktop, WindowsDesktop, MacOS };
    Q_ENUM(Platform)
    Q_PROPERTY(Platform platform READ platform CONSTANT)
    Platform platform() const;

    Q_PROPERTY(bool isMacOSPlatform READ isMacOSPlatform CONSTANT)
#ifdef Q_OS_MAC
    bool isMacOSPlatform() const { return true; }
#else
    bool isMacOSPlatform() const { return false; }
#endif

    Q_PROPERTY(bool isWindowsPlatform READ isWindowsPlatform CONSTANT)
#ifdef Q_OS_WIN
    bool isWindowsPlatform() const { return true; }
#else
    bool isWindowsPlatform() const { return false; }
#endif

    Q_PROPERTY(bool isLinuxPlatform READ isLinuxPlatform CONSTANT)
#ifdef Q_OS_MAC
    bool isLinuxPlatform() const { return false; }
#else
#ifdef Q_OS_UNIX
    bool isLinuxPlatform() const { return true; }
#else
    bool isLinuxPlatform() const { return false; }
#endif
#endif

    Q_PROPERTY(QString controlKey READ controlKey CONSTANT)
    QString controlKey() const;

    Q_PROPERTY(QString altKey READ altKey CONSTANT)
    QString altKey() const;

    Q_INVOKABLE QString polishShortcutTextForDisplay(const QString &text) const;

    Q_PROPERTY(QString baseWindowTitle READ baseWindowTitle WRITE setBaseWindowTitle NOTIFY baseWindowTitleChanged)
    void setBaseWindowTitle(const QString &val);
    QString baseWindowTitle() const { return m_baseWindowTitle; }
    Q_SIGNAL void baseWindowTitleChanged();

    Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)
    QString qtVersion() const { return QString::fromLatin1(QT_VERSION_STR); }

    Q_INVOKABLE QString typeName(QObject *object) const;
    Q_INVOKABLE bool verifyType(QObject *object, const QString &name) const;

    Q_PROPERTY(QVersionNumber versionNumber READ versionNumber CONSTANT)
    QVersionNumber versionNumber() const { return m_versionNumber; }

    Q_PROPERTY(QUndoGroup* undoGroup READ undoGroup CONSTANT)
    QUndoGroup *undoGroup() const { return m_undoGroup; }

    Q_INVOKABLE UndoStack *findUndoStack(const QString &objectName) const;

    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
    bool canUndo() const { return m_undoGroup->canUndo(); }
    Q_SIGNAL void canUndoChanged();

    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    bool canRedo() const { return m_undoGroup->canRedo(); }
    Q_SIGNAL void canRedoChanged();

    Q_PROPERTY(QString undoText READ undoText NOTIFY undoTextChanged)
    QString undoText() const { return m_undoGroup->undoText(); }
    Q_SIGNAL void undoTextChanged();

    Q_PROPERTY(QString redoText READ redoText NOTIFY redoTextChanged)
    QString redoText() const { return m_undoGroup->redoText(); }
    Q_SIGNAL void redoTextChanged();

    Q_INVOKABLE QJsonObject systemFontInfo() const;
    Q_INVOKABLE QColor pickColor(const QColor &initial) const;
    Q_INVOKABLE QRectF textBoundingRect(const QString &text, const QFont &font) const;
    Q_INVOKABLE void revealFileOnDesktop(const QString &pathIn);
    Q_INVOKABLE QJsonArray enumerationModel(QObject *object, const QString &enumName) const;
    Q_INVOKABLE QJsonArray enumerationModelForType(const QString &typeName, const QString &enumName) const;
    Q_INVOKABLE QString enumerationKey(QObject *object, const QString &enumName, int value) const;
    Q_INVOKABLE QString enumerationKeyForType(const QString &typeName, const QString &enumName, int value) const;
    Q_INVOKABLE QJsonObject fileInfo(const QString &path) const;

    Q_PROPERTY(QString settingsFilePath READ settingsFilePath CONSTANT)
    QString settingsFilePath() const;

    Q_PROPERTY(TransliterationEngine* transliterationEngine READ transliterationEngine CONSTANT)
    TransliterationEngine* transliterationEngine() const { return TransliterationEngine::instance(); }

    Q_INVOKABLE QPointF cursorPosition() const;
    Q_INVOKABLE QPointF mapGlobalPositionToItem(QQuickItem *item, const QPointF &pos) const;

    Q_INVOKABLE void execLater(QObject *context, int howMuchLater, const QJSValue &function, const QJSValueList &args=QJSValueList());

    Q_INVOKABLE QColor translucent(const QColor &input, qreal alpha=0.5) const {
        QColor ret = input;
        ret.setAlphaF(qBound(0.0, ret.alphaF() * alpha, 1.0));
        return ret;
    }

    QSettings *settings() const { return m_settings; }

    Q_PROPERTY(AutoUpdate* autoUpdate READ autoUpdate CONSTANT)
    AutoUpdate *autoUpdate() const;

    Q_INVOKABLE QJsonObject objectConfigurationFormInfo(const QObject *object, const QMetaObject *from) const;

    Q_PROPERTY(QVariantList standardColors READ standardColorsVariantList NOTIFY standardColorsChanged STORED false)
    QVariantList standardColorsVariantList() const { return m_standardColors; }
    Q_SIGNAL void standardColorsChanged();

    Q_INVOKABLE QColor pickStandardColor(int counter) const;
    Q_INVOKABLE QColor textColorFor(const QColor &bgColor) const;
    const QVector<QColor> standardColors() const { return standardColors(QVersionNumber()); }

    Q_INVOKABLE QRectF boundingRect(const QString &text, const QFont &font) const;
    Q_INVOKABLE QRectF intersectedRectangle(const QRectF &of, const QRectF &with) const;
    Q_INVOKABLE bool   doRectanglesIntersect(const QRectF &r1, const QRectF &r2) const;
    Q_INVOKABLE QSizeF scaledSize(const QSizeF &of, const QSizeF &into) const;
    Q_INVOKABLE QRectF uniteRectangles(const QRectF &r1, const QRectF &r2) const;
    Q_INVOKABLE QRectF adjustRectangle(const QRectF &rect, qreal left, qreal top, qreal right, qreal bottom) const;
    Q_INVOKABLE bool   isRectangleInRectangle(const QRectF &bigRect, const QRectF &smallRect) const;
    Q_INVOKABLE QPointF translationRequiredToBringRectangleInRectangle(const QRectF &bigRect, const QRectF &smallRect) const;

    Q_INVOKABLE QPoint mouseCursorPosition() const { return QCursor::pos(); }
    Q_INVOKABLE void moveMouseCursor(const QPoint &pos) { QCursor::setPos(pos); }

    Q_INVOKABLE QString fileContents(const QString &fileName) const;
    Q_INVOKABLE QString fileName(const QString &path) const;

    Q_INVOKABLE QScreen *windowScreen(QObject *window) const;

    Q_INVOKABLE QString getEnvironmentVariable(const QString &name) const;

    // Must be called from main.cpp
    void initializeStandardColors(QQmlEngine *);

    static QVector<QColor> standardColors(const QVersionNumber &version);

    // QCoreApplication interface
    bool notify(QObject *, QEvent *);

    // Although public, please do not call it.
    bool notifyInternal(QObject *object, QEvent *event);

signals:
    void minimizeWindowRequest();

private:
    bool loadScript();

private:
    QSettings *m_settings = nullptr;
    QUndoGroup *m_undoGroup = new QUndoGroup(this);
    QString m_baseWindowTitle;
    ErrorReport *m_errorReport = new ErrorReport(this);
    QVersionNumber m_versionNumber;
    QVariantList m_standardColors;
};

#endif // APPLICATION_H