#include <stdio.h>
#include <string.h>

#include "parson.h"
#include "wiredef.h"

/* Top arrya index */
#define GLOBAL_DEF 0

/* This should be replaced by a list of interfaces */
static WD_interface interface001;
static WD_interface *intf = &interface001;

static int find_intfType(const char *type)
{
    static const char *type_list[] = {
        "MQTT channel",
        NULL};

    int i;
    printf("ind_intfType\n");
    for(i = 0; type_list[i]; i++) {
        if(strcmp(type_list[i], type) == 0)
            return i;
    }
    return -1;
}

static int get_interface(JSON_Object *interface)
{
    const char *type;

    printf("get_interface 1\n");

    if((intf->name = json_object_get_string(interface, "name")) == 0) {
        printf("ERROR: Interface name\n");
        return -1;
    }

    printf("get_interface 2\n");
    if((type = json_object_get_string(interface, "type")) == 0) {
        printf("ERROR: Interface name\n");
        return -1;
    }

    printf("get_interface 2\n");
    if((intf->type = find_intfType(type)) < 0) {
        printf("ERROR: Interface type\n");
        return -1;
    }

    printf("get_interface 3\n");
    if ((intf->userID = json_object_get_string(interface, "userID")) == 0)
    {
        printf("ERROR: Interface userID\n");
        return -1;
    }

    printf("get_interface 4\n");
    if ((intf->CAcert = json_object_get_string(interface, "CAcert")) == 0)
    {
        printf("ERROR: Interface userID\n");
        return -1;
    }

    printf("name: %s\ntype: %d\nuserID: %s\nCAcert: %s\n",
        intf->name, intf->type, intf->userID, intf->CAcert);
    return 0;
}

static int get_wire(JSON_Object *wire)
{
    return 0;
}

static int get_global(JSON_Object *global) {
    int n, i;
    JSON_Array *interface;

    if(strcmp(json_object_get_string(global, "type"), "global") != 0) {
        printf("Not started with Global definition\n");
        return -1;
    }

    if((interface = json_object_get_array(global, "interface")) == NULL) {
        printf("ERROR: Interface array\n");
        return -1;
    }    
    n = json_array_get_count(interface);
    printf("n = %d\n", n);

    for (i = 0; i < n; i++)
    {
        if (get_interface(json_array_get_object(interface, i)) < 0)
        {
            printf("ERROR: Global\n");
            return -1;
        }
    }
    return 0;
}

static int get_node(JSON_Object *node)
{
    char *type;
    int i;

    typedef enum {
        WD_NODETYPE_MQTT,
        WD_NODETYPE_BOARD,
    } WD_nodetype;

    struct node {
        char *name;
        WD_nodetype type;
    };

    const char node_type[] = {
        { "MQTT", WD_NODETYPE_MQTT},
        { "board", WD_NODETYPE_BOARD},
        { NULL, 0 }
    };

    type = json_object_get_string(global, "type");
    if(type == NULL) {
        printf("ERROR: Missing type. ");
        return -1;
    }

    for(i = 0; node_type.type[i]; i++) {
        if(strcmp(node_type[i], type) == 0)
            return node_type[i].type;
    }

    printf("Invalid type name. ");
    return -1;
}

int main(int ac, char **av)
{
    JSON_Value *val;
    JSON_Object *global;
    JSON_Array *top;
    size_t i, n;

    val = json_parse_file("simple_wire.json");
    printf("val = %lx\n", (unsigned long)val);
    if((top = json_value_get_array(val)) == NULL) {
        printf("ERROR: Top array\n");
        return -1;
    }

    n = json_array_get_count(top);
    printf("n = %zu\n", n);

    for (i = 0; i < n; i++) {
        if(get_node(json_array_get_object(top, i)) < 0) {
            printf("Node[%d]\n", i);
        }
    }
    return 0;
}

#if 0
    printf("top = %lx\n", (unsigned long)top);
    if((global = json_array_get_object (top, 0)) == NULL ||
        get_global(global) < 0) {
        printf("ERROR: Global\n");
        return -1;  
    }
    n = json_array_get_count(top); 
    printf("n = %zu\n", n);

    for (i = 0; i < n; i++) {
        if(get_wire(json_array_get_object(top, i)) < 0) {
            printf("ERROR: Global\n");
            return -1;
        }
    }
}
#endif