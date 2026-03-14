#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <functional> // pour std::reference_wrapper dans std::optional

#include "configuration.h"
class Configuration;

// Type de disposition possible
enum class LayoutType {
    Node = 0,        // Feuille : zone sans subdivision
    Horizontal = 1,  // Découpage horizontal (colonnes)
    Vertical = 2     // Découpage vertical (lignes)
};
enum class MergeDirection {
    Top = 0,
    Right = 1,
    Bottom = 2,
    Left = 3
};
/**
 * Classe Layout : représente un nœud (ou sous-nœud) d’une arborescence de disposition.
 * Chaque Layout peut contenir plusieurs sous-layouts et des poids relatifs.
 */
class Layout {
private:
    static int          __global__id__;                     // Générateur d'ID unique global
    Configuration*      _configuration;                     // A quelle configuration appartient ce layout
    Layout*             _ancestor;                          // Pointeur vers le parent (non possédant)
    std::string         _id;                                // Identifiant du layout
    LayoutType          _type;                              // Type du layout (NODE, HORIZONTAL, VERTICAL)
    std::vector<std::unique_ptr<Layout>> _sublayouts;       // Sous-layouts (possession unique)
    std::vector<double> _weights;                           // Poids relatifs des sous-layouts

public:
    // --- CONSTRUCTION / DESTRUCTION ---
    explicit Layout(Configuration* configuration, Layout* ancestor, LayoutType type, const std::string& id = "");
    ~Layout() = default;

    // Empêcher la copie
    Layout(const Layout&) = delete;
    Layout& operator=(const Layout&) = delete;

    // Autoriser le déplacement
    Layout(Layout&&) noexcept = default;
    Layout& operator=(Layout&&) noexcept = default;

    // --- MÉTHODES DE MODIFICATION ---
    void split(LayoutType type);          // Scinde un nœud en sous-nœuds d'un certain type
    void merge(const std::string& id);    // Fusionne deux nœuds frères contigus
    void merge(MergeDirection direction);

    std::vector<std::string> siblings() const;
    bool can_merge(MergeDirection direction);

    // --- ACCÈS ET RECHERCHE ---
    Layout* root();
    Configuration* configuration();
    unsigned int get_index();
    Graph* graph(const std::string& id);
    LayoutType type() const;
    void set_type(const LayoutType type);
    double weight();
    void set_weight(double new_weight);
    std::optional<std::reference_wrapper<Layout>> get(const std::string& id); // Recherche récursive d'un layout par ID
    int index_of(const Layout* layout) const;                                 // Retourne l'index d'un sous-layout dans _sublayouts

    std::map<std::string, double[4]> nodes_bounds(bool include_all=false);
    void nodes_bounds(std::map<std::string, double[4]>& buffer, double x=0, double y=0, double width=0, double height=0, bool include_all=false);

    // --- DEBUG ---
    void dump(int depth = 0) const;      // Affiche la structure hiérarchique pour le debug
    void render_html(std::ofstream& out, double x, double y, double width, double height) const;
    void draw_html(const std::string& filename, double width, double height) const;

    const std::vector<std::unique_ptr<Layout>>& sublayouts() const;
    const std::vector<double>& weights() const;
    std::vector<double> absolute_weights();
    const std::string& id() const;
    Layout* ancestor() const;
};
