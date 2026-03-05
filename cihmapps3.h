#ifndef CIHMAPPS3_H
#define CIHMAPPS3_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class CIhmAppS3;
}

/*!
 * \brief Classe CIhmAppS3
 *        Semaine 3 : utilisation de QTimer pour
 *        effectuer des actions a intervalle regulier.
 */
class CIhmAppS3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhmAppS3(QWidget *parent = nullptr);
    ~CIhmAppS3();

private slots:
    void slotDemarrerArreter();
    void slotTimeout();

private:
    Ui::CIhmAppS3 *ui;
    QTimer        *m_timer;
    int            m_compteur;
};

#endif // CIHMAPPS3_H
