#include <glib.h>
#include <gio/gio.h>
#include "g_lib_to_work.h"

static
void PrintArg(GDBusArgInfo** args, gchar* pattern){
    if(args[0]){
        gchar* res = "";
        for(int i = 0; args[i] != NULL; i++){
            if(args[i] !=NULL){
                
                gchar* m_name = (args[i]->name) ? args[i]->name : "";
                gchar* sign = (args[i]->signature) ? args[i]->signature : "";
                if(sign)
                    sign = g_strconcat(":", sign, NULL);
                
                res = g_strconcat(res, m_name, sign, NULL);
                if(args[i + 1]){
                    res = g_strconcat(res, ",", NULL);
                }
            }
        }
        g_print(pattern, res); //"(%s)"
    }
}

static
void PrintMethods(GDBusMethodInfo ** m_info){
    printf("--Methods:\n");
    for (int j = 0; m_info[j] != NULL; j++) {
        if(m_info[j])
            g_print("  %s ", m_info[j]->name);
        
        PrintArg(m_info[j]->in_args, "(%s)");
        PrintArg(m_info[j]->in_args, " out [%s]");
        g_print("\n");
    }
}

static
void PrintPropertys(GDBusPropertyInfo **p_info){
    printf("--Properties:\n");
    for (int k = 0; p_info[k] != NULL; k++) {
        if(p_info[k])
            g_print("  %s (%s)\n", p_info[k]->name, p_info[k]->signature);
    }
}

static
void PrintSignals(GDBusSignalInfo **s_info){
    printf("--Signals:\n");
    for (int z = 0; s_info[z] != NULL ; z++) {
        if(s_info[z]){
            g_print("%s ", s_info[z]->name);
            PrintArg(s_info[z]->args, "(%s)");
        }
    }
}

int bus_introspect(int argc, char** argv){
    
    GDBusProxy *proxy;
    GVariant *result = NULL;
    GError *error = NULL;
    
    gchar *obj_name = "org.freedesktop.systemd1"; //"org.freedesktop.DBus"; //
    gchar *obj_path = "/org/freedesktop/systemd1";//"/org/freedesktop/DBus"///
    gchar *i_name = "org.freedesktop.DBus.Introspectable";

    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 0, NULL, obj_name, 
                                                obj_path, i_name, 
                                                NULL, &error);


    result = g_dbus_proxy_call_sync(proxy, "Introspect", 
                                    NULL, G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, &error);
    g_object_unref(proxy);
    

    GVariantIter *interfaces = g_variant_iter_new(result);
    
    g_variant_unref(result);

    const gchar *interface_names;
    g_variant_iter_next(interfaces, "s", &interface_names, NULL);


    g_print(interface_names);
    GDBusNodeInfo *node_info = g_dbus_node_info_new_for_xml(interface_names, NULL);
    if(node_info == NULL)
        return 2;

    for (int i = 0; node_info->interfaces[i] != NULL; i++) {
        GDBusInterfaceInfo *interface_info = node_info->interfaces[i];
        printf("Interface: %s\n", interface_info->name);
        
        if(interface_info->methods[0] != NULL){
            PrintMethods(interface_info->methods);
        }
        
        if(interface_info->properties[0] != NULL){        
            PrintPropertys(interface_info->properties);
        }
        if(interface_info->signals[0] != NULL){
            PrintSignals(interface_info->signals);
        }
    }
    g_dbus_node_info_unref(node_info);
    return 0;
}

