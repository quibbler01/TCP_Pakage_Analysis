#include <QMainWindow>

namespace Ui {
class chooseDevice;
}

class chooseDevice : public QMainWindow
{
    Q_OBJECT

public:
    explicit chooseDevice(QWidget *parent = nullptr);
    ~chooseDevice();
    void appendItem(QString string);
    void ResetComoBox();
public slots:
    void on_devicepushButton_clicked();
    void on_pushButton_clicked();
private:
    Ui::chooseDevice *ui;
signals:
    void startListener(int);
};
