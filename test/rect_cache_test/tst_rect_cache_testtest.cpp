#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QRectF>

class Rect_cache_testTest : public QObject
{
    Q_OBJECT

public:
    Rect_cache_testTest();

private Q_SLOTS:
    void cacheTest();
    void calculateTest();
};

Rect_cache_testTest::Rect_cache_testTest()
{
}

QRectF getCalculateRect()
{
    return QRectF(10,10,100,100).normalized().adjusted(-3,-3,3,3);
}

QRectF getCachedRect() {
    static QRectF cachedRect(10,10,100,100);
    return cachedRect;
}

void Rect_cache_testTest::calculateTest()
{
    QBENCHMARK {
        QRectF r = getCalculateRect();
    }
}

void Rect_cache_testTest::cacheTest()
{
    QBENCHMARK {
        QRectF r = getCachedRect();
    }
}

QTEST_APPLESS_MAIN(Rect_cache_testTest);

#include "tst_rect_cache_testtest.moc"
