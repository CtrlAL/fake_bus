#include <glib.h>
#include <gio/gio.h>
#include "g_lib_to_work.h"

//call SERVICE OBJECT INTERFACE METHOD [SIGNATURE [ARGUMENT...]]

static
void Get_Call_Args(gchar **obj_name, gchar **obj_path, gchar **i_name, gchar** method_name, char **argv){   
        *obj_name = argv[1];
        *obj_path = argv[2];
        *i_name = argv[3];
        *method_name = argv[4];
        return 0;
}

int bus_call(int argc, char** argv){
    GDBusProxy *proxy;
    GVariant *result;
    GError *error = NULL;
    gchar *obj_name;
    gchar *obj_path; 
    gchar *i_name; 
    gchar *method_name;

    Get_Call_Args(&obj_name, &obj_path, &i_name, &method_name, argv);
    

    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, NULL, obj_name, 
                                                obj_path, i_name, 
                                                NULL, &error);


  

    result = g_dbus_proxy_call_sync(proxy, method_name, 
                                    NULL, G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, &error);
    
    if(IsError(error)){
        g_object_unref(proxy);
        g_object_unref(result);
        FreeError(error);
        g_print("Error message %s \n", error->message);
        return 0;
    }

    PrintVariant(g_variant_get_child_value(result, 0), NULL);

    g_object_unref(proxy);
    g_variant_unref(result);
    
    return 0;
}
