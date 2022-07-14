#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_ADD_clicked();

    void on_DEL_clicked();

    void on_Start_btn_clicked();

    void on_radioButton_1_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_textEdit_Search_textChanged();

    void on_pushButton_add_grouplist_clicked();

    void on_pushButton_g_clear_clicked();

public:
    void AddData(const QString& sValue);
    void UpdateLibDataList();
    void ReadLibData();
    void UpdateGroupList();
private:
    Ui::MainWindow *ui;
    int m_combineSize = 2;
    QString m_curSearchKey = "";
    QVector<QString> m_vecLibData;
    QVector<QString> m_vecGroup;
    QSqlDatabase db;
    std::vector<int> vecIndex;
};
#endif // MAINWINDOW_H
