#include "mainwindow.h"

#include <QFile>
#include <DFileDialog>
#include <DMessageBox>
#include <QTextStream>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent)
{
    resize(1200, 740);

    m_central_widget = new CentralWidget;
    setCentralWidget(m_central_widget);
    setupAction();

    m_search_edit = new DSearchEdit;
    titlebar()->setCustomWidget(m_search_edit);
    m_search_edit->setFixedWidth(400);

//    QHBoxLayout *m_layout = new QHBoxLayout;
//    m_layout->setContentsMargins(0, 0, 0, 0);
//    m_layout->setSpacing(0);
//    m_layout->addWidget();
   // BottomBar *m_bottom_bar = new BottomBar;
   // m_layout->addWidget(m_bottom_bar);
   // m_central_widget->setLayout(m_layout);
    //setStatusBar(m_bottom_bar);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupAction() {
    QMenu *menu;
    QAction *actionNew, *actionOpen, *actionSave, *actionSaveAs;

    actionNew = new QAction(tr("New"));
    actionOpen = new QAction(tr("Open"));
    actionSave = new QAction(tr("Save"));
    actionSaveAs = new QAction(tr("SaveAs"));
    menu = new QMenu;
    menu->addAction(actionNew);
    menu->addAction(actionOpen);
    menu->addAction(actionSave);
    menu->addAction(actionSaveAs);

    titlebar()->setMenu(menu);

    connect(actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(m_central_widget->m_editor_widget->document(), &QTextDocument::modificationChanged,
          actionSave, &QAction::setEnabled);
}

void MainWindow::openFile(const QString &path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_file_path = path;
    m_central_widget->m_editor_widget->setPlainText(f.readAll());
}

bool MainWindow::isModified() const {
    return m_central_widget->m_editor_widget->document()->isModified();
}

void MainWindow::onFileNew() {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    m_file_path.clear();
    m_central_widget->m_editor_widget->setPlainText(tr("## New document"));
    m_central_widget->m_editor_widget->document()->setModified(false);
}

void MainWindow::onFileOpen() {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    QString path = DFileDialog::getOpenFileName(this,
        tr("Open MarkDown File"), "", tr("MarkDown File (*.md)"));
    if (path.isEmpty())
        return;

    openFile(path);
}

void MainWindow::onFileSave() {
    if (m_file_path.isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(m_file_path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_file_path), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << m_central_widget->m_editor_widget->toPlainText();
    m_central_widget->m_editor_widget->document()->setModified(false);
}

void MainWindow::onFileSaveAs() {
    QString path = DFileDialog::getSaveFileName(this,
        tr("Save MarkDown File"), "", tr("MarkDown File (*.md, *.markdown)"));
    if (path.isEmpty())
        return;
    m_file_path = path;
    onFileSave();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != DMessageBox::Yes) {
            event->ignore();
            return;
        }
    }
    event->accept();
}