#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QScopedPointer>

#include <boost/optional.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(boost::optional<QSettings::Format> setsFormat,
                        boost::optional<QSettings::Scope> setsScope,
                        const QString& setsFileName,
                        const QString& preferedDbFileName,
                        QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNewDatafile_triggered();

private:
    static
    QSettings* createSettings(boost::optional<QSettings::Format> setsFormat,
                              boost::optional<QSettings::Scope> setsScope,
                              const QString& setsFileName);

    bool createDbStructure();

    bool openDb(const QString& fileName);

    void readGuiSettings();
    void writeGuiSettings();

    QScopedPointer<QSettings> settings_;
    Ui::MainWindow *ui;

    static QString setsLastDatafile_;
    static QString setsOpenLastDatafileOnStartup_;
};

#endif // MAINWINDOW_H
