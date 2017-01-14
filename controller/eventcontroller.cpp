#include "eventcontroller.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>

#include "model/eventssqlmodel.h"
#include "view/eventview.h"

//====================================================================================

const int INVALID_INDEX = -1;
const int MIN_WIDTH = 150;
const int MIN_HEIGHT = 150;
const QString EXTENSION_PATTERN = "(.png)|(.jpg)|(.jpeg)";

//====================================================================================

EventController::EventController(EventView* eventView,
                                 EventsSqlModel* eventsModel,
                                 int currentRow,
                                 QObject* parent) :
    QObject(parent),
    _eventView(eventView),
    _eventsModel(eventsModel),
    _currentRow(currentRow)
{   
    connect(_eventView, &EventView::destroyed, this, &EventController::finished);

    connect(_eventView, &EventView::saved, this, &EventController::saveEvent);
    connect(_eventView, &EventView::canceled, this, &EventController::cancelSaving);

    connect(_eventView, &EventView::uploadBtnClicked, this, &EventController::openFileDialog);
    connect(_eventView, &EventView::removeBtnClicked, this, &EventController::removeImage);
    connect(_eventView, &EventView::nextBtnClicked, this, &EventController::nextImage);
    connect(_eventView, &EventView::previousBtnClicked, this, &EventController::previousImage);
    connect(_eventView, &EventView::currentImageClicked, this, &EventController::openCurrentImage);

    _widgetMapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _widgetMapper.setModel(_eventsModel);

    QStringList themesList = {""}, placesList = {""}, sourcesList = {""};
    for(int i = 0; i != _eventsModel->rowCount(); i++)
    {
        themesList.push_back(_eventsModel->theme(i));
        placesList.push_back(_eventsModel->place(i));
        sourcesList.push_back(_eventsModel->source(i));
    }
    themesList.removeDuplicates();
    placesList.removeDuplicates();
    sourcesList.removeDuplicates();
    _eventView->addThemes(themesList);
    _eventView->addPlaces(placesList);
    _eventView->addSources(sourcesList);

    if(currentRow != INVALID_INDEX)
    {
        _eventView->setDay(_eventsModel->day(currentRow));
        _eventView->setMonth(_eventsModel->month(currentRow));
        _eventView->setYear(QString::number(_eventsModel->year(currentRow)));

        _eventView->setCurrentTheme(_eventsModel->theme(_currentRow));
        _eventView->setCurrentPlace(_eventsModel->place(_currentRow));
        _eventView->setCurrentSource(_eventsModel->source(_currentRow));

        loadImages();
    }
    else
    {
        _eventsModel->insertRow(_eventsModel->rowCount());
        _currentRow = _eventsModel->rowCount() - 1;
    }

    _widgetMapper.addMapping(_eventView->shortEdit(), _eventsModel->column(ShortDescription));
    _widgetMapper.addMapping(_eventView->fullEdit(), _eventsModel->column(LongDescription));
    _widgetMapper.addMapping(_eventView->extraEdit(), _eventsModel->column(ExtraDescription));
    _widgetMapper.addMapping(_eventView->themeBox(), _eventsModel->column(Theme));
    _widgetMapper.addMapping(_eventView->placeBox(), _eventsModel->column(Place));
    _widgetMapper.addMapping(_eventView->sourceBox(), _eventsModel->column(Source));
    _widgetMapper.setCurrentIndex(_currentRow);
}

//====================================================================================

EventController::~EventController()
{
    _widgetMapper.revert();
    _eventsModel->revertAll();
    qDebug() << "controller destroyed";
}

//====================================================================================

void EventController::saveEvent()
{
    _eventsModel->setDate(_currentRow,
                          _eventView->selectedDay(),
                          _eventView->selectedMonth(),
                          _eventView->selectedYear());
    removeTemporaryImages();
    saveImages();
    _eventsModel->setImagesList(_currentRow, getImagesNames());
    _widgetMapper.submit();
    _eventsModel->submitAll();
    delete _eventView;
}

//====================================================================================

void EventController::cancelSaving()
{
    _widgetMapper.revert();
    _eventsModel->revertAll();
    delete _eventView;
}

//====================================================================================

void EventController::openCurrentImage()
{
   if(_currentImageIndex != INVALID_INDEX)
   {
       QDesktopServices process;
       if(_images[_currentImageIndex]._tempPath == QString()) // images was previously saved
           process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/images/" + _images[_currentImageIndex]._imageName));
       else
           process.openUrl(QUrl::fromLocalFile(_images[_currentImageIndex]._tempPath));
   }
}

//====================================================================================

void EventController::openFileDialog()
{
   QFileDialog* import = new QFileDialog(_eventView);
   /*
    * This line is commented below because there is some issue on Linux desktop
    * fileSelected signal is emitted twice
    * So this line possible will resolve this strange issue
    */
   // import->setOption(QFileDialog::DontUseNativeDialog, true);
   connect(import, &QFileDialog::fileSelected, this, &EventController::uploadImage);
   import->setWindowModality(Qt::ApplicationModal);
   import->setAttribute(Qt::WA_DeleteOnClose);
   import->show();
}

//====================================================================================

void EventController::loadImages()
{
   QStringList imagesList = _eventsModel->imagesList(_currentRow);
   for(int i = 0; i != imagesList.size(); i++)
   {
       QString imageName = imagesList[i];
       // qDebug() << imageName;
       QString imagePath = QApplication::applicationDirPath() + "/images/" + imageName;
       qDebug() << imagePath;
       QFile file(imagePath);
       if(file.exists()) // if file exists in folder
       {
           QPixmap loadedPixmap;
           loadedPixmap.load(imagePath);
           QIcon icon(loadedPixmap.scaled(MIN_WIDTH, MIN_HEIGHT, Qt::KeepAspectRatio));
           _eventView->currentImage()->setIcon(icon);
           _images.push_back(Image{loadedPixmap, imageName, QString()});
       }
       else
       {
           QString messageText = "Файла " + imageName + " не существует";
           QMessageBox::critical(_eventView, "Error", messageText, QMessageBox::Ok);
       }
   }
   _currentImageIndex = _images.size() - 1;
}

//====================================================================================

void EventController::uploadImage(const QString& filePath)
{
   if(filePath.indexOf(QRegExp(EXTENSION_PATTERN, Qt::CaseInsensitive)) != INVALID_INDEX) // только .PNG или .JPG/.JPEG
   {
       QFileInfo fullPath(filePath);
       QString imageName = fullPath.fileName();
       QFile file(QApplication::applicationDirPath() + "/images/" + imageName);
       if(!file.exists()) // check if loaded file exists
       {
           QPixmap loadedPixmap;
           loadedPixmap.load(filePath);
           QIcon icon(loadedPixmap.scaled(MIN_WIDTH, MIN_HEIGHT, Qt::KeepAspectRatio));
           _eventView->currentImage()->setIcon(icon);
           _images.push_back(Image{loadedPixmap, imageName, filePath});
           _currentImageIndex = _images.size() - 1;
       }
       else
           QMessageBox::critical(_eventView, "Error", "Файл с таким именем уже есть!");
   }
   else
       QMessageBox::critical(_eventView, "Error", "Неверное изображение!", QMessageBox::Ok);
}

//====================================================================================

void EventController::removeImage()
{
   if(_currentImageIndex >= 0 && _currentImageIndex < _images.size())
   {
       if(_images[_currentImageIndex]._tempPath == QString()) // image was previously saved in database
           _imagesToRemove.push_back(_images[_currentImageIndex]._imageName);
       _images.erase(_images.begin() + _currentImageIndex);
       if(_images.empty())
       {
           _currentImageIndex = INVALID_INDEX;
           QPixmap blankPixmap(MIN_WIDTH, MIN_HEIGHT);
           blankPixmap.fill(Qt::transparent);
           _eventView->currentImage()->setIcon(QIcon(blankPixmap));
       }
       else
       {
           if(_currentImageIndex == _images.size())
               --_currentImageIndex;
           _eventView->currentImage()->setIcon(QIcon(_images[_currentImageIndex]._pixmap));
       }
   }
}

//====================================================================================

void EventController::nextImage()
{
   if((_currentImageIndex + 1) < _images.size())
   {
       ++_currentImageIndex;
       _eventView->currentImage()->setIcon(QIcon(_images[_currentImageIndex]._pixmap));
   }
}

//====================================================================================

void EventController::previousImage()
{
   if((_currentImageIndex - 1) >= 0)
   {
       --_currentImageIndex;
       _eventView->currentImage()->setIcon(QIcon(_images[_currentImageIndex]._pixmap));
   }
}

//====================================================================================

void EventController::saveImages()
{
   for(Image curentImage: _images)
   {
       QFile file(QApplication::applicationDirPath() + "/images/" + curentImage._imageName);
       if(!file.exists())
           curentImage._pixmap.save(QApplication::applicationDirPath() + "/images/" + curentImage._imageName);
   }
}

//====================================================================================

QStringList EventController::getImagesNames()
{
    QStringList imagesList;
    for(Image currentImage: _images)
    {
        imagesList.push_back(currentImage._imageName);
    }
    return imagesList;
}

//====================================================================================

void EventController::removeTemporaryImages()
{
   for(QString imagePath: _imagesToRemove)
   {
       QFile file(QApplication::applicationDirPath() + "/images/" + imagePath);
       if(file.exists())
           file.remove();
   }
}

//====================================================================================
