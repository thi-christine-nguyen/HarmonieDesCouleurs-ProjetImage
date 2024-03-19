#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // void calculMoyenne();

private slots:
    void on_Import_clicked();
    void on_Monochromatique_clicked();

    void on_Complementaire_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
