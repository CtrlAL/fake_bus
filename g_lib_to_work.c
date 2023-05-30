#include <glib.h>
#include <gio/gio.h>
#include "g_lib_to_work.h"

void FreeError(GError *err){
    if(err)
        g_error_free(err);
}
int IsError(GError *err){
    if(err)
        return 0;
}
void PrintError(GError *err, char *pre_err_message){
    if(err != NULL){
        g_print(pre_err_message);
        g_print(" %s \n", err->message);
    }
}

void PrintVariant(GVariant *var, gchar *before_string){
    if(before_string != NULL)
        g_print("%s : ", before_string);

    g_print("%s \n", g_variant_print(var, TRUE));
}
