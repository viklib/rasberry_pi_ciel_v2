#ifndef CIHMAPPS6_H
#define CIHMAPPS6_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QTcpSocket>       // Communication TCP
#include <QSerialPort>      // Communication RS232 serie
#include <QSerialPortInfo>  // Pour lister les ports series disponibles
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "csht20.h"
#include "cgpio.h"

namespace Ui {
class CIhmAppS6;
}

/*!
 * \brief Enumeration des etats possibles de la LED
 *        Reprise de S5, inchangee
 */
enum EtatLed {
    ETAT_ETEINT,         // Temperature sous le seuil Min → LED eteinte
    ETAT_INTERMEDIAIRE,  // Temperature entre Min et Max  → LED orange
    ETAT_MAX             // Temperature au dessus du Max  → LED rouge
};

/*!
 * \brief Classe CIhmAppS6 — Semaine 6 version 2
 *
 *        Reprise complete de S5 (capteur SHT20, seuils Min/Max,
 *        commande LED via GPIO) avec ajout de DEUX canaux
 *        de communication simultanes :
 *
 *        1. TCP   : envoi vers un serveur reseau (QTcpSocket)
 *        2. RS232 : envoi via le port serie DB9 du shield (QSerialPort)
 *
 *        Les deux canaux envoient la meme trame CSV a chaque acquisition.
 *        Chaque canal est independant : on peut utiliser l'un, l'autre,
 *        ou les deux en meme temps.
 *
 *        Toutes les donnees sont egalement sauvegardees dans un
 *        fichier CSV local horodate sur la Raspberry.
 *
 *        Format trame :
 *        date;heure;temperature_c;humidite_pct;etat_led
 *        Exemple : 15/01/2026;14:32:05;24,5;58,2;NORMAL
 *
 *        Simulation Windows :
 *        - TCP  : fonctionne avec 127.0.0.1 + ncat -l 12345
 *        - RS232: utiliser un port COM virtuel (ex: COM3) ou un
 *                 emulateur comme com0com sous Windows
 */
class CIhmAppS6 : public QMainWindow
{
    Q_OBJECT

public:
    explicit CIhmAppS6(QWidget *parent = nullptr);
    ~CIhmAppS6();

private slots:
    // ── Capteur / GPIO (repris de S5) ──────────────────────
    void slotDemarrerArreter();
    void slotActiverGpio();
    void slotAcquisition();
    void slotErreurCapteur(QString msg);
    void slotErreurGpio(QString msg);

    // ── Communication TCP ───────────────────────────────────
    void slotConnecter();
    void slotTcpConnecte();
    void slotTcpDeconnecte();
    void slotTcpErreur(QAbstractSocket::SocketError erreur);

    // ── Communication RS232 ─────────────────────────────────
    void slotOuvrirFermerSerie();
    void slotScanPorts();
    void slotErreurSerie(QSerialPort::SerialPortError erreur);

private:
    Ui::CIhmAppS6 *ui;

    // ── Objets metier ───────────────────────────────────────
    CSht20        *m_capteur;     // Capteur SHT20 via I2C
    CGpio         *m_gpio;        // LED via sysfs GPIO
    QTimer        *m_timer;       // Timer acquisition 2s

    // ── Canaux de communication ─────────────────────────────
    QTcpSocket    *m_socket;      // Socket TCP client
    QSerialPort   *m_serie;       // Port serie RS232

    // ── Fichier CSV ─────────────────────────────────────────
    QFile         *m_fichierCsv;  // Fichier CSV local horodate

    // ── Compteurs d'envoi ───────────────────────────────────
    int m_nbTramesTcp;    // Nombre de trames envoyees via TCP
    int m_nbTramesSerie;  // Nombre de trames envoyees via RS232
    int m_nbLignesCsv;    // Nombre de lignes ecrites dans le CSV

    // ── Methodes privees ────────────────────────────────────
    void    majAffichageLed(EtatLed etat);
    void    majStatutTcp(bool connecte);
    void    majStatutSerie(bool ouvert);
    void    initialiserCsv();
    void    remplirBaudrates();
    void    envoyerDonnees(float temperature, float humidite, EtatLed etat);
    QString construireTrame(float temperature, float humidite, EtatLed etat);
    QString etatLedVersTexte(EtatLed etat);
};

#endif // CIHMAPPS6_H
