#include <glib.h>
#include <gio/gio.h>

#include "g_lib_to_work.h"

static
void Get_Args(gchar **obj_name,gchar **obj_path, gchar **i_name, gchar** prop_name, char **argv)
{   
    *obj_name = argv[1];
    *obj_path = argv[2];
    *i_name = argv[3];
    if(argv[4] != NULL){
        *prop_name = argv[4];
    }
}


static
void PrintGetAll(GVariant* result){
    GVariant* sub_res = g_variant_get_child_value(result, 0);
    GVariantIter* iter = g_variant_iter_new(sub_res);
    gchar* key;
    GVariant* val;
    while(g_variant_iter_next(iter, "{sv}", &key, &val)){
        g_print("%s :", key);
        PrintVariant(val, NULL);
    }
}

static
GVariant* Get(GDBusProxy* proxy, gchar* i_name, gchar* prop_name, GError** error){
    GVariant* res;
    res = g_dbus_proxy_call_sync(proxy, "Get", 
                                    g_variant_new("(ss)", i_name, prop_name), G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, error);
    return res;
}

static
GVariant* GetAll(GDBusProxy* proxy, gchar* i_name, GError** error){
    GVariant* res;
    
    res = g_dbus_proxy_call_sync(proxy, "GetAll", 
                                    g_variant_new("(s)", i_name), G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, error);
    return res;
}

int bus_property_get(int argc, char **argv){
    gchar* name;
    gchar* path;
    gchar* interface;
    gchar* prop_name = NULL;
    if(argc < 4){
        g_print("To Few Arguments for get-property");
        return 2;
    }
    
    Get_Args(&name, &path, &interface, &prop_name, argv);
    GDBusProxy *proxy;
    GVariant *result = NULL;
    GError *error = NULL;
    gchar *call_method_interface = "org.freedesktop.DBus.Properties";        
    
    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 0, NULL, name, 
                                                path, call_method_interface,  
                                                NULL, &error);
    
    if(IsError(error)){
        PrintError(error, "NULL");
        FreeError(error);
        return 2;
    }
    
    if(prop_name != NULL){
        result = Get(proxy, interface, prop_name, &error);
        g_print("%s : ", prop_name);
        PrintVariant(result, NULL);
    }else{
        result = GetAll(proxy, interface, &error);
        PrintGetAll(result);
    }
    
    if(IsError(error)){
        PrintError(error, "NULL");
        FreeError(error);
        g_object_unref(proxy);
        return 2;
    }
    
    g_object_unref(proxy);  
}
int bus_property_set(int argc, char **argv){
    gchar* name;
    gchar* path;
    gchar* interface;
    gchar* prop_name = NULL;
    gchar* val = NULL;
    
    if(argc < 6){
        g_print("To Few Arguments for set-property");
        return 2;
    }
    
    Get_Args(&name, &path, &interface, &prop_name, argv);
    val = argv[5];

    GDBusProxy *proxy;
    GVariant *result = NULL;
    GError *error = NULL;
    gchar *call_method_interface = "org.freedesktop.DBus.Properties";
      
    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 0, NULL, name, 
                                                path, call_method_interface,  
                                                NULL, &error);
    
    
    result = g_dbus_proxy_call_sync(proxy, "Set", 
                                    g_variant_new("(ssv)", interface, prop_name, val), G_DBUS_CALL_FLAGS_NONE, 
                                    -1, NULL, error);
                         
    if(IsError(error)){
        PrintError(error, "NULL");
        FreeError(error);
        return 2;
    }
    
    PrintVariant(result, NULL);

    g_object_unref(proxy);  
    return 0;
}
