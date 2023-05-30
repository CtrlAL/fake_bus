#ifndef G_LIB_TO_WORK_H
#define G_LIB_TO_WORK_H

#include <glib.h>
#include <gio/gio.h>

void FreeError(GError *err);
int IsError(GError *err);
void PrintError(GError *err, char *pre_err_message);
void PrintVariant(GVariant *var, gchar *before_string);

#endif