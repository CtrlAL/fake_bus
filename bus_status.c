#include <gio/gio.h>
#include <gio-unix-2.0/gio/gunixconnection.h>
#include <unistd.h>

static
void bus_creds_dump(GVariant *creds, const gchar *prefix, gboolean verbose) {
    /*
    const gchar *key;
    GVariant *value;
    GVariantIter iter;
    g_print('TAK');
    
    g_variant_iter_init(&iter, creds);
    while (g_variant_iter_next(&iter, "{sv}", &key, &value)) {
        
        if (prefix != NULL)
            g_print("%s", prefix);

        g_print("%s => ", key);

        if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {
            g_print("%s", g_variant_get_string(value, NULL));
        } 
        else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
            g_print("(%lu bytes)", g_variant_get_size(value));
        if (verbose) {
            const gchar *str = (const gchar *)g_variant_get_data(value);

        g_print(": ");
        for (gsize i = 0; i < g_variant_get_size(value); i++)
            g_print("%02x ", str[i]);
        }
        } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT32)) {
            g_print("%d", g_variant_get_int32(value));
        } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT64)) {
            g_print("%i", g_variant_get_int64(value));
        } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT64)) {
            g_print("%i", g_variant_get_uint64(value));
        } else {
            g_print("unsupported");
        }
        g_print("\n");
        g_variant_unref(value);
    }
    */
}

static
int status(int argc, char **argv) {
    GDBusConnection *conn = NULL;
    GDBusProxy *proxy = NULL;
    gchar *pid_str;
    gchar *connection_name = NULL, *group_id = NULL;
    gint pid;
    gboolean success;
    GVariant *creds_data = NULL, *results = NULL;
    GError *error = NULL;
    int ret = -1;

    pid_str = g_strdup(argv[1]);
    
    conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error); // If pid argument is not given, use default system bus
    if (error != NULL) {
        g_print("Error while getting system D-Bus connection: %s\n",
        error->message);
        goto exit;
    }
    connection_name = g_dbus_connection_get_unique_name(conn);
    group_id = g_dbus_connection_get_guid(conn);
    g_print("%s\n", connection_name);
    // Get bus credentials
    proxy = g_dbus_proxy_new_sync(conn, G_DBUS_PROXY_FLAGS_NONE,
                                    NULL, "org.freedesktop.DBus", "/org/freedesktop/DBus",
                                    "org.freedesktop.DBus", NULL, &error);

    GFile *file = g_file_new_for_path("/org/freedesktop/DBus");
    GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN "," G_FILE_ATTRIBUTE_ACCESS_CAN_READ "," G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_QUERY_INFO_NONE, NULL, NULL);         


    if (error != NULL) {
        g_print("Error while creating D-Bus proxy: %s\n",
        error->message);
        goto exit;
    }
    results = g_dbus_proxy_call_sync(proxy, "GetConnectionCredentials",
                                        g_variant_new("(s)", ":1.0"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    if (error != NULL) {
        g_print("Error while getting connection credentials: %s\n",
        error->message);
        goto exit;
    }
    results = g_variant_get_child_value(results, 0);
    PrintVariant(results, NULL);

    GVariantIter* iter = g_variant_iter_new(results);
    gchar* key = NULL;
    GVariant* val = NULL;
    
    while(g_variant_iter_next(iter, "{sv}", &key, &val)){
        g_print("%s ", key);
        g_print("(%s)", g_variant_print(val, TRUE));
        g_print("\n");
    }
    ret = 0;

    exit:
    if (error != NULL)
        g_error_free(error);
    if (proxy != NULL)
        g_object_unref(proxy);
    if (conn != NULL)
        g_object_unref(conn);

    g_free(pid_str);
    return ret;
}

int bus_status(int argc, char** argv){
    return status(argc, argv);
}
