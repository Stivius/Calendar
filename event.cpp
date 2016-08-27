#include "event.h"
#include "ui_event.h"

Event::Event(Model* _model, QWidget *parent, int _row) :
    QDialog(parent),
    ui(new Ui::Event),
    model(_model),
    row(_row)
{
    ui->setupUi(this);
    QStringList themesList = {""}, placesList = {""}, sourcesList = {""};
    for(int i = 0; i != model->count(); i++)
    {
        themesList.push_back(model->getTheme(i));
        placesList.push_back(model->getPlace(i));
        sourcesList.push_back(model->getSource(i));
    }
    themesList.removeDuplicates();
    placesList.removeDuplicates();
    sourcesList.removeDuplicates();
    ui->themeBox->addItems(themesList);
    ui->placeBox->addItems(placesList);
    ui->sourceBox->addItems(sourcesList);
    currentImage = -1;
    if(row != -1)
    {
        ui->extraEdit->setText(model->getExtra(row));
        ui->fullEdit->setPlainText(model->getLDescrpition(row));
        ui->shortEdit->setText(model->getSDescrpition(row));
        ui->yearEdit->setText(QString::number(model->getYear(row)));
        int dayInd = ui->dayBox->findText(QString::number(model->getDay(row)));
        if(dayInd == -1)
            ui->dayBox->setCurrentIndex(0);
        else
            ui->dayBox->setCurrentIndex(dayInd);
        int monthInd = ui->monthBox->findText(model->getMonthName(model->getMonth(row))); //
        ui->monthBox->setCurrentIndex(monthInd);
        int themeInd = ui->themeBox->findText(model->getTheme(row));
        ui->themeBox->setCurrentIndex(themeInd);
        int placeInd = ui->placeBox->findText(model->getPlace(row));
        ui->placeBox->setCurrentIndex(placeInd);
        int sourceInd = ui->sourceBox->findText(model->getSource(row));
        ui->sourceBox->setCurrentIndex(sourceInd);
        QString images = model->getImages(row);
        QStringList imagesList = images.split(QChar('\n'), QString::SkipEmptyParts);
        loadImages(imagesList);
    }

    connect(this, SIGNAL(addEvent(const QString&, const QString&, const QString&, const QString&, const QString&)),  parent, SLOT(addEvent(const QString&, const QString&, const QString&, const QString&, const QString&)));
    connect(this, SIGNAL(updateEvent(int,const QString&, const QString&, const QString&, const QString&, const QString&)), parent, SLOT(updateEvent(int,const QString&, const QString&, const QString&, const QString&, const QString&)));
}

Event::~Event()
{
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
    connect(import,SIGNAL(fileSelected(QString)),SLOT(uploadedPhoto(QString)));
    import->setWindowModality(Qt::ApplicationModal);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}

void Event::loadImages(const QStringList& imagesList)
{
    qDebug() << imagesList.size();
    for(QString path: imagesList)
    {
        QPixmap pix;
        pix.load(QApplication::applicationDirPath() + "/images/" + path);
        QIcon icn(pix.scaled(150,150,Qt::KeepAspectRatio));
        ui->currentPhoto->setIcon(icn);
        images.push_back(Image{pix, path, ""});
        currentImage = images.size()-1;
    }
}

void Event::uploadedPhoto(QString& filePath)
{
    if(filePath.indexOf(QRegExp("(.png)|(.jpg)|(.jpeg)",Qt::CaseInsensitive)) != -1) // только .PNG или .JPG/.JPEG
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
        currentImage = images.size()-1;
    }
    else
        QMessageBox::critical(this, "Error", "Неверное изображение!", QMessageBox::Ok);
}

void Event::on_removeImage_clicked()
{
    if(currentImage >= 0 && currentImage < images.size())
    {
        if(images[currentImage].tempPath == "")
            removed.push_back(images[currentImage].path);
        images.erase(images.begin() + currentImage);
        if(images.size() == 0)
        {
            currentImage = -1;
            QPixmap* pix = new QPixmap(150,150);
            pix->fill(Qt::transparent);
            ui->currentPhoto->setIcon(QIcon(*pix));
        }
        else if(images.size() >= 1 && currentImage != images.size())
        {
            ui->currentPhoto->setIcon(QIcon(images[currentImage].image));
        }
        else if(currentImage == images.size())
        {
            --currentImage;
            ui->currentPhoto->setIcon(QIcon(images[currentImage].image));
        }
    }
}

void Event::on_nextImage_clicked()
{
    if(currentImage+1 < images.size())
    {
        ++currentImage;
        ui->currentPhoto->setIcon(QIcon(images[currentImage].image));
    }
}

void Event::on_previousImage_clicked()
{
    if(currentImage-1 >= 0)
    {
        --currentImage;
        ui->currentPhoto->setIcon(QIcon(images[currentImage].image));
    }
}

QString Event::getSavedImages()
{
    QString imagesList = "";
    for(int i = 0; i != images.size(); i++)
    {
        QFile file(QApplication::applicationDirPath() + "/images/" + images[i].path);
        if(!file.exists())
            images[i].image.save(QApplication::applicationDirPath() + "/images/" + images[i].path);
        imagesList += images[i].path + "\n";
    }
    return imagesList;
}

void Event::removeImages()
{
    for(int i = 0; i != removed.size(); i++)
    {
        QFile file(QApplication::applicationDirPath() + "/images/" + removed[i]);
        if(file.exists())
            file.remove();
    }
}

void Event::on_saveButton_clicked()
{
    QString imagesList = getSavedImages();
    removeImages();
    QVector<QString> data = {ui->dayBox->currentText(),QString::number(model->getMonth(ui->monthBox->currentText())),ui->yearEdit->text(),
                             ui->themeBox->currentText(), ui->shortEdit->text(), ui->fullEdit->toPlainText(),
                             ui->placeBox->currentText(), ui->sourceBox->currentText(), ui->extraEdit->text(), imagesList};
    QString date = model->formatDate(ui->dayBox->currentText().toInt(), model->getMonth(ui->monthBox->currentText()), ui->yearEdit->text().toInt());
    if(row == -1)
    {
        model->insertEvent(data);
        emit addEvent(date, ui->shortEdit->text(), ui->placeBox->currentText(), ui->sourceBox->currentText(), (images.size() > 0) ? "Есть" : "Нет");
    }
    else
    {
        model->updateEvent(row, data);
        emit updateEvent(row, date, ui->shortEdit->text(), ui->placeBox->currentText(), ui->sourceBox->currentText(), (images.size() > 0) ? "Есть" : "Нет");
    }
    this->close();
}

void Event::on_cancelButton_clicked()
{
    this->close();
}

void Event::on_currentPhoto_clicked()
{
    if(currentImage != -1)
    {
        QDesktopServices process;
        if(images[currentImage].tempPath == "")
            process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/images/" + images[currentImage].path));
        else
            process.openUrl(QUrl::fromLocalFile(images[currentImage].tempPath));
    }
}
