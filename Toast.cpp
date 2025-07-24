#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QFrame>
#include "Toast.h"

static void Toast::showText(QWidget *parent, const QString &text, int duration = 2000) {
        Toast *toast = new Toast(parent, text);
        toast->show();

        // 定时关闭
        QTimer::singleShot(duration, toast, &Toast::deleteLater);
    }


    explicit Toast::Toast(QWidget *parent, const QString &text) : QWidget(parent) {
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
            int y = pos.y() + parent->height() - height() - 50;
            move(x, y);
        }

        adjustSize(); // 根据内容调整大小
    }
