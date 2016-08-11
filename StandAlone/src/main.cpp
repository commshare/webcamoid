/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2016  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://webcamoid.github.io/
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QWindow>
#include <QTranslator>
#include <QSettings>
#include <QDir>

#include "mediatools.h"
#include "videodisplay.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    /*获取命令行参数,路径错误了
     * Arguments :  ("J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\webcamoid.exe", "-r", "-q", "J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\libAvKys", "-p", "J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\libAvKys\\Plugins")
     *
     * Arguments :  ("J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\webcamoid.exe", "-r", "-q", "J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\..\\libAvKys", "-p", "J:\\media\\XLIVE\\windows\\webcamoid\\StandAlone\\..\\libAvKys\\Plugins")
    */
    QStringList arguments = QCoreApplication::arguments();
    qDebug() << "Arguments : " << arguments;
/*
    QPalette palette = app.palette();
    palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    app.setPalette(palette);
*/
    QCoreApplication::setApplicationName(COMMONS_APPNAME);
    QCoreApplication::setApplicationVersion(COMMONS_VERSION);
    QCoreApplication::setOrganizationName(COMMONS_APPNAME);
    QCoreApplication::setOrganizationDomain(QString("%1.com").arg(COMMONS_APPNAME));

    // Install translations.
    QTranslator translator;
    translator.load(QLocale::system().name(), ":/Webcamoid/share/ts");
    QCoreApplication::installTranslator(&translator);

    // Install fallback icon theme.
    if (QIcon::themeName().isEmpty())
        QIcon::setThemeName("hicolor");
    /*应用程序路径*/
    QString appPath=QCoreApplication::applicationDirPath();
    qDebug()<<"##appPath["<<appPath<<"]";
    /*MediaTools是在哪里创建的呢？*/
    MediaTools::setApplicationDir(appPath);

    /* QCommandLineParser是提供了一系列命令行参数的类*/
    QCommandLineParser cliOptions;
    /*
      添加help选项(在窗口上有-h,--help and ?)，这个选项是由
      QCommandLineParser自动自动处理的
    */
    cliOptions.addHelpOption();
    /*
     添加 -v/--version选项，这个用来显示应用程序的版本信息
     这个同样是由QCommandLineParser自动处理，同样可以通过
     QCoreApplication::setApplicationVersion()来添加版本信息
    */
    cliOptions.addVersionOption();
    cliOptions.setApplicationDescription(QObject::tr("Webcam capture application."));

    /*如果PATH为空，从应用程序路径加载配置文件*/
    QCommandLineOption configPathOpt(QStringList() << "c" << "config",
                                     QObject::tr("Load settings from PATH. "
                                                 "If PATH is empty, load configs "
                                                 "from application directory."),
                                     "PATH", ""/*这就是为空吧*/);
    cliOptions.addOption(configPathOpt);

    QSettings config;

    config.beginGroup("PluginConfigs");

    /*qml路径*/
    QString qmlPluginPath = config.value("qmlPluginPath", Ak::qmlPluginPath())
                                  .toString();
    /*
     * 这个路径好像是设置进来了，可是这个是给qml用的？
     * "##qmlPluginPath[../libAvKys -p J:/media/XLIVE/windows/webcamoid/libAvKys/Plugins]"
    */
    qDebug()<<"##qmlPluginPath["+qmlPluginPath+"]";

    QCommandLineOption qmlPathOpt(QStringList() << "q" << "qmlpath",
                                  QObject::tr("Path to search the Qml interface."),
                                  "PATH", qmlPluginPath);

    cliOptions.addOption(qmlPathOpt);

    // Set recursive plugin path search. 插件 搜索路径,应该是true的
    bool recursive = config.value("recursive", false).toBool();
    qDebug()<<"###recursive["<<recursive<<"]";

    QCommandLineOption recursiveOpt(QStringList() << "r" << "recursive",
                                    QObject::tr("Search in the specified plugins paths recursively."));
    cliOptions.addOption(recursiveOpt);

    // Set the paths for plugins search. 搜寻插件的路径
    QStringList defaultPluginPaths = AkElement::searchPaths();
    /*
     * defaultPluginPaths[ ("J:/media/XLIVE/windows/webcamoid/lib/avkys") ]
    */
    qDebug()<<"defaultPluginPaths["<<defaultPluginPaths<<"]";
    int size = config.beginReadArray("paths");

    for (int i = 0; i < size; i++) {
        config.setArrayIndex(i);
        QString path = config.value("path").toString();

#ifdef Q_OS_WIN32
        path = MediaTools::convertToAbsolute(path);
#endif

        path = QDir::toNativeSeparators(path);

        if (!defaultPluginPaths.contains(path))
            AkElement::addSearchPath(path);
    }

    QCommandLineOption pluginPathsOpt(QStringList() << "p" << "paths",
                                      QObject::tr("Semi-colon separated list of paths to search for plugins."),
                                      "PATH1;PATH2;PATH3;...");
    cliOptions.addOption(pluginPathsOpt);

    config.endArray();
    config.endGroup();

    cliOptions.process(app);

    // Set path for loading user settings.
    if (cliOptions.isSet(configPathOpt)) {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QString configPath = cliOptions.value(configPathOpt);

        if (configPath.isEmpty())
            configPath = QCoreApplication::applicationDirPath();

        configPath = MediaTools::convertToAbsolute(configPath);

        QSettings::setPath(QSettings::IniFormat,
                           QSettings::UserScope,
                           configPath);
    }

    // Set Qml plugins search path.
    if (cliOptions.isSet(qmlPathOpt))
        qmlPluginPath = cliOptions.value(qmlPathOpt);

#ifdef Q_OS_WIN32
    qmlPluginPath = MediaTools::convertToAbsolute(qmlPluginPath);
#endif

#if 0
    Ak::setQmlPluginPath(qmlPluginPath);
#else
    /* J:\media\XLIVE\windows\webcamoid\lib\qt\qml */
    QString mypath="J:/MYSELF/webcamoid/lib/qt/qml";
    Ak::setQmlPluginPath(mypath);
#endif

    // Set recusive search.
    if (cliOptions.isSet(recursiveOpt))
        recursive = true;

    AkElement::setRecursiveSearch(recursive);

    // Set alternative paths to search for plugins.
    if (cliOptions.isSet(pluginPathsOpt)) {
        QStringList defaultPluginPaths = AkElement::searchPaths();
        QStringList pluginPaths = cliOptions.value(pluginPathsOpt)
                                             .split(';');

        foreach (QString path, pluginPaths) {
#ifdef Q_OS_WIN32
            path = MediaTools::convertToAbsolute(path);
            qDebug()<<"path["+path+"]";
#endif

            path = QDir::toNativeSeparators(path);

            if (!defaultPluginPaths.contains(path))
                AkElement::addSearchPath(path);
        }
    }

    // Load cache
    config.beginGroup("PluginsCache");
    size = config.beginReadArray("paths");
    QStringList pluginsCache;

    for (int i = 0; i < size; i++) {
        config.setArrayIndex(i);
        QString path = config.value("path").toString();

#ifdef Q_OS_WIN32
        path = MediaTools::convertToAbsolute(path);
#endif

        pluginsCache << path;
    }

    AkElement::setPluginsCache(pluginsCache);
    config.endArray();
    config.endGroup();

    // Initialize environment.
    QQmlApplicationEngine engine;
    MediaTools mediaTools(&engine);

    // @uri Webcamoid
    qmlRegisterType<VideoDisplay>("Webcamoid", 1, 0, "VideoDisplay");

    engine.rootContext()->setContextProperty("Webcamoid", &mediaTools);
    engine.load(QUrl(QStringLiteral("qrc:/Webcamoid/share/qml/main.qml")));

    emit mediaTools.interfaceLoaded();

    foreach (QObject *obj, engine.rootObjects()) {
        QWindow *applicationWindow = qobject_cast<QWindow *>(obj);

        // Set window icon.
        applicationWindow->setIcon(QIcon::fromTheme("webcamoid",
                                                    QIcon(":/icons/hicolor/scalable/webcamoid.svg")));

        // First, find where to enbed the UI.
        VideoDisplay *videoDisplay = obj->findChild<VideoDisplay *>("videoDisplay");

        if (!videoDisplay)
            continue;

        QObject::connect(&mediaTools,
                         &MediaTools::frameReady,
                         videoDisplay,
                         &VideoDisplay::setFrame);

        break;
    }

    return app.exec();
}
