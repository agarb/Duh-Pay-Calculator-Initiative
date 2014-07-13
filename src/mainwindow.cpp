#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QDir>

MainWindow::MainWindow(boost::optional<QSettings::Format> setsFormat,
                       boost::optional<QSettings::Scope> setsScope,
                       const QString &setsFileName,
                       const QString &preferedDbFileName,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    {
        auto pSettings = createSettings(setsFormat, setsScope,
                                        setsFileName);
        if(!pSettings) {
            QMessageBox::critical(this, qApp->applicationDisplayName(),
                                  tr("No proper information about settings storage"));
            qApp->exit(1);  //decade about replecing by exception raising
            return;
        }

        settings_.reset( pSettings );
    }

    if(!preferedDbFileName.isEmpty()) {
        openDb(preferedDbFileName);
    }
    else {
        bool openOnStartup =
                settings_->value(setsOpenLastDatafileOnStartup_).toBool();
        if(openOnStartup) {
            QString fileName =
                    settings_->value(setsLastDatafile_).toString();
            openDb(fileName);
        }
    }

    //todo: setLocale & Language

    readGuiSettings();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::setsLastDatafile_ = "last_datafile";
QString MainWindow::setsOpenLastDatafileOnStartup_ = "open_last_datafile_on_startup";

// return nullptr if bad parameters
QSettings *MainWindow::createSettings(boost::optional<QSettings::Format> setsFormat,
                                      boost::optional<QSettings::Scope> setsScope,
                                      const QString &setsFileName)
{
    //todo: phisical creating storage with def values;
    if(setsFormat && setsScope)
    {
        auto pSettings = new QSettings(setsFormat.get(), setsScope.get(),
                                       qApp->organizationName(), qApp->applicationName());
        pSettings->setFallbacksEnabled(false);
        return pSettings;
    }
    else if(!setsFileName.isEmpty())
    {
        auto pSettings = new QSettings(setsFileName, QSettings::IniFormat);
        pSettings->setFallbacksEnabled(false);
        return pSettings;
    }
    else
        return 0;
}

//todo: move gui messages outside function
bool MainWindow::createDbStructure()
{
    QString createTableArea =
            "create table 'area' ( "
            "   month date, "
            "   area numeric "
            ") ";

    QString createTablePayPerM2 =
            "create table 'pay_per_m2' ( "
            "   month date, "
            "   pay numeric "
            ") ";

    auto db = QSqlDatabase::database();
    QSqlQuery q;        

    bool success =
            db.transaction() &&
            q.exec(createTableArea) &&
            q.exec(createTablePayPerM2) &&
            db.commit();

    if(!success) {
        QMessageBox::critical(this, qApp->applicationDisplayName(),
                              tr("Database structure can't be created.\n") +
                              q.lastError().text());
    }

    return success;
}

//todo: move gui messages outside function
bool MainWindow::openDb(const QString &fileName)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(!db.open())
    {
        QMessageBox::critical(this, qApp->applicationDisplayName(),
                              tr("Database don't open.\n")+db.lastError().text());
    }
    return db.isOpen();
}


void MainWindow::readGuiSettings()
{
    //todo:
}

//todo: call this on close event
void MainWindow::writeGuiSettings()
{
    //todo:
}

//todo: move gui messages outside function (use class-wide lastError method)
void MainWindow::on_actionNewDatafile_triggered()
{
    QString uniqueName =
            QUuid::createUuid().toString();
    QString tempFileName =
            QDir::temp().absoluteFilePath(uniqueName);

    openDb(tempFileName);
    createDbStructure();
}
