# Semaine 3 — QTimer et acquisition périodique 🦝
 
> Projet pédagogique | BTS CIEL | Qt / C++ / Raspberry Pi

---

## Objectif

Troisième semaine de formation Qt. L'objectif est de maîtriser la classe **QTimer** pour exécuter des actions à intervalle régulier. Cette notion est fondamentale dans les systèmes embarqués pour l'acquisition périodique de données issues de capteurs.

---

## Ce que fait l'application

L'utilisateur règle une période en millisecondes, démarre le timer et observe un compteur s'incrémenter automatiquement à chaque déclenchement. Le bouton bascule entre **Démarrer** et **Arrêter** selon l'état du timer.

```
┌─────────────────────────────────────────┐
│          Semaine 3 - QTimer             │
│                                         │
│      Période (ms) : [ 1000 ]            │
│                                         │
│            [ Démarrer ]                 │
│                                         │
│     Compteur :     42                   │
│     Etat :         En cours...          │
│                                         │
└─────────────────────────────────────────┘
```

---

## Structure du projet

```
appS3/
├── CMakeLists.txt       # Configuration du projet Qt
├── main.cpp             # Point d'entrée de l'application
├── cihmapps3.h          # Déclaration de la classe IHM
├── cihmapps3.cpp        # Implémentation de la classe IHM
└── cihmapps3.ui         # Interface graphique (Qt Designer)
```

---

## Notions abordées

| Notion | Description |
|--------|-------------|
| QTimer | Déclenchement d'actions à intervalle régulier |
| `isActive()` | Vérifie si le timer est en cours d'exécution |
| `setInterval()` | Définit la période en millisecondes |
| Signal `timeout()` | Émis automatiquement à chaque fin de période |
| Bouton bascule | Un seul bouton qui change de rôle selon l'état |
| `setEnabled()` | Activation / désactivation dynamique d'un widget |
| `setStyleSheet()` | Mise en forme dynamique des labels |

---

## Widgets utilisés

| Widget | Nom (objectName) | Rôle |
|--------|-----------------|------|
| `QLabel` | `laTitre` | Titre de la fenêtre |
| `QLineEdit` | `lePeriode` | Saisie de la période en ms |
| `QPushButton` | `pbDemarrer` | Démarre / Arrête le timer |
| `QLabel` | `laCompteur` | Affiche la valeur du compteur |
| `QLabel` | `laEtat` | Affiche l'état du timer |

---

## Connexions signaux/slots

```cpp
// Bouton démarrer/arrêter
connect(ui->pbDemarrer, &QPushButton::clicked,
        this, &CIhmAppS3::slotDemarrerArreter);

// Timer -> slot d'acquisition
connect(m_timer, &QTimer::timeout,
        this, &CIhmAppS3::slotTimeout);
```

---

## Logique du bouton bascule

```cpp
if (m_timer->isActive()) {
    m_timer->stop();
    ui->pbDemarrer->setText("Demarrer");
    ui->lePeriode->setEnabled(true);   // On peut modifier la période
} else {
    m_timer->setInterval(periode);
    m_timer->start();
    ui->pbDemarrer->setText("Arreter");
    ui->lePeriode->setEnabled(false);  // Période verrouillée pendant le timer
}
```

---

## Améliorations apportées avec l'aide de Claude (IA)

Certaines bonnes pratiques ont été suggérées par l'IA **Claude** (Anthropic) lors du développement :

- **Connexions explicites Qt5** — utilisation de pointeurs de fonctions au lieu du nommage automatique `on_foo_bar()`, évitant les doubles connexions silencieuses et les avertissements de l'analyseur Clazy
- **Vérification de la période** — ajout d'un contrôle sur la valeur saisie avant de lancer le timer, pour éviter un comportement indéfini avec une période nulle ou négative
- **Désactivation du champ période pendant l'exécution** — verrouillage du `QLineEdit` via `setEnabled(false)` pendant que le timer tourne, pour éviter une modification en cours d'acquisition

---

## Compilation et exécution

### Sur Raspberry Pi (Linux)
```bash
git clone https://github.com/viklib/rasberry_pi_ciel.git
cd rasberry_pi_ciel
git checkout Semaine_3
cmake .
make
./appS3
```

### Sous Windows (Qt Creator)
Ouvrir `CMakeLists.txt` dans Qt Creator puis `Ctrl+R`.

---

## Progression du projet

| Semaine | Contenu | Statut |
|---------|---------|--------|
| S1 | Découverte Qt Creator, première fenêtre | ✅ Terminé |
| S2 | IHM complète, signaux/slots | ✅ Terminé |
| S3 | QTimer, acquisition périodique | ✅ Terminé |
| S4 | GPIO, commande LED | ⏳ A venir |
| S5 | Capteur I2C SHT20 | ⏳ A venir |
| S6 | Communication TCP / Série | ⏳ A venir |
| S7 | Structuration logicielle | ⏳ A venir |
| S8 | Finalisation + Soutenance | ⏳ A venir |

---

*BTS CIEL — 2025/2026* 🦝
