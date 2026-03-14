#include "layout.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Initialisation de l'identifiant global
int Layout::__global__id__ = 0;
/**
 * Constructeur :
 *  - Si un ID n'est pas fourni, génère un identifiant unique.
 *  - Si c'est la racine (pas d'ancêtre), crée un layout horizontal par défaut
 *    contenant un seul sous-nœud de type LayoutType::Node.
 */
Layout::Layout(Configuration* configuration, Layout* ancestor, LayoutType type, const std::string& id)
    : _ancestor(ancestor), _type(type) {
    if (id.empty() && _ancestor) {
        _id = std::string(" ");
        _id[0] = 'A' + __global__id__++;
        if(_id[0] > 'Z') {
            std::ostringstream slabel;
            slabel << Layout::__global__id__;
            _id += slabel.str();
        }
    } 
    else if(_ancestor) {
        _id = id;
    }

    // Si c'est la racine : un layout horizontal par défaut avec un enfant unique
    if (!_ancestor) {
        _id = "_root";
        _configuration = configuration;
        _type = LayoutType::Horizontal;
        _weights.emplace_back(1.0);
        _sublayouts.emplace_back(std::make_unique<Layout>(nullptr, this, LayoutType::Node));
    }
}
/**
 * Retourne l'indice d'un sous-layout spécifique dans le vecteur _sublayouts.
 * Renvoie -1 si le layout n'est pas trouvé.
 */
int Layout::index_of(const Layout* layout) const {
    for (size_t i = 0; i < _sublayouts.size(); ++i)
        if (_sublayouts[i].get() == layout)
            return static_cast<int>(i);
    return -1;
}
/**
 * Scinde un nœud (type LayoutType::Node) en deux sous-nœuds, selon un type donné (LayoutType::Horizontal / LayoutType::Vertical).
 * - Si le parent est du même type, on ajoute directement un nouveau LayoutType::Node frère.
 * - Sinon, on encapsule le nœud courant dans un nouveau conteneur du type demandé.
 */
void Layout::split(LayoutType type) {
    if (_type != LayoutType::Node || !_ancestor) return;

    int idx = _ancestor->index_of(this);
    if (idx < 0) return;

    double w = _ancestor->_weights[idx];

    if (_ancestor->_type == type) {
        // Même type : on ajoute un nouveau sous-layout frère
        _ancestor->_weights[idx] = w / 2.0;
        _ancestor->_weights.insert(_ancestor->_weights.begin() + idx + 1, w / 2.0);
        _ancestor->_sublayouts.insert(_ancestor->_sublayouts.begin() + idx + 1,
                                      std::make_unique<Layout>(nullptr, _ancestor, LayoutType::Node));
    } else {
        // Type différent : on encapsule le nœud courant dans un nouveau sous-layout
        auto buffer = std::make_unique<Layout>(nullptr, _ancestor, type); // " "
        buffer->_weights = {0.5, 0.5};
        buffer->_sublayouts.push_back(std::make_unique<Layout>(nullptr, buffer.get(), LayoutType::Node, _id));
        buffer->_sublayouts.push_back(std::make_unique<Layout>(nullptr, buffer.get(), LayoutType::Node));

        _ancestor->_sublayouts[idx] = std::move(buffer);
    }
}
/**
 * Fusionne le layout courant avec un autre ayant le même ancêtre,
 * à condition qu'ils soient contigus dans le vecteur des sous-layouts.
 */
std::vector<std::string> Layout::siblings() const {
    std::vector<std::string> list = {};
    for(auto& item: _ancestor->_sublayouts)
        list.push_back(item->id());
    return list;
}
bool Layout::can_merge(MergeDirection direction) {
    if (!_ancestor) return false;
    int idx = _ancestor->index_of(this);
    LayoutType ancestor_type = _ancestor->type();
    auto siblings_list = siblings();
    switch(direction) {
    case MergeDirection::Top:
        return (ancestor_type == LayoutType::Vertical && idx > 0);
    case MergeDirection::Right:
        return (ancestor_type == LayoutType::Horizontal && idx < static_cast<int>(siblings_list.size()) - 1);
    case MergeDirection::Bottom:
        return (ancestor_type == LayoutType::Vertical && idx < static_cast<int>(siblings_list.size()) - 1);
    case MergeDirection::Left:
        return (ancestor_type == LayoutType::Horizontal && idx > 0);
    }
    return false;
}
void Layout::merge(MergeDirection direction) {
    if(!_ancestor) return;
    if(!can_merge(direction)) return;
    int idx = _ancestor->index_of(this);
    auto siblings_list = siblings();
    merge(siblings_list[direction == MergeDirection::Top || direction == MergeDirection::Left ? idx-1 : idx + 1]);
}
void Layout::merge(const std::string& id) {
    if (!_ancestor) return;

    int idx = _ancestor->index_of(this);
    if (idx < 0) return;

    auto other_opt = _ancestor->get(id);
    if (!other_opt.has_value()) return;

    Layout& other = other_opt->get();
    if (other._ancestor != _ancestor) return;

    int other_idx = _ancestor->index_of(&other);
    if (other_idx < 0) return;

    // Vérifie qu'ils sont bien contigus
    if (other_idx != idx - 1 && other_idx != idx + 1) return;

    // Fusionne les poids et supprime l'autre layout
    double new_weight = _ancestor->_weights[idx] + _ancestor->_weights[other_idx];
    _ancestor->_weights[idx] = new_weight;

    // Efface le layout fusionné
    _ancestor->_sublayouts.erase(_ancestor->_sublayouts.begin() + other_idx);
    _ancestor->_weights.erase(_ancestor->_weights.begin() + other_idx);

    // Si l'ancetre ne contient plus qu'un layout, on peut supprimer un niveau
    if(_ancestor->_sublayouts.size() == 1) {
        if(_ancestor->_ancestor) {
            idx = _ancestor->_ancestor->index_of(_ancestor);
            _ancestor->_ancestor->_sublayouts[idx] = std::make_unique<Layout>(nullptr, _ancestor->_ancestor, LayoutType::Node, _id);
        }
    }
}
/**
 * Accesseurs simples
 */
LayoutType Layout::type() const { 
    return _type; 
}
void Layout::set_type(const LayoutType type) { 
    _type = type; 
}
double Layout::weight() { 
    if (!_ancestor) return -1.0;
    int idx = _ancestor->index_of(this);
    if (idx < 0) return -1.0;
    return _ancestor->_weights[idx];
}
void Layout::set_weight(double new_weight) {
    if (!_ancestor) return;
    int idx = _ancestor->index_of(this);
    if (idx < 0) return;
    double initial_weight = _ancestor->_weights[idx];
    double to_deploy = initial_weight - new_weight;
    int how_many = _ancestor->_sublayouts.size() - (idx + 1);
    _ancestor->_weights[idx] = new_weight;
    for(size_t i=idx+1; i<_ancestor->_sublayouts.size(); i++) _ancestor->_weights[i] += (to_deploy / how_many);
}
/**
 * Recherche récursive d’un layout par ID.
 * Retourne un std::optional<std::reference_wrapper<Layout>> :
 * - std::nullopt si non trouvé
 * - une référence sur le Layout sinon
 */
std::optional<std::reference_wrapper<Layout>> Layout::get(const std::string& id) {
    if (id == _id)
        return *this;

    for (auto& child : _sublayouts) {
        auto result = child->get(id);
        if (result.has_value())
            return result;
    }

    return std::nullopt;
}
std::map<std::string, double[4]> Layout::nodes_bounds(bool include_all) {
    std::map<std::string, double[4]> map;
    nodes_bounds(map, 0.0, 0.0, 1.0, 1.0, include_all);
    return map;
}
void Layout::nodes_bounds(std::map<std::string, double[4]>& buffer, double x, double y, double width, double height, bool include_all) {
    if(include_all || _type == LayoutType::Node) {
        buffer[_id][0] = x; buffer[_id][1] = y; buffer[_id][2] = width; buffer[_id][3] = height;
    }
    if(_type != LayoutType::Node) {
    // else {
        double total = 0;
        for (double w : _weights) total += w;
        double offset = 0;
        for (size_t i = 0; i < _sublayouts.size(); ++i) {
            double ratio = _weights[i] / total;
            if (_type == LayoutType::Horizontal) {
                double w = width * ratio;
                _sublayouts[i]->nodes_bounds(buffer, x + offset * width, y, w, height, include_all);
                offset += ratio;
            } else { // VERTICAL
                double h = height * ratio;
                _sublayouts[i]->nodes_bounds(buffer, x, y + offset * height, width, h, include_all);
                offset += ratio;
            }
        }
    }
}
const std::vector<std::unique_ptr<Layout>>& Layout::sublayouts() const { 
    return _sublayouts; 
}
const std::vector<double>& Layout::weights() const { 
    return _weights; 
}
std::vector<double> Layout::absolute_weights() {
    std::vector<double> v;
    auto siblings_list = siblings();
    std::cerr << "flag" << std::endl;
    auto nb = this->nodes_bounds(true);
    for(auto [key, value]: nb) {
        std::cerr << "+" << key << std::endl;
    }
    for(int i=0; i<_sublayouts.size(); i++) {
        v.push_back(nb[_sublayouts[i]->id()][ancestor()->type() == LayoutType::Horizontal ? 2 : 3]);
    }
    // for(auto item: sublayouts()) {

    // }
    // for(auto sibling: siblings()) {
    //     v.push_back(nb[sibling][ancestor()->type() == LayoutType::Horizontal ? 2 : 3]);
    // }
    return v;
}
const std::string& Layout::id() const { 
    return _id; 
}
Layout* Layout::ancestor() const { 
    return _ancestor; 
}
/*************
 *** DEBUG ***
 *************/

/**
 * Affiche la structure hiérarchique complète dans la console.
 * Exemple de sortie :
 *   1:LayoutType::Horizontal(1)
 *   .LayoutType::Node:2(1)
 */
void Layout::dump(int depth) const {
    for (int i = 0; i < depth; ++i)
        std::cout << '.';

    double weight = 1.0;
    if (_ancestor) {
        int idx = _ancestor->index_of(this);
        if (idx >= 0)
            weight = _ancestor->_weights[idx];
    }

    std::cout << static_cast<int>(_type) << ":" << _id << "(" << weight << ")" << std::endl;

    for (const auto& child : _sublayouts)
        child->dump(depth + 1);

    if (depth == 0)
        std::cout << "----------------------" << std::endl;
}
/**
 * Fonction récursive : génère du code PostScript pour le Layout courant.
 * Chaque rectangle est tracé et annoté avec son identifiant (_id).
 */
void Layout::render_html(std::ofstream& out,
                       double x, double y, double width, double height) const {
    // Dessine un rectangle
    out << "draw_rect(ctx," << x << ", " << y << ", " << width << ", " << height << ");";
    out << "ctx.strokeText(\"" << _id << "\", " << (x + 3) << ", " << (y + 10) << ");";
    // Affiche le texte (ID) dans le coin supérieur gauche du rectangle

    // Si c'est une feuille, on s'arrête ici
    if (_type == LayoutType::Node)
        return;

    // Sinon, on divise récursivement selon le type
    double total = 0;
    for (double w : _weights) total += w;

    double offset = 0;
    for (size_t i = 0; i < _sublayouts.size(); ++i) {
        double ratio = _weights[i] / total;
        if (_type == LayoutType::Horizontal) {
            double w = width * ratio;
            _sublayouts[i]->render_html(out, x + offset * width, y, w, height);
            offset += ratio;
        } else { // VERTICAL
            double h = height * ratio;
            _sublayouts[i]->render_html(out, x, y + offset * height, width, h);
            offset += ratio;
        }
    }
}
/**
 * Fonction principale : génère un fichier PostScript complet avec cadres et identifiants.
 */
void Layout::draw_html(const std::string& filename,
                             double width, double height) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    out     << "<!doctype html>"
            << "<html>"
            << "<head>"
            << "  <script>"
            << "function draw_rect(ctx, x, y, w, h) {"
            << "  ctx.beginPath();"
            << "  ctx.moveTo(x, y);"
            << "  ctx.lineTo(x + w, y);"
            << "  ctx.lineTo(x + w, y + h);"
            << "  ctx.lineTo(x, y + h);"
            << "  ctx.closePath();"
            << "  ctx.stroke();"
            << "}"
            << "function onload() {"
            << "  let canvas = document.getElementById('main-canvas');"
            << "  let ctx = canvas.getContext('2d');";

    render_html(out, 0, 0, width, height);

    out     << "}"
            << "  </script>"
            << "</head>"
            << "<body onload=\"onload();\">"
            << "  <canvas id=\"main-canvas\" width=\"600\" heigh=\"400\"></canvas>"
            << "</body>"
            << "</html>";

    out.close();

    std::cout << "Fichier HTML : " << filename << std::endl;
}
Layout* Layout::root() {
    Layout* current = this;
    while(current->_ancestor) current = current->_ancestor;
    return current;
}
Configuration* Layout::configuration() {
    return root()->_configuration;
}
unsigned int Layout::get_index() {
    return configuration()->get_layout_index(root());
}
Graph* Layout::graph(const std::string& id) {
    return configuration()->get_layout_graph(get_index(), id);
}
