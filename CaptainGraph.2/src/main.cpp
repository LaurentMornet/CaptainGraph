#include "main.h"
#include <iostream>


int main(int argc, char** argv) {
    Manager manager;
    QApplication app(argc, argv);
    MainWindow main_window(&manager);
    main_window.show();
    return app.exec();
    // Configuration configuration;
    // configuration.set_label("Demo Configuration with Smart Pointers");
    // auto& layout = configuration.add_layout();
    // configuration.set_layout_title(0, "First layout");
    // configuration.sublayout(0, "A").split(LayoutType::Horizontal);
    // configuration.sublayout(0, "B").split(LayoutType::Vertical);
    // configuration.layout(0).draw_html("layout_ids.htm", 150, 80);
    // configuration.print();

    // auto main_layout = std::make_shared<Layout>("A", "A");
    // auto zone_A = std::make_shared<Layout>("B", "Zone B");
    // auto zone_Aptr = main_layout->add_child(zone_A);
    // auto zone_A1 = std::make_shared<Layout>("C", "Zone C");
    // auto zone_A1ptr = zone_Aptr->add_child(zone_A1);

    // auto g = std::make_unique<Graph>();
    // g->set_id("graph1");
    // g->set_title("Sine curve");
    // g->add_plot({"plot1", "sin(x)", {0, 1, 2, 3}, {0, 0.84, 0.91, 0.14}});
    // configuration.graph_stack().add_graph(std::move(g));

    // zone_A1ptr->add_graph("graph1");
    // configuration.add_layout(main_layout);
    // configuration.print();
    // if(auto parent = zone_A1ptr->parent()) {
    //     std::cout << std::endl << "Parent de " << zone_A1ptr->label() << " est " << parent->label() << std::endl;
    // }
    // if(auto* gptr = configuration.get_graph("graph1")) {
    //     std::cout << std::endl << "Graph trouvé: " << gptr->title() << " (" << gptr->plots_count() << " plot(s)" << std::endl;
    // }

    // auto layout = configuration.layout("A");
    // main_layout->print(0);
}