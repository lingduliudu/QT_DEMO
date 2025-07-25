#ifndef TOAST_H
#define TOAST_H
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QFrame>

class Toast : public QWidget {
    Q_OBJECT

public:
    static void showText(QWidget *parent, const QString &text, int duration = 2000) {
        Toast *toast = new Toast(1,parent, text);
        toast->show();

        // 定时关闭
        QTimer::singleShot(duration, toast, &Toast::deleteLater);
    }
    static void showText(int x,QWidget *parent, const QString &text, int duration = 2000) {
        Toast *toast = new Toast(x,parent, text);
        toast->show();

        // 定时关闭
        QTimer::singleShot(duration, toast, &Toast::deleteLater);
    }

private:
    explicit Toast(int toastNumber,QWidget *parent, const QString &text) : QWidget(parent) {
        setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_DeleteOnClose);

        QLabel *label = new QLabel(text);
        label->setStyleSheet(R"(
            background-color: rgba(50, 50, 50, 220);
            color: white;
            padding: 10px 15px;
            border-radius: 8px;
            font-size: 14px;
        )");
        label->setAlignment(Qt::AlignCenter);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(label);
        setLayout(layout);

        // 设置位置（居中于父窗口底部）
        if (parent) {
            QPoint pos = parent->mapToGlobal(QPoint(0, 0));
            int x = pos.x() + (parent->width() - width()) / 2;
            int y = pos.y() + parent->height() - height() - 50*toastNumber;
            move(x, y);
        }

        adjustSize(); // 根据内容调整大小
    }
};
#endif // TOAST_H
