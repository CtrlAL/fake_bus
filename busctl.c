#include <glib.h>

int bus_status(int argc, char** argv);
int bus_list(void);
int bus_property_get(int argc, char **argv);
int bus_property_set(int argc, char **argv);
int bus_introspect(int argc, char** argv);
int bus_emit(void);
int bus_call(int argc, char** argv);

/*
emit OBJECT INTERFACE SIGNAL [SIGNATURE [ARGUMENT...]]
                           Emit a signal
get-property SERVICE OBJECT INTERFACE PROPERTY...
                           Get property value
set-property SERVICE OBJECT INTERFACE PROPERTY SIGNATURE ARGUMENT...
                           Set property value
help                     Show this help

*/


struct bus_call_struct { 
    void (*bus_void_func) (void);
    void (*bus_arg_func) (int argc, char** args);
    char *bus_comand_name;
};

struct bus_call_struct command[] = {
    {bus_list, NULL, "list"},
    {NULL, bus_call, "call"},
    {NULL, bus_introspect, "introspect"},
    {NULL, bus_status, "status"},
    {NULL, bus_property_get, "get-property"},
    {NULL, bus_property_set, "set-property"},
    {bus_emit, NULL, "emit"},
};

struct bus_call_struct FindCommand(char *arg_command){
    struct bus_call_struct res = {NULL, NULL, NULL};
    int n = sizeof(command) / sizeof(struct bus_call_struct);
    
    for(int i = 0; i < n; i++){
        if(strcmp(command[i].bus_comand_name, arg_command) == 0){
            res = command[i];
            break;
        };
            
    }
    return res;
}

char** shl(int argc, char **args){
    for(int i = 0; i < argc; i++){
        args[i] = args[i+1];
    }
    return args;
}


int main (int argc, char **argv){
    if(argc == 1)
        return 0;
    
    struct bus_call_struct comm = FindCommand(argv[1]);

    if(comm.bus_comand_name == NULL){
        return -1;
    }

    if(comm.bus_void_func != NULL){
        comm.bus_void_func();
    }else{
        argc--;
        char* new_argc = shl(argc, argv);
        comm.bus_arg_func(argc, argv);
    }
}

/*
monitor [SERVICE...]     Show bus traffic
capture [SERVICE...]     Capture bus traffic as pcap
tree [SERVICE...]        Show object tree of service
*/