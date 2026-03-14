#pragma once

#include <string>
#include <map>
#include <variant>
#include <vector>
#include <optional>
#include <stdexcept>
#include <cassert>
#include <iostream>

class Graph;
// #include "graph.h"

/**
 * @file plot.h
 * @brief Déclarations de la classe Plot et des structures associées.
 *
 * - Guard rails : vérifications de bornes, exceptions expliquées, std::optional pour lectures
 *   susceptibles d'échouer.
 *
 * Cible : C++17 (évite les APIs C++20 non portables).
 */

/**
 * @brief Type sémantique d'un élément de style.
 */
enum StyleItemType {
    BOOL,
    STRING,
    INTEGER,
    DOUBLE,
    COLOR,
    INT_RANGE,
    DOUBLE_RANGE,
    CUSTOM_ICON
};

/**
 * @brief Niveau d'exposition de l'option (UI).
 */
enum StyleItemMode {
    BASIC,
    ADVANCED
};

/**
 * @brief Type utilisé pour les arguments d'un style (bornes, pas, ...).
 * Utilisation : StyleItemArg peut être int ou double.
 */
using StyleItemArg = std::variant<int, double, std::string>;

/**
 * @brief Valeur stockée pour un StyleItem : bool | int | double | std::string
 *
 * Note : on évite std::monostate pour conserver la simplicité.
 */
using StyleItemValue = std::variant<bool, int, double, std::string>;

/**
 * @brief Description d'un élément de style (métadonnées).
 */
struct StyleItem {
    StyleItemType               type;          /**< Type sémantique */
    StyleItemMode               mode;          /**< Mode d'affichage (BASIC/ADVANCED) */
    std::string                 label;         /**< Libellé lisible (UI) */
    std::vector<StyleItemArg>   args;          /**< Arguments (p.ex. min,max,step) */
    StyleItemValue              default_value; /**< Valeur par défaut */
};

/**
 * @brief Métadonnées d'une feature (axe / colonne de données).
 *
 * Actuellement non utilisé partout, fourni pour extension future.
 */
struct PlotFeature {
    bool mandatory{false};
    std::string label;
    std::string datadesc;
};

/**
 * @brief Représentation d'un "plot" — configuration sémantique d'un plot.
 *
 * Fournit :
 *  - titre, type
 *  - noms/valeurs des features (X,Y,Z)
 *  - définitions de style (métadonnées) et valeurs effectives (_styledef)
 *
 * Attention : la grande table `base_plots_registry` contient des Plot "modèles"
 * et est initialisée via load_base_plots_registry().
 */
class Plot {
public:
    /**
     * @brief Charge (initialise) la table statique base_plots_registry.
     *
     * Doit être appelée une fois (p.ex. au démarrage). Lance std::runtime_error
     * si une tentative d'initialiser deux fois la table est détectée.
     */
    static std::map<std::string, std::vector<std::string>> CustomIconTypes;
    static void load_base_plots_registry();

    /** Table de noms (utiles pour UI / sérialisation). */
    static std::vector<std::string> PlotTypeNames;
    static int PLOT_NUMBER;

private:
    /**
     * @brief Registre statique contenant des Plot "de base" (templates).
     *
     * Clef : PlotType -> Plot (template)
     *
     * Note :
     *  - on n'expose pas directement la map pour forcer l'accès via les fonctions fournies,
     *    ce qui permet d'ajouter des vérifications.
     */
    static std::map<std::string, Plot> base_plots_registry;

    /* ----- données d'instance ----- */
    Graph*                                  _graph;
    std::string                             _label;
    std::string                             _type;
    std::vector<std::string>                _features_names;
    std::vector<std::string>                _features_values;

    // _style contient la description (type,label,args,default) pour le plot local.
    // Pour les instances "modèles" (dans base_plots_registry) c'est la source de vérité.
    std::vector<std::string>                _style_keys;
    std::map<std::string, StyleItem>        _style;

    // _styledef contient les valeurs réellement définies pour cette instance.
    std::map<std::string, StyleItemValue>   _styledef;

public:
    /** Constructeur par copie */
    explicit Plot(const Plot& source);
    /** Constructeur par défaut : plot "vide" de type PLOT2D. */
    explicit Plot(Graph * parent_graph=nullptr, std::string type="plot_2d");

    /** Destructeur par défaut (RAII sur conteneurs standards). */
    virtual ~Plot() = default;
    // virtual ~Plot() {
    //     std::cout << "deleting plot " << _label << std::endl;
    // }

    // ----- copie / assignation -----
    /**
     * @brief Copie complète depuis un autre Plot.
     * @param src Référence source (non nulle)
     *
     * Utilise les copies de conteneurs standards. La fonction ne jette pas
     * d'exception intentionnellement mais les opérations de copie peuvent propager.
     */
    void from(const Plot& src);

    /**
     * @brief Copie uniquement le styledef (valeurs de style) depuis src.
     * @param src Référence source (non nulle)
     */
    void from_styledef(const Plot& src);
    void from_styledef(std::map<std::string, StyleItemValue> map);
    /**
     * @brief Réinitialise les valeurs de style (styledef) à l'état vide.
     *
     * Equivalent à _styledef.clear() mais fourni pour API.
     */
    void raz_styledef();

    // ----- getters / setters simples -----
    Graph* graph() const noexcept;
    /**
     * @brief Accès en lecture du titre.
     * @return const std::string& Référence constante vers le titre.
     */
    const std::string& label() const noexcept;

    /**
     * @brief Définit le label.
     * @param label Référence const vers la nouvelle valeur.
     */
    void set_label(const std::string& label);

    /**
     * @brief Retourne le type du plot.
     */
    std::string type() const noexcept;

    /**
     * @brief Définit le type du plot.
     * @param type Nouveau type.
     *
     * @warning Changer le type n'ajuste pas automatiquement _features_names ou _style.
     *          Pour synchroniser, il faut copier depuis base_plots_registry[type].
     */
    void set_type(std::string type) noexcept;

    // ----- style API (lecture/écriture) -----
    std::map<std::string, StyleItemValue> current_styledef() const;
    /**
     * @brief Définit (ou met à jour) une valeur de style pour la clé id.
     * @param id Identifiant de l'élément de style.
     * @param value Valeur à affecter (copie).
     */
    void set_style_item_value(const std::string& id, const StyleItemValue& value);

    /**
     * @brief Récupère la valeur effective d'une clé de style.
     *
     * - Si une valeur est définie dans _styledef, elle est retournée (copie).
     * - Sinon, si la clé existe dans la définition (base_plots_registry pour le type
     *   courant), on retourne la valeur par défaut.
     * - Sinon, on retourne std::nullopt.
     *
     * @param id Identifiant de l'élément de style.
     * @return std::optional<StyleItemValue> Valeur (copie) ou std::nullopt si la clé est inconnue.
     */
    std::optional<StyleItemValue> get_style_item_value(const std::string& id) const noexcept;

    /**
     * @brief Récupère le StyleItem (métadonnée) correspondant à `id` dans le modèle
     *        du type courant.
     *
     * @param id Identifiant de l'élément de style.
     * @return const StyleItem& Référence vers le StyleItem dans la table modèle.
     * @throws std::out_of_range si la clé n'existe pas pour le type courant.
     *
     * Attention : la référence est vers la donnée contenue dans la table statique.
     */
    const StyleItem& get_style_item(const std::string& id) const;

    /**
     * @brief Retourne la liste des clés de style applicables à ce plot.
     * @return std::vector<std::string> Copie contrôlée des clés.
     */
    std::vector<std::string> get_style_keys() const;

    /**
     * @brief Récupère les arguments (args) d'un StyleItem (copie).
     * @param id Identifiant de l'élément de style.
     * @return std::optional<std::vector<StyleItemArg>> Liste d'arguments ou std::nullopt si la clé inconnue.
     */
    std::optional<std::vector<StyleItemArg>> get_style_item_args(const std::string& id) const noexcept;

    // ----- features (X, Y, Z) -----
    /**
     * @brief Nom d'une feature (p.ex. "X", "Y", "Z") par index.
     * @param index Index (0..features_count()-1)
     * @return const std::string& Référence constante vers le nom.
     * @throws std::out_of_range si index invalide.
     */
    const std::string& feature_name(size_t index) const;

    /**
     * @brief Accède (et éventuellement étend) la valeur d'une feature par index.
     * @param index Index (0..features_count()-1)
     * @return std::string& Référence vers la valeur (modifiable).
     *
     * Si la taille interne de _features_values est inférieure, la méthode étend le vecteur
     * pour garantir l'accès. Cette méthode ne jette pas d'exception pour l'extension, mais
     * jette std::out_of_range si index >= features_count().
     */
    std::string& feature_value(size_t index);

    /**
     * @brief Définit la valeur d'une feature (copie).
     * @param index Index (0..features_count()-1)
     * @param value Nouvelle valeur (copie).
     * @throws std::out_of_range si index invalide.
     */
    void set_feature_value(size_t index, const std::string& value);

    /**
     * @brief Nombre de features attendues pour le type courant (p.ex. 2 pour 2D).
     */
    size_t features_count() const noexcept;

    // ----- utilitaires statiques -----
    /**
     * @brief Accès à un model Plot depuis la table statique.
     * @param type Type du modèle.
     * @return const Plot& Référence vers l'entrée du registre.
     * @throws std::out_of_range si le registre n'a pas été initialisé ou si la clef est absente.
     */
    static const Plot& model_for_type(std::string type);

private:
    // helpers privés
    static bool base_registry_initialized() noexcept;
};

