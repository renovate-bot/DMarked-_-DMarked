#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"

#include <QString>
#include <DSearchEdit>
#include <DTitlebar>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <DLabel>
#include <DPlainTextEdit>
#include <DSplitter>
#include <QWebEngineView>
#include <QAction>

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openFile(const QString &path);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();

private:
    bool isModified() const;
    void setAllAction();

    DPlainTextEdit *m_eidtor_widget;
    QSplitter *m_splitter;
    QWebEngineView *m_preview_widget;

    QLayout *m_central_layout;
    DWidget *m_central_widget;

    QString m_filePath;
    Document m_content;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
