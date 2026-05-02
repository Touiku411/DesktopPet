#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWindow>
#include <QDebug>
#include <QMenu>
#include <QContextMenuEvent>

class PetWindow : public QWidget {
    Q_OBJECT
public:
    PetWindow(QWidget *parent =  nullptr) : QWidget(parent){
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool );
        
        setAttribute(Qt::WA_TranslucentBackground);
        QLabel *petLabel = new QLabel(this);
        movie = new QMovie(":/gif/firefly.gif");
        movie->setScaledSize(QSize(150,200));
        petLabel->setMovie(movie);
        movie->start();

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(petLabel);
        // setFixedSize(movie->frameRect().size());
        setFixedSize(150,200);
    }
protected:
    void mousePressEvent(QMouseEvent *event) override{
        if(event->button() == Qt::LeftButton){
            if(windowHandle()){
                windowHandle()->startSystemMove();
            }
            event->accept();
        }
        // if(event->button() == Qt::RightButton){
        //     this->close();
        // }
        QWidget::mousePressEvent(event);
    }
    void mouseDoubleClickEvent(QMouseEvent *event)override{
        if(event->button() == Qt::LeftButton){
            qDebug() << "好痛痛!";
            event->accept();
        }
    }
    void contextMenuEvent(QContextMenuEvent *event)override{
        QMenu menu(this);
        menu.addAction("Close", qApp, &QApplication::quit);
        menu.exec(event->globalPos());
        // menu.exec(event->globalPosition().toPoint());
    }
    
private:
    QMovie *movie;
} ;
int main(int argc, char *argv[]){

    qputenv("QT_QPA_PLATFORM", "xcb");//強制X11相容模式

    QApplication app(argc, argv);
    PetWindow pet;
    pet.show();
    return app.exec();
}
#include "main.moc"