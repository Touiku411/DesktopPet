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
#include <QTimer>
#include <QScreen>

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


        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this](){
            int nextX = this->x();
            int nextY = this->y();
           
            QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
            int GroundY = screenRect.y() + screenRect.height() - this->height();
            bool isDrag = QGuiApplication::mouseButtons() & Qt::LeftButton;
            bool inAir = (this->y() < GroundY);
            if(isDrag){
                // 滑鼠拖曳中，暫停重力影響
                isFalling = false;
                speedY = 0;
            }
            else if(inAir){
                // 在空中
                isFalling = true;
                speedY += 1; // 重力加速度
                nextY += speedY;
                if(nextY >= GroundY){
                    nextY = GroundY;
                    speedY = 0;
                    isFalling = false;
                }
            }
            else{
                // 在地面上
                isFalling = false;
                speedY = 0;
            }
            if(isWalking && !isFalling){
                nextX += speedX;
                if(nextX <= screenRect.x()){
                    nextX = screenRect.x();
                    speedX = abs(speedX);
                }
                else if(nextX + this->width() >= screenRect.width()){
                    nextX = screenRect.width() - this->width();
                    speedX = -abs(speedX);
                }
            }
            if(this->x() != nextX || this->y() != nextY){
                this->move(nextX, nextY);
            }
        });
        timer->start(16); // ~60 FPS

    }
protected:
    void mousePressEvent(QMouseEvent *event) override{
        if( !isWalking && !isFalling && event->button() == Qt::LeftButton){
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
            if(isFalling){
                event->accept();
                return;
            }
            isWalking = !isWalking;
            if(!isWalking){
                qDebug() << "Stop Walking:" << isWalking;
            }
            else{
                qDebug() << "Walking:" << isWalking;
            }
            
            
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
    QTimer *timer;
    int speedX = 4;
    int speedY = 12;
    bool isWalking = false;
    bool isFalling = false;    
} ;
int main(int argc, char *argv[]){

    qputenv("QT_QPA_PLATFORM", "xcb");//強制X11相容模式

    QApplication app(argc, argv);
    PetWindow pet;
    pet.show();
    return app.exec();
}
#include "main.moc"