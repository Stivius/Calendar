#include "eventcontroller.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>

#include "model/eventssqlmodel.h"
#include "model/settingssqlmodel.h"
#include "view/eventview.h"

//====================================================================================

const int INVALID_INDEX = -1;
const int MIN_WIDTH = 150;
const int MIN_HEIGHT = 150;
const QString EXTENSION_PATTERN = "(.png)|(.jpg)|(.jpeg)";

//====================================================================================

EventController::EventController(EventView* eventView,
                                 std::shared_ptr<EventsSqlModel>& eventsModel,
                                 std::shared_ptr<SettingsSqlModel>& settingsModel,
                                 int currentRow,
                                 QObject* parent) :
    QObject(parent),
    _eventView(eventView),
    _eventsModel(eventsModel),
    _settingsModel(settingsModel),
    _currentRow(currentRow)
{
    connect(_eventView, &EventView::saveBtnClicked, this, &EventController::saveEvent);
    connect(_eventView, &EventView::cancelBtnClicked, this, &EventController::cancelSaving);

    connect(_eventView, &EventView::uploadBtnClicked, this, &EventController::openFileDialog);
    connect(_eventView, &EventView::removeBtnClicked, this, &EventController::removeImage);
    connect(_eventView, &EventView::nextBtnClicked, this, &EventController::nextImage);
    connect(_eventView, &EventView::previousBtnClicked, this, &EventController::previousImage);
    connect(_eventView, &EventView::currentImageClicked, this, &EventController::openCurrentImage);

    QSet<QString> themes = {""}, places = {""}, sources = {""};
    for(int i = 0; i != _eventsModel->rowCount(); i++)
    {
        themes.insert(_eventsModel->theme(i));
        places.insert(_eventsModel->place(i));
        sources.insert(_eventsModel->source(i));
    }
    _eventView->addThemes(themes.toList());
    _eventView->addPlaces(places.toList());
    _eventView->addSources(sources.toList());

    if(_currentRow != INVALID_INDEX)
    {
        _eventView->setDay(_eventsModel->day(_currentRow));
        _eventView->setMonth(_eventsModel->month(_currentRow));
        _eventView->setYear(QString::number(_eventsModel->year(_currentRow)));

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

    _eventView->setMapperModel(_eventsModel.get());
    _eventView->setMapperIndex(_currentRow);
}

//====================================================================================

EventController::~EventController()
{
    _eventsModel->revertAll();
    qDebug() << "event controller deleted";
}

//====================================================================================

void EventController::saveEvent()
{
    bool result = _eventsModel->setDate(_currentRow,
                                        _eventView->selectedDay(),
                                        _eventView->selectedMonth(),
                                        _eventView->selectedYear());
    if(result)
    {
        removeTemporaryImages();
        saveImages();
        _eventsModel->setImagesList(_currentRow, getImagesNames());
        _eventsModel->submitAll();
        _eventView->close();
    }
    else
        QMessageBox::critical(_eventView, "Error", tr("Дата неверна!"), QMessageBox::Ok);
}

//====================================================================================

void EventController::cancelSaving()
{
    _eventsModel->revertAll();
    _eventView->close();
}

//====================================================================================

void EventController::openCurrentImage()
{
   if(_currentImageIndex != INVALID_INDEX)
   {
       QDesktopServices process;
       if(_images[_currentImageIndex]._tempPath == QString()) // images was previously saved
           process.openUrl(QUrl::fromLocalFile(_settingsModel->imagesFolder() + _images[_currentImageIndex]._imageName));
       else
           process.openUrl(QUrl::fromLocalFile(_images[_currentImageIndex]._tempPath));
   }
}

//====================================================================================

void EventController::openFileDialog()
{
   auto import = new QFileDialog(_eventView);
   // import->setOption(QFileDialog::DontUseNativeDialog, true);
   connect(import, &QFileDialog::fileSelected, this, &EventController::uploadImage);
   import->setWindowModality(Qt::ApplicationModal);
   import->setNameFilter("*.png *.jpg *.jpeg");
   import->setAttribute(Qt::WA_DeleteOnClose);
   import->show();
}

//====================================================================================

void EventController::loadImages()
{
   QStringList imagesList = _eventsModel->imagesList(_currentRow);
   for(const QString& imageName: imagesList)
   {
       QString imagePath = _settingsModel->imagesFolder() + imageName;
       QFile file(imagePath);
       if(file.exists()) // if file exists in folder
       {
           QPixmap loadedPixmap;
           loadedPixmap.load(imagePath);
           QPixmap scaledPixmap = loadedPixmap.scaled(MIN_WIDTH, MIN_HEIGHT, Qt::KeepAspectRatio);
           _eventView->setCurrentImage(scaledPixmap);
           _images.push_back(Image{loadedPixmap, imageName, QString()});
       }
       else
       {
           QString messageText = tr("Файла %1 не существует!");
           QMessageBox::critical(_eventView, "Error", messageText.arg(imageName), QMessageBox::Ok);
       }
   }
   _currentImageIndex = _images.size() - 1;
}

//====================================================================================

void EventController::uploadImage(const QString& filePath)
{
    QFileInfo fullPath(filePath);
    QString imageName = fullPath.fileName();
    QFile file(_settingsModel->imagesFolder() + imageName);
    if(!file.exists()) // check if loaded file exists
    {
       QPixmap loadedPixmap;
       loadedPixmap.load(filePath);
       QPixmap scaledPixmap = loadedPixmap.scaled(MIN_WIDTH, MIN_HEIGHT, Qt::KeepAspectRatio);
       _eventView->setCurrentImage(scaledPixmap);
       _images.push_back(Image{loadedPixmap, imageName, filePath});
       _currentImageIndex = _images.size() - 1;
    }
    else
       QMessageBox::critical(_eventView, "Error", tr("Файл с таким именем уже есть!"));
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
           _eventView->setCurrentImage(blankPixmap);
       }
       else
       {
           if(_currentImageIndex == _images.size())
               --_currentImageIndex;
           _eventView->setCurrentImage(_images[_currentImageIndex]._pixmap);
       }
   }
}

//====================================================================================

void EventController::nextImage()
{
   if((_currentImageIndex + 1) < _images.size())
   {
       ++_currentImageIndex;
       _eventView->setCurrentImage(_images[_currentImageIndex]._pixmap);
   }
}

//====================================================================================

void EventController::previousImage()
{
   if((_currentImageIndex - 1) >= 0)
   {
       --_currentImageIndex;
       _eventView->setCurrentImage(_images[_currentImageIndex]._pixmap);
   }
}

//====================================================================================

void EventController::saveImages()
{
   for(const Image& curentImage: _images)
   {
       QFile file(_settingsModel->imagesFolder() + curentImage._imageName);
       if(!file.exists())
           curentImage._pixmap.save(_settingsModel->imagesFolder() + curentImage._imageName);
   }
}

//====================================================================================

QStringList EventController::getImagesNames()
{
    QStringList imagesList;
    for(const Image& currentImage: _images)
    {
        imagesList.push_back(currentImage._imageName);
    }
    return imagesList;
}

//====================================================================================

void EventController::removeTemporaryImages()
{
   for(const QString& imagePath: _imagesToRemove)
   {
       QFile file(_settingsModel->imagesFolder() + imagePath);
       if(file.exists())
           file.remove();
   }
}

//====================================================================================
