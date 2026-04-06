#include "cihmapps5.h"
#include "ui_cihmapps5.h"

CIhmAppS5::CIhmAppS5(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::CIhmAppS5),
    m_capteur(nullptr),
    m_gpio(nullptr),
    m_timer(nullptr)
{
    ui->setupUi(this);

    // Initialisation du capteur et du timer
    m_capteur = new CSht20(this);
    m_timer   = new QTimer(this);
    m_timer->setInterval(2000); // Acquisition toutes les 2 secondes

    // Remplissage combobox GPIO
    QList<int> broches = {4, 5, 6, 12, 13, 16, 17, 18,
                          19, 20, 21, 22, 23, 24, 25, 26, 27};
    for (int b : broches)
        ui->cbGpio->addItem(QString::number(b));
    ui->cbGpio->setCurrentText("22");

    // Connexions explicites
    connect(ui->pbDemarrer,  &QPushButton::clicked,
            this, &CIhmAppS5::slotDemarrerArreter);
    connect(ui->pbActiver,   &QPushButton::clicked,
            this, &CIhmAppS5::slotActiverGpio);
    connect(m_timer,         &QTimer::timeout,
            this, &CIhmAppS5::slotAcquisition);
    connect(m_capteur,       &CSht20::erreur,
            this, &CIhmAppS5::slotErreurCapteur);

    // Etat initial
    ui->pbActiver->setEnabled(true);
    ui->laTemperature->setText("-- °C");
    ui->laHumidite->setText("-- %");
    majAffichageLed(ETAT_ETEINT);
}

CIhmAppS5::~CIhmAppS5()
{
    if (m_timer->isActive()) m_timer->stop();
    delete ui;
}

/*!
 * \brief Slot : demarre ou arrete l'acquisition
 */
void CIhmAppS5::slotDemarrerArreter()
{
    if (m_timer->isActive()) {
        m_timer->stop();
        ui->pbDemarrer->setText("Demarrer acquisition");
        qDebug() << "[IHM] Acquisition arretee.";
    } else {
        m_timer->start();
        ui->pbDemarrer->setText("Arreter acquisition");
        qDebug() << "[IHM] Acquisition demarree.";
    }
}

/*!
 * \brief Slot : initialise la broche GPIO pour la LED
 */
void CIhmAppS5::slotActiverGpio()
{
    if (m_gpio != nullptr) {
        delete m_gpio;
        m_gpio = nullptr;
    }

    int broche = ui->cbGpio->currentText().toInt();
    m_gpio = new CGpio(this, broche, GPIO_OUT);

    connect(m_gpio, &CGpio::erreur,
            this, &CIhmAppS5::slotErreurGpio);

    ui->pbActiver->setEnabled(false);
    qDebug() << "[IHM] GPIO" << broche << "activee.";
}

/*!
 * \brief Slot : lit le capteur et met a jour l'IHM
 *        Gestion de 3 etats selon les seuils min/max :
 *        - En dessous du min  → LED eteinte
 *        - Entre min et max   → LED orange (alerte intermediaire)
 *        - Au dessus du max   → LED allumee (rouge)
 *
 *        Les seuils sont lus en temps reel depuis l'IHM :
 *        pas besoin d'arreter l'acquisition pour les changer.
 */
void CIhmAppS5::slotAcquisition()
{
    float temperature = m_capteur->lireTemperature();
    float humidite    = m_capteur->lireHumidite();

    // Mise a jour affichage
    ui->laTemperature->setText(QString::number(temperature, 'f', 1) + " °C");
    ui->laHumidite->setText(QString::number(humidite, 'f', 1) + " %");

    // Lecture des seuils en direct depuis l'IHM
    float seuilMin = ui->leSeuilMin->text().toFloat();
    float seuilMax = ui->leSeuilMax->text().toFloat();

    if (m_gpio != nullptr) {
        if (temperature >= seuilMax) {
            // Au dessus du max : LED allumee
            m_gpio->ecrire(1);
            majAffichageLed(ETAT_MAX);
        } else if (temperature >= seuilMin) {
            // Entre min et max : LED allumee + alerte orange
            m_gpio->ecrire(1);
            majAffichageLed(ETAT_INTERMEDIAIRE);
        } else {
            // En dessous du min : LED eteinte
            m_gpio->ecrire(0);
            majAffichageLed(ETAT_ETEINT);
        }
    }
}

/*!
 * \brief Met a jour l'affichage de l'etat de la LED
 * \param etat : ETAT_ETEINT, ETAT_INTERMEDIAIRE ou ETAT_MAX
 */
void CIhmAppS5::majAffichageLed(EtatLed etat)
{
    switch (etat) {
    case ETAT_MAX:
        ui->laLed->setText("LED ALLUMEE - Seuil MAX depasse !");
        ui->laLed->setStyleSheet("color: red; font-weight: bold; font-size: 14px;");
        break;
    case ETAT_INTERMEDIAIRE:
        ui->laLed->setText("LED ALLUMEE - Zone intermediaire");
        ui->laLed->setStyleSheet("color: orange; font-weight: bold; font-size: 14px;");
        break;
    case ETAT_ETEINT:
    default:
        ui->laLed->setText("LED ETEINTE - Temperature normale");
        ui->laLed->setStyleSheet("color: green; font-weight: bold; font-size: 14px;");
        break;
    }
}

/*!
 * \brief Slot : affiche une erreur capteur
 */
void CIhmAppS5::slotErreurCapteur(QString msg)
{
    qDebug() << "[IHM] Erreur capteur :" << msg;
    QMessageBox::critical(this, "Erreur capteur", msg);
}

/*!
 * \brief Slot : affiche une erreur GPIO
 */
void CIhmAppS5::slotErreurGpio(QString msg)
{
    qDebug() << "[IHM] Erreur GPIO :" << msg;
    QMessageBox::critical(this, "Erreur GPIO", msg);
}
