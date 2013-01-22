#ifndef DHTCPPROTOCOL_H
#define DHTCPPROTOCOL_H
/* [simple tcp file transfer]
 * client listen at data tcp port,
 * then issue link to server,
 * server link back to client,
 * transfer data.
 */
namespace DHtcp{

enum eCMD{
    CMD_START,
    CMD_STOP,

    //tcp file sendig
    FILE_SENT
};

}

#endif // DHTCPPROTOCOL_H
