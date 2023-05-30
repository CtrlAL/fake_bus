#include <glib.h>
#include <gio/gio.h>
#include "g_lib_to_work.h"

int bus_emit(void){
    GDBusConnection *connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

    // Создаем прокси для интерфейса, который отправляет сигнал
    GDBusProxy *proxy = g_dbus_proxy_new_sync(connection, G_DBUS_PROXY_FLAGS_NONE, NULL, "org.example.MyObject",
        "/org/example/MyObject", "org.example.MyInterface", NULL, NULL);

    // Создаем структуру GVariant, которая представляет сигнал
    GVariant *signal_data = g_variant_new("(ss)", "some signal data", "more signal data here");
    // Эмулируем вызов сигнала (emit)
    //g_dbus_connection_emit_signal(connection, "org.freedesktop.systemd1");

    g_variant_unref(signal_data);
    g_object_unref(proxy);
    g_object_unref(connection);
}
