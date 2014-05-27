/*
# libPKG
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
# libPKG is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 2.1.
*/
#include "include/pkg.h"


PKG::PKG()
{
}

QString PKG::_tmp()
{
    int uid = 0;

    #ifndef Q_WS_WIN
    uid = getuid();
    #endif

    if (uid > 0) {
        QString tmp_home = QDir::homePath()+"/tmp";
        QDir tmp_folder(tmp_home);
        if (!tmp_folder.exists())
            tmp_folder.mkdir(tmp_home);
        return tmp_home;
    }
    else
        return "/tmp";
}

QString PKG::_home()
{
    int uid = 0;

    #ifndef Q_WS_WIN
    uid = getuid();
    #endif

    if (uid > 0)
        return QDir::homePath();
    else
        return "/usr";
}

QString PKG::_db()
{
    int uid = 0;

    #ifndef Q_WS_WIN
    uid = getuid();
    #endif

    if (uid > 0)
        return _home()+"/.pkgdb";
    else
        return "/var/db/pkg";
}

QString PKG::_tools()
{
    int uid = 0;

    #ifndef Q_WS_WIN
    uid = getuid();
    #endif

    if (uid > 0)
        return _home()+"/pkg/sbin";
    else
        return "/usr/pkg/sbin";
}

QString PKG::_make()
{
    int uid = 0;

    #ifndef Q_WS_WIN
    uid = getuid();
    #endif

    if (uid > 0)
        return _home()+"/pkg/bin/bmake";
    else
        return "/usr/pkg/bin/bmake";
}

QStringList PKG::_installed_packages()
{
    QStringList pkg_info_result;
    QString pkg_info_path = _tools()+"/pkg_info";
    QFile pkg_info_file (pkg_info_path);
    if (pkg_info_file.exists()) {
        QProcess pkg_info_proc;
        pkg_info_proc.setProcessChannelMode(QProcess::MergedChannels);
        pkg_info_proc.start(pkg_info_path);
        pkg_info_proc.waitForFinished(-1);
        QString raw_result = pkg_info_proc.readAll();
        QStringList pkgList = raw_result.split("\n",QString::SkipEmptyParts);
        foreach(QString pkg,pkgList) {
            QStringList pkgInfo = pkg.split(" ",QString::SkipEmptyParts);
            if (!pkgInfo.isEmpty()) {
                QString pkgFile = pkgInfo.takeFirst();
                QString pkgDesc;
                if (!pkgInfo.isEmpty()) {
                    foreach(QString pkgInfoDesc,pkgInfo)
                        pkgDesc.append(pkgInfoDesc+" ");
                }
                QString pkgVersion = pkgFile.mid(pkgFile.lastIndexOf("-")).replace("-","");
                QString pkgName = pkgFile.left(pkgFile.lastIndexOf("-"));
                if (!pkgName.isEmpty()&&!pkgVersion.isEmpty()&&!pkgDesc.isEmpty())
                    pkg_info_result << pkgName+"|"+pkgVersion+"|"+pkgDesc;
            }
        }
    }
    return pkg_info_result;
}

QString PKG::_info(QString pkg, QString cat)
{
    QString result;
    if (cat.isEmpty()) {
        QString pkg_info_path = _tools()+"/pkg_info";
        QFile pkg_info_file (pkg_info_path);
        if (pkg_info_file.exists() && !pkg.isEmpty()) {
            QProcess pkg_info_proc;
            QStringList pkg_info_args;
            pkg_info_args << "-X" << pkg;
            pkg_info_proc.setProcessChannelMode(QProcess::MergedChannels);
            pkg_info_proc.start(pkg_info_path,pkg_info_args);
            pkg_info_proc.waitForFinished(-1);
            result = pkg_info_proc.readAll();
        }
    }
    else {
        QString work_path(_home()+"/pkgsrc/"+cat+"/"+pkg);
        QDir work_dir(work_path);
        if (work_dir.exists() && !_make().isEmpty()) {
            QString pkg_name;
            QString pkg_version;
            //QString pkg_depends;
            //QString pkg_options;
            //QString pkg_comment;
            //QString pkg_description;

            QProcessEnvironment env_name = QProcessEnvironment::systemEnvironment();
            env_name.insert("VARNAME","PKGBASE");

            QProcessEnvironment env_version = QProcessEnvironment::systemEnvironment();
            env_version.insert("VARNAME","PKGVERSION");

            QStringList show_var;
            show_var << "show-var";

            //QStringList show_depends;
            //show_depends << "show-depends";

            QProcess pkg_info_proc;
            pkg_info_proc.setProcessChannelMode(QProcess::MergedChannels);
            pkg_info_proc.setWorkingDirectory(work_path);

            pkg_info_proc.setProcessEnvironment(env_name);
            pkg_info_proc.start(_make(),show_var);
            pkg_info_proc.waitForFinished(-1);
            pkg_name = pkg_info_proc.readAll();
            pkg_info_proc.close();

            pkg_info_proc.setProcessEnvironment(env_version);
            pkg_info_proc.start(_make(),show_var);
            pkg_info_proc.waitForFinished(-1);
            pkg_version = pkg_info_proc.readAll();
            pkg_info_proc.close();

            if (!pkg_name.isEmpty() && !pkg_version.isEmpty()) {
                result.append("PKGNAME="+pkg_name.simplified()+"-"+pkg_version.simplified()+"\n");
            }

            /*pkg_info_proc.start(_make(),show_depends);
            pkg_info_proc.waitForFinished(-1);
            pkg_depends = pkg_info_proc.readAll();
            pkg_info_proc.close();

            QStringList pkg_depends_list = pkg_depends.split("\n",QString::SkipEmptyParts);
            for (int i = 0; i < pkg_depends_list.size(); ++i) {
                if (!pkg_depends_list.at(i).isEmpty())
                    result.append("DEPENDS="+pkg_depends_list.at(i)+"\n");
            }*/

            QFile pkg_Makefile(work_path+"/Makefile");
            if (pkg_Makefile.exists()) {
                if (pkg_Makefile.open(QIODevice::ReadOnly)) {
                    QTextStream stream(&pkg_Makefile);
                    QString line;
                    do {
                        line = stream.readLine();
                        if (line.contains("COMMENT="))
                            result.append(line.simplified()+"\n");
                    } while (!line.isNull());
                    pkg_Makefile.close();
                }
            }
        }
    }
    return result;
}

QStringList PKG::_available_local_packages(QString cat, QString search)
{
    QStringList output;
    QString workdir = _home()+"/pkgsrc";
    QString searchdir = workdir;

    if (!cat.isEmpty()) {
        workdir = workdir+"/"+cat;
    }

    if (!search.isEmpty()) {
        QStringList categories = _available_local_categories();
        for (int i = 0; i < categories.size(); ++i) {
            QDir pkgsrc(searchdir+"/"+categories.at(i));
            QFileInfoList pkgsrcList(pkgsrc.entryInfoList(QDir::NoDotAndDotDot|QDir::AllDirs));
            foreach (QFileInfo folder, pkgsrcList) {
                if (folder.isDir()) {
                    QFile Makefile(folder.filePath()+"/Makefile");
                    if (Makefile.exists(folder.filePath()+"/Makefile") && folder.fileName().contains(search)) {
                        output << categories.at(i)+"|"+folder.fileName();
                    }
                }
            }
        }
    }
    else {
        QDir pkgsrc(workdir);
        QFileInfoList pkgsrcList(pkgsrc.entryInfoList(QDir::NoDotAndDotDot|QDir::AllDirs));
        foreach (QFileInfo folder, pkgsrcList) {
            if (folder.isDir()) {
                QFile Makefile(folder.filePath()+"/Makefile");
                if (Makefile.exists(folder.filePath()+"/Makefile")) {
                    if (!cat.isEmpty()) {
                        output << cat+"|"+folder.fileName();
                    }
                    else {
                        output << folder.fileName();
                    }
                }
            }
        }
    }
    return output;
}

QStringList PKG::_available_local_categories()
{
    QStringList output;
    QString work_path = _home()+"/pkgsrc";
    QDir pkgsrc(work_path);
    QFileInfoList pkgsrcList(pkgsrc.entryInfoList(QDir::NoDotAndDotDot|QDir::AllDirs));
    foreach (QFileInfo folder, pkgsrcList) {
        if (folder.isDir()) {
            if (folder.baseName()!= "regress") {
                QFile Makefile;
                if (Makefile.exists(folder.filePath()+"/Makefile"))
                    output << folder.fileName();
            }
        }
    }
    return output;
}
