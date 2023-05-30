
#include <glib.h>
#include <gio/gio.h>
#include "g_lib_to_work.h"

static
void FirstPrintDBusList(GVariant *result){
    const gchar* const* names = NULL;
    g_variant_get(result, "(^as)", &names);
    for(int i = 0; names[i] != NULL; i++){
        g_print("Obj name:  %s \n", names[i]);
    }

}

static
void SecondPrintDBusList(GVariant *result){
    gchar** sec_names = NULL;
    g_variant_get(result, "(^as)", &sec_names);
    for(gchar **p = sec_names ; *p; ++p){
        g_print("Obj name:  %s \n", *p);
    }
}

int bus_list(void){

    GVariant *result;
    GDBusProxy *bus_proxy;
    GError *error = NULL;

    bus_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 0, NULL, "org.freedesktop.DBus", 
                                                "/org/freedesktop/DBus", "org.freedesktop.DBus",
                                                NULL, &error);

    if(error != NULL){
        g_print("Error %s \n", error->message);
        return 1;
    }

    result = g_dbus_proxy_call_sync(bus_proxy, "ListNames", 
                                    NULL, G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, &error);

    PrintVariant(result, NULL);
    
    if(error != NULL){
        g_print("Error %s \n", error->message);
        return 1;
    }

    SecondPrintDBusList(result);
    
    g_variant_unref(result);
    g_object_unref(bus_proxy);
    

    return 0;
}   
