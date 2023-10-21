
/*
"interface" : [
    {
        "name" : "channel01",
        "type" : "MQTT channel",
        "userID" : "wolfSSL",
        "CAcert" : "wolfSSL_CA.pem"
    }
]
*/

typedef enum {
    WD_INTF_MQTT,
} WD_INTF_type;

typedef struct {
    const char *name;
    WD_INTF_type type;
    const char *userID;
    const char *CAcert;
} WD_interface;

/*
"wire" : [
    {
        "type" : "bit.stream",
        "from" : "GP15",
        "to" : "channel01"
    }
]
*/

typedef enum {
    WD_WIRE_BITSTREAM,
} WD_WIRE_type;

typedef struct {
    WD_WIRE_type type;
} WD_wire;
