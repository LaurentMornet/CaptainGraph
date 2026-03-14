#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "plot.h"

/**
 * @file graph.h
 * @brief Définition de la classe Graph — conteneur de plusieurs Plot.
 *
 * La classe Graph représente un ensemble cohérent de courbes (plots),
 * avec un titre global et un ensemble dynamique de labels d’axes.
 *
 * Caractéristiques :
 *  - Peut contenir plusieurs Plot de types variés.
 *  - Les labels d’axes (_labels) s’ajustent automatiquement
 *    en fonction du nombre maximal de features présents.
 *  - API sûre avec vérifications de bornes et exceptions explicites.
 *
 * Cible : C++17.
 */
class Graph {
private:
    std::string          _title;        /**< Titre global du graphique. */
    std::vector<std::string> _labels;   /**< Labels des axes (X,Y,Z,...) dynamiques. */
    bool                 _axis_equal;   /**< Indique si les axes doivent être forcés égaux. */
    std::vector<Plot>    _plots;        /**< Ensemble des plots contenus dans ce graphique. */
    static int GRAPH_NUMBER;
public:
    /** Constructeur par défaut. */
    explicit Graph();

    /** Destructeur par défaut. */
    virtual ~Graph() = default;

    /**
     * @brief Copie toutes les propriétés d’un autre graphe.
     * Utilisé par GraphConfigFrame pour synchroniser les modèles.
     */
    void from(const Graph& other);

    /* ======================== Gestion des plots ========================== */

    /**
     * @brief Ajoute un Plot vide au graphique et le retourne.
     * @return Référence vers le Plot ajouté.
     */
    Plot * add_plot(Plot* plot = nullptr);
    void remove_last_plot();
    void remove_plot(Plot* plot);
    /**
     * @brief Retourne le nombre total de plots.
     */
    unsigned short plots_count() const noexcept;

    /**
     * @brief Accès à un plot par index.
     * @param index Index du plot (0..plot_count()-1)
     * @return Référence vers le Plot.
     * @throws std::out_of_range si index invalide.
     */
    Plot& plot(unsigned short index);
    int index_of_plot(Plot * plot);
    /* ======================== Titres et labels =========================== */

    /**
     * @brief Définit le titre global du graphique.
     */
    void set_title(const std::string& title);

    /**
     * @brief Retourne le titre global du graphique.
     */
    std::string title() const noexcept;

    /**
     * @brief Retourne le nombre de labels actuellement utilisés.
     * @return Taille du vecteur de labels.
     */
    unsigned short label_count() const noexcept;

    /**
     * @brief Calcule le nombre maximal de labels nécessaires en fonction
     *        du nombre de features des plots.
     */
    unsigned short max_labels() const noexcept;

    /**
     * @brief Définit le label d’axe par index.
     * @param index Index de l’axe (0..label_count()-1)
     * @param label Texte du label.
     * @throws std::out_of_range si index >= label_count().
     */
    void set_label(unsigned short index, const std::string& label);

    /**
     * @brief Retourne le label d’axe par index.
     * @param index Index de l’axe (0..label_count()-1)
     * @return Copie du label.
     * @throws std::out_of_range si index >= label_count().
     */
    std::string label(unsigned short index) const;

    /**
     * @brief Redimensionne dynamiquement le nombre de labels.
     * @param count Nouveau nombre de labels (0..n)
     * @warning Les anciens labels sont conservés si possible, les nouveaux initialisés à "".
     */
    void resize_labels(unsigned short count);

    /* ========================= Options diverses ========================== */

    /**
     * @brief Définit si les axes doivent être forcés égaux.
     */
    void set_axis_equal(bool equal) noexcept;

    /**
     * @brief Retourne l’état de l’option "axes égaux".
     */
    bool axis_equal() const noexcept;
};
