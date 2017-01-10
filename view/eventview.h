#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QPushButton>

namespace Ui {
class EventView;
}

class EventView : public QWidget
{
    Q_OBJECT
signals:
    void saved();
    void canceled();
    void uploadBtnClicked();
    void removeBtnClicked();
    void nextBtnClicked();
    void previousBtnClicked();

public:
    explicit EventView(QWidget *parent = 0);
    ~EventView();

    void setDay(int index);
    void setMonth(int index);
    void setYear(const QString &year);
    int selectedDay();
    int selectedMonth();
    int selectedYear();
    void addThemes(const QStringList &themes);
    void addPlaces(const QStringList &places);
    void addSources(const QStringList &sources);
    void setCurrentTheme(const QString &theme);
    void setCurrentPlace(const QString &place);
    void setCurrentSource(const QString &source);
    QComboBox *dayBox();
    QComboBox *monthBox();
    QLineEdit *yearEdit();
    QLineEdit *shortEdit();
    QPlainTextEdit *fullEdit();
    QLineEdit *extraEdit();
    QComboBox *themeBox();
    QComboBox *placeBox();
    QComboBox *sourceBox();
    QPushButton *currentImage();

private:
    Ui::EventView *ui;

};

#endif // EVENTVIEW_H
