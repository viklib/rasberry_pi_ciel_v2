#ifndef CIHMAPPS5_H
#define CIHMAPPS5_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "csht20.h"
#include "cgpio.h"

namespace Ui {
class CIhmAppS5;
}

/*!
 * \brief Enumeration des etats possibles de la LED
 */
enum EtatLed {
    ETAT_ETEINT,         // Temperature en dessous du seuil min
    ETAT_INTERMEDIAIRE,  // Temperature entre seuil min et max
    ETAT_MAX             // Temperature au dessus du seuil max
};

/*!
 * \brief Classe CIhmAppS5
 *        Semaine 5 : affichage temperature/humidite
 *        depuis le capteur SHT20 via I2C.
 *        LED commandee automatiquement selon seuils min/max.
 *        Les seuils sont modifiables en temps reel sans relancer.
 */
class CIhmAppS5 : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhmAppS5(QWidget *parent = nullptr);
    ~CIhmAppS5();

private slots:
    void slotDemarrerArreter();
    void slotActiverGpio();
    void slotAcquisition();
    void slotErreurCapteur(QString msg);
    void slotErreurGpio(QString msg);

private:
    Ui::CIhmAppS5 *ui;
    CSht20        *m_capteur;
    CGpio         *m_gpio;
    QTimer        *m_timer;

    void majAffichageLed(EtatLed etat);
};

#endif // CIHMAPPS5_H
