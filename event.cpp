#include "event.h"
#include "ui_event.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>

#include "eventsproxymodel.h"

Event::Event(EventsProxyModel* eventsModel, int currentRow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Event),
    _eventsModel(eventsModel),
    _currentRow(currentRow)
{
    ui->setupUi(this);

    _widgetMapper = new QDataWidgetMapper;
    _widgetMapper->setModel(_eventsModel);
    _widgetMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    _widgetMapper->addMapping(ui->shortEdit, _eventsModel->column(ShortDescription));
    _widgetMapper->addMapping(ui->fullEdit, _eventsModel->column(LongDescription));
    _widgetMapper->addMapping(ui->extraEdit, _eventsModel->column(ExtraDescription));
    _widgetMapper->addMapping(ui->themeBox, _eventsModel->column(Theme));
    _widgetMapper->addMapping(ui->placeBox, _eventsModel->column(Place));
    _widgetMapper->addMapping(ui->sourceBox, _eventsModel->column(Source));

    if(currentRow != -1)
    {
        ui->dayBox->setCurrentIndex(_eventsModel->day(currentRow));
        ui->monthBox->setCurrentIndex(_eventsModel->month(currentRow));
        ui->yearEdit->setText(QString::number(_eventsModel->year(currentRow)));

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
        ui->themeBox->addItems(themesList);
        ui->placeBox->addItems(placesList);
        ui->sourceBox->addItems(sourcesList);
        _widgetMapper->setCurrentIndex(currentRow);
    }
    else
    {
        _eventsModel->insertRow(_eventsModel->rowCount());
        _widgetMapper->setCurrentIndex(_eventsModel->rowCount()-1);
    }

    loadImages();
}

Event::~Event()
{
    _widgetMapper->revert();
    delete ui;
}

void Event::on_uploadButton_clicked()
{
    QFileDialog* import = new QFileDialog(this);
    /*
     * This line is commented below because there is some issue on Linux desktop
     * fileSelected signal is emitted twice
     * So this line possible will resolve this strange issue
     */
    // import->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(import, &QFileDialog::fileSelected, this, &Event::uploadPhoto);
    import->setWindowModality(Qt::ApplicationModal);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}

void Event::loadImages()
{
    QDir currentDirectory(QApplication::applicationDirPath() + "/images");
    QFileInfoList files = currentDirectory.entryInfoList(QDir::Files);
    qDebug() << files.size();
//    qDebug() << files.size();
    for(QFileInfo file: files)
    {
        qDebug() << file.filePath();
        if(file.filePath().indexOf(QRegExp("(.png)|(.jpg)|(.jpeg)", Qt::CaseInsensitive)) != -1) // только .PNG или .JPG/.JPEG
        {
            QPixmap pixmap;
            pixmap.load(file.filePath());
            QIcon icn(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
            ui->currentPhoto->setIcon(icn);
            images.push_back(Image{pixmap, file.fileName(), ""});
            _currentImageIndex = images.size()-1;
        }
    }
}

void Event::uploadPhoto(QString filePath)
{
    if(filePath.indexOf(QRegExp("(.png)|(.jpg)|(.jpeg)", Qt::CaseInsensitive)) != -1) // только .PNG или .JPG/.JPEG
    {
        QFileInfo fullPath(filePath);
        QString imageName = fullPath.fileName();
        QFile file(QApplication::applicationDirPath() + "/images/" + imageName);
        // check if loaded file has the same name
        if(file.exists())
        {
            QMessageBox::critical(this, "Error", "Файл с таким именем уже есть!");
            return ;
        }
        QPixmap pix;
        pix.load(filePath);
        QIcon icn(pix.scaled(150,150,Qt::KeepAspectRatio));
        ui->currentPhoto->setIcon(icn);
        images.push_back(Image{pix, imageName, filePath});
        _currentImageIndex = images.size()-1;
    }
    else
        QMessageBox::critical(this, "Error", "Неверное изображение!", QMessageBox::Ok);
}

void Event::on_removeImage_clicked()
{
    if(_currentImageIndex >= 0 && _currentImageIndex < images.size())
    {
        if(images[_currentImageIndex]._tempPath == "")
            imagesToRemove.push_back(images[_currentImageIndex]._path);
        images.erase(images.begin() + _currentImageIndex);
        if(images.size() == 0)
        {
            _currentImageIndex = -1;
            QPixmap* pix = new QPixmap(150,150);
            pix->fill(Qt::transparent);
            ui->currentPhoto->setIcon(QIcon(*pix));
        }
        else if(images.size() >= 1 && _currentImageIndex != images.size())
        {
            ui->currentPhoto->setIcon(QIcon(images[_currentImageIndex]._pixmap));
        }
        else if(_currentImageIndex == images.size())
        {
            --_currentImageIndex;
            ui->currentPhoto->setIcon(QIcon(images[_currentImageIndex]._pixmap));
        }
    }
}

void Event::on_nextImage_clicked()
{
    if(_currentImageIndex+1 < images.size())
    {
        ++_currentImageIndex;
        ui->currentPhoto->setIcon(QIcon(images[_currentImageIndex]._pixmap));
    }
}

void Event::on_previousImage_clicked()
{
    if(_currentImageIndex-1 >= 0)
    {
        --_currentImageIndex;
        ui->currentPhoto->setIcon(QIcon(images[_currentImageIndex]._pixmap));
    }
}

void Event::saveImages()
{
    QString imagesList = "";
    for(Image curentImage: images)
    {
        QFile file(QApplication::applicationDirPath() + "/images/" + curentImage._path);
        if(!file.exists())
            curentImage._pixmap.save(QApplication::applicationDirPath() + "/images/" + curentImage._path);
    }
}

void Event::removeImages()
{
    for(QString imagePath: imagesToRemove)
    {
        QFile file(QApplication::applicationDirPath() + "/images/" + imagePath);
        if(file.exists())
            file.remove();
    }
}

void Event::on_saveButton_clicked()
{
    if(_currentRow != -1)
    {
        _eventsModel->setDate(_currentRow,
                              ui->dayBox->currentIndex(),
                              ui->monthBox->currentIndex(),
                              ui->yearEdit->text().toInt());
    }
    _widgetMapper->submit();
//    _eventsModel->submitAll();
    this->close();
}

void Event::on_cancelButton_clicked()
{
    _widgetMapper->revert();
    this->close();
}

void Event::on_currentPhoto_clicked()
{
    if(_currentImageIndex != -1)
    {
        QDesktopServices process;
        if(images[_currentImageIndex]._tempPath == "")
            process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/images/" + images[_currentImageIndex]._path));
        else
            process.openUrl(QUrl::fromLocalFile(images[_currentImageIndex]._tempPath));
    }
}
