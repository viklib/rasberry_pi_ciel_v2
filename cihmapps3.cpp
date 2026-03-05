#include "cihmapps3.h"
#include "ui_cihmapps3.h"

CIhmAppS3::CIhmAppS3(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::CIhmAppS3),
    m_compteur(0)
{
    ui->setupUi(this);

    // Initialisation du timer
    m_timer = new QTimer(this);

    // Connexions explicites
    connect(ui->pbDemarrer, &QPushButton::clicked,
            this, &CIhmAppS3::slotDemarrerArreter);
    connect(m_timer, &QTimer::timeout,
            this, &CIhmAppS3::slotTimeout);

    // Etat initial
    ui->laCompteur->setText("0");
    ui->laEtat->setText("Timer arrete");
    ui->laEtat->setStyleSheet("color: red; font-weight: bold;");
}

CIhmAppS3::~CIhmAppS3()
{
    delete ui;
}

/*!
 * \brief Slot : demarre ou arrete le timer
 *        selon son etat actuel
 */
void CIhmAppS3::slotDemarrerArreter()
{
    if (m_timer->isActive()) {
        // Timer en cours -> on arrete
        m_timer->stop();
        ui->pbDemarrer->setText("Demarrer");
        ui->lePeriode->setEnabled(true);
        ui->laEtat->setText("Timer arrete");
        ui->laEtat->setStyleSheet("color: red; font-weight: bold;");
    } else {
        // Timer arrete -> on demarre
        int periode = ui->lePeriode->text().toInt();

        // Verification que la periode est valide
        if (periode <= 0) {
            ui->laEtat->setText("Periode invalide !");
            ui->laEtat->setStyleSheet("color: orange; font-weight: bold;");
            return;
        }

        m_timer->setInterval(periode);
        m_timer->start();

        ui->pbDemarrer->setText("Arreter");
        ui->lePeriode->setEnabled(false);
        ui->laEtat->setText("En cours...");
        ui->laEtat->setStyleSheet("color: green; font-weight: bold;");
    }
}

/*!
 * \brief Slot : appele a chaque declenchement du timer
 *        Incremente le compteur et met a jour l'IHM
 */
void CIhmAppS3::slotTimeout()
{
    m_compteur++;
    ui->laCompteur->setText(QString::number(m_compteur));
}
