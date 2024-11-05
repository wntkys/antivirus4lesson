#include <vector>

#include <gtk4-layer-shell.h>
#include <gtkmm.h>

struct TaskData
{
    std::string name;
    int pid;
};

struct WorkspaceData
{
    std::vector<TaskData> tasks;
};

struct WorkspacesData
{
    std::vector<WorkspaceData> workspaces;
};

class WorkspacesWidget : public Gtk::Widget
{
    WorkspacesData data;

    WorkspacesWidget()
    {
    }
};

class WNBar : public Gtk::Window
{
private:
    Gtk::Box top_container;

    Gtk::Box left_box;
    Gtk::Box center_box;
    Gtk::Box right_box;

public:
    WNBar()
    {

        gtk_layer_init_for_window(this->gobj());

        gtk_layer_auto_exclusive_zone_enable(this->gobj());

        gtk_layer_set_layer(this->gobj(), GTK_LAYER_SHELL_LAYER_TOP);

        gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_TOP, true);
        gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
        gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_RIGHT, true);

        top_container = Gtk::Box(Gtk::Orientation::HORIZONTAL, 20);
        top_container.add_css_class("top-container");

        left_box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
        left_box.add_css_class("left-pane");

        center_box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
        center_box.add_css_class("center-pane");
        center_box.set_hexpand();

        right_box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
        right_box.add_css_class("right-pane");

        top_container.append(left_box);
        top_container.append(center_box);
        top_container.append(right_box);

        this->set_child(top_container);

        GtkCssProvider *provider;
        GdkDisplay *display;

        provider = gtk_css_provider_new();
        display = gdk_display_get_default();

        gtk_style_context_add_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);

        GError *error = NULL;

        gtk_window_present(this->gobj());

        gtk_css_provider_load_from_path(provider, "style.css");
    }
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create("com.wntkys.bar");

    return app->make_window_and_run<WNBar>(argc, argv);
}