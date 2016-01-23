#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QtWidgets>
#include "eventsmodel.h"

class MainInterface: public QWidget
{
    Q_OBJECT
public:
    friend class NewEvent;
    MainInterface(QWidget *parent = 0);
    ~MainInterface();
    void set(QString,QString,QString,QString,QString,QString,QString);
    void up(QString,QString,QString,QString,QString,QString,QString);
    bool isFilter();
private slots:
    void settings();
    void closeset();
    void newevent();
    void edititem(QTableWidgetItem*);
    void changedetails(QTableWidgetItem*);
    void del();
    void menushow(const QPoint&);
    void fbig();
    void fsmall();
    void themes();
    void places();
    void card();
    void card(QTableWidgetItem*);
    void indetail();
    void exporttable();
    void submitexport();
    void choosepath();
    void setpath();
    void importtable();
    void exceltemplate();
    void submitimport();
    void upsettings();
    void filterday(int);
    void filtermonth(int);
    void filteryear(int);
    void filteranniver();
    void filtertheme(int);
    void filterplace(int);
    void filterphotos();
    void filtersearch(QString);
signals:
    void senditem(QTableWidgetItem*);
private:
    int filter[8];
    int fday;
    QString fmonth;
    int fyear;
    int fanniver;
    QString ftheme;
    QString fplace;
    void setfilter();
    bool eventFilter(QObject*, QEvent*);
    EventsModel *db;
    QVBoxLayout *mlayout;
    QTableWidget *table;
    //
    QWidget *wgtpath;
    QVBoxLayout *laypath;
    QTreeView *viewpath;
    QPushButton *okpath;
    QDirModel *dirpath;
    // экспорт
    QWidget *exportwgt;
    QLineEdit *path;
    QPushButton *choose;
    QPushButton *submit;
    QGroupBox *texportbox;
    QRadioButton *print;
    QRadioButton *browser;
    QVBoxLayout *texportlay;
    QVBoxLayout *exportlay;
    QHBoxLayout *exportpathlay;
    // ----------------------
    // импорт
    QWidget *importwgt;
    QVBoxLayout *importlayout;
    QPushButton *excel;
    QPushButton *importbtn;
    // ----------------------
    // подробное описание
    QLineEdit *date;
    QLineEdit *theme;
    QLineEdit *place;
    QHBoxLayout *hlay[2];
    QVBoxLayout *vlay;
    QVBoxLayout *datelay;
    QVBoxLayout *themelay;
    QVBoxLayout *placelay;
    QVBoxLayout *extralay;
    QVBoxLayout *ldesclay;
    QGroupBox *eventgroup;
    QLabel *datelbl;
    QLabel *themelbl;
    QLabel *placelbl;
    QLabel *extralbl;
    QLabel *ldesclbl;
    QLineEdit *extra;
    QTextEdit *ldesc;
    // ----------------------
    QMenu *contextmenu;
    QAction *cmm[3];
    int showlist;
    QListWidget *list;
    QHBoxLayout *laylist;
    // меню
    QMenuBar *bar;
    QMenu *menu;
    QMenu *menu2;
    QAction *asettings;
    QAction *areference;
    QAction *anewevent;
    QAction *aimport;
    QAction *aexport;
    QAction *athemes;
    QAction *aplaces;
    QAction *aexit;
    QAction *abigfont;
    QAction *asmallfont;
    QAction *aanniversary;
    QAction *aphotos;
    QAction *aindetail;
    // ----------------------
    // поиск + фильтр
    QGroupBox *search;
    QGroupBox *datesearch;
    QHBoxLayout *msearchlay;
    QHBoxLayout *datesearchlay;
    QHBoxLayout *searchlay;
    QCheckBox *anniversary;
    QComboBox *day;
    QComboBox *month;
    QComboBox *year;
    QLabel *daylbl;
    QLabel *monthlbl;
    QLabel *yearlbl;
    QLineEdit *searchinput;
    // ----------------------
    // settings
    QLabel *lbl[2];
    QVBoxLayout *setlay;
    QHBoxLayout *btnlay;
    QHBoxLayout *pathlay;
    QPushButton *choosep;
    QWidget *settingswgt;
    QVBoxLayout *launchlay;
    QLineEdit *settingspath;
    QSlider *quality;
    QPushButton *ok;
    QPushButton *cancel;
    QGroupBox *launchbox;
    QRadioButton *alldates;
    QRadioButton *anniversarytoday;
    // ----------------------
};

#endif // MAININTERFACE_H
