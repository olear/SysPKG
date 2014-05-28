#include "include/common.h"

Common::Common(QObject *parent) :
    QObject(parent)
{
}

QString Common::_tmp()
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

QString Common::_home()
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

QString Common::_db()
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

QString Common::_tools()
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

QString Common::_make()
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

QStringList Common::_installed_packages()
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

QStringList Common::_info(QString pkg, QString cat)
{
    QStringList result;
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
            QString raw_result = pkg_info_proc.readAll();
            result = raw_result.split("\n");
        }
    }
    else {
        QString work_path(_home()+"/pkgsrc/"+cat+"/"+pkg);
        QDir work_dir(work_path);
        if (work_dir.exists() && !_make().isEmpty()) {
            QString pkg_name;
            //QString pkg_version;
            //QString pkg_depends;
            QString pkg_options;
            QString pkg_comment;

            QProcessEnvironment env_name = QProcessEnvironment::systemEnvironment();
            env_name.insert("VARNAME","PKGNAME");

            //QProcessEnvironment env_version = QProcessEnvironment::systemEnvironment();
            //env_version.insert("VARNAME","PKGVERSION");

            QProcessEnvironment env_comment = QProcessEnvironment::systemEnvironment();
            env_comment.insert("VARNAME","COMMENT");

            QStringList show_var;
            show_var << "show-var";

            /*QStringList show_depends;
            show_depends << "show-depends";
*/
            QStringList show_options;
            show_options << "show-options";

            QProcess pkg_info_proc;
            pkg_info_proc.setProcessChannelMode(QProcess::MergedChannels);
            pkg_info_proc.setWorkingDirectory(work_path);

            pkg_info_proc.setProcessEnvironment(env_name);
            pkg_info_proc.start(_make(),show_var);
            pkg_info_proc.waitForFinished(-1);
            pkg_name = pkg_info_proc.readAll();
            pkg_info_proc.close();

            /*pkg_info_proc.setProcessEnvironment(env_version);
            pkg_info_proc.start(_make(),show_var);
            pkg_info_proc.waitForFinished(-1);
            pkg_version = pkg_info_proc.readAll();
            pkg_info_proc.close();

            if (!pkg_name.isEmpty() && !pkg_version.isEmpty()) {
                result << "PKGNAME="+pkg_name.simplified()+"-"+pkg_version.simplified();
            }*/
            if (!pkg_name.isEmpty())
                result << "PKGNAME="+pkg_name.simplified();

            /*pkg_info_proc.start(_make(),show_depends);
            pkg_info_proc.waitForFinished(-1);
            pkg_depends = pkg_info_proc.readAll();
            pkg_info_proc.close();

            QStringList pkg_depends_list = pkg_depends.split("\n",QString::SkipEmptyParts);
            for (int i = 0; i < pkg_depends_list.size(); ++i) {
                if (!pkg_depends_list.at(i).isEmpty())
                    result << "DEPENDS="+pkg_depends_list.at(i).simplified();
            }*/

            pkg_info_proc.setProcessEnvironment(env_comment);
            pkg_info_proc.start(_make(),show_var);
            pkg_info_proc.waitForFinished(-1);
            pkg_comment = pkg_info_proc.readAll();
            pkg_info_proc.close();

            if (!pkg_comment.isEmpty()) {
                result << "COMMENT="+pkg_comment.simplified();
            }

            if (!cat.isEmpty()) {
                result << "CATEGORIES="+cat;
                result << "PKGPATH="+cat+"/"+pkg;
            }

            pkg_info_proc.start(_make(),show_options);
            pkg_info_proc.waitForFinished(-1);
            pkg_options = pkg_info_proc.readAll();
            pkg_info_proc.close();

            if (!pkg_options.isEmpty()) {
                QString options;
                QStringList optionsList = pkg_options.split("\n");
                QStringList optionsAvail;
                QStringList optionsSelected;
                QString tmp;
                int optionsAdd = 0;
                int optionsDef = 0;
                for (int i = 0; i < optionsList.size(); ++i) {
                    if (optionsList.at(i) == "Any of the following general options may be selected:")
                        optionsAdd=1;
                    if (optionsList.at(i) == "These options are enabled by default:")
                        optionsAdd=0;
                    if (optionsList.at(i) == "These options are currently enabled:")
                        optionsDef=1;
                    if (optionsList.at(i) == "You can select which build options to use by setting PKG_DEFAULT_OPTIONS")
                        optionsDef=0;

                    if (optionsDef==1 && optionsList.at(i) !="These options are currently enabled:" && optionsList.at(i) != "You can select which build options to use by setting PKG_DEFAULT_OPTIONS") {
                        if (!optionsList.at(i).isEmpty())
                            tmp.append(optionsList.at(i).simplified().replace(" ","|"));
                    }
                    if (optionsAdd==1 && optionsList.at(i) != "Any of the following general options may be selected:") {
                        QStringList getFlag = optionsList.at(i).split(" ",QString::SkipEmptyParts);
                        if(!getFlag.isEmpty()) {
                            QString value = getFlag.takeFirst().simplified().replace(" ","");
                            if (!value.isEmpty())
                                optionsAvail << value;
                        }
                    }
                }
                QStringList tmpList = tmp.split("|",QString::SkipEmptyParts);
                for (int i = 0; i < tmpList.size(); ++i) {optionsSelected << tmpList.at(i);}
                for (int i = 0; i < optionsAvail.size(); ++i) {
                    QString availOpt = optionsAvail.at(i);
                    if (!availOpt.isEmpty()) {
                        int foundOpt=0;
                        for (int i = 0; i < optionsSelected.size(); ++i) {
                            if (availOpt==optionsSelected.at(i))
                                foundOpt=1;
                        }
                        if (foundOpt==1)
                            options.append("+"+availOpt+" ");
                        else
                            options.append("-"+availOpt+" ");
                    }
                }
                if (!options.isEmpty())
                    result<<"OPTIONS="+options.simplified();
}
        }
    }
    return result;
}

void Common::_scan_local_packages()
{
    QStringList pkgs = _available_local_packages(_scan_category,"");
    for (int y = 0; y < pkgs.size(); ++y) {
        QString pkg_options;
        QString pkg_depends;
        QString pkg_category;
        QString pkg_name;
        QString pkg_version;
        QString pkg_path;
        QString pkg_comment;
        QStringList pkg_info = _info(pkgs.at(y),_scan_category);
        for (int x = 0; x < pkg_info.size(); ++x) {
            QString line = pkg_info.at(x);
            if (line.contains("OPTIONS="))
                pkg_options = line.replace("OPTIONS=","");
            if (line.contains("DEPENDS="))
                pkg_depends.append(line.replace("DEPENDS=",""));
            if (line.contains("CATEGORIES="))
                pkg_category = line.replace("CATEGORIES=","");
            if (line.contains("PKGPATH="))
                pkg_path = line.replace("PKGPATH=","");
            if (line.contains("COMMENT="))
                pkg_comment = line.replace("COMMENT=","");
            if (line.contains("PKGNAME")) {
                pkg_version = line.mid(line.lastIndexOf("-")).replace("-","").replace("PKGNAME=","");
                pkg_name = line.left(line.lastIndexOf("-")).replace("PKGNAME=","");
            }
        }
        emit _scanned_local_package(pkg_name+"|"+pkg_version+"|"+pkg_category+"|"+pkg_path+"|"+pkg_comment+"|"+pkg_depends+"|"+pkg_options);
    }
    emit _scan_local_packages_done();
}

QStringList Common::_available_local_packages(QString cat, QString search)
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
                    if (Makefile.exists(folder.filePath()+"/Makefile") && folder.fileName().contains(search))
                        output << categories.at(i)+"|"+folder.fileName();
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
                    /*if (!cat.isEmpty())
                        output << cat+"|"+folder.fileName();
                    else*/
                        output << folder.fileName();
                }
            }
        }
    }
    return output;
}

QStringList Common::_available_local_categories()
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
