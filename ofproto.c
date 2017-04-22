#include<stdio.h>
#include "openflow.h"
#include<malloc.h>
#include <string.h>
#include<stddef.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_PACKET_IN_BUFFER 100
#define MAX_TABLE_SUPPORTED 5
#define COOKIE 0xaabbccddeeccddee
# define TABLE_ID 0xaa
#define ZERO 0
void creat_pkt(unsigned int ch,unsigned char *packet);
void Send_Message(void *);
int main(int argc,char *argv[])
{
    unsigned char *packet = NULL ;
    creat_pkt(OFPT_ECHO_REQUEST,packet);
return 0;
}

long int xid_gen()
{
    double cpu_time;
    struct timeval tv;
    cpu_time = ((double) clock()) / CLOCKS_PER_SEC;
    gettimeofday(&tv, NULL);
    srandom((getpid() << 16) ^ tv.tv_sec ^ tv.tv_usec ^(long int) cpu_time);
    return random();
}
void  creat_pkt(unsigned int packet_type,unsigned char *packet)
{

                                  int num_element =0;struct ofp_hello *msg=NULL; unsigned length = 0; struct ofp_hello_elem_versionbitmap ver_element ; int ver=0 ;int num_of_hello_elem = 1 ;int i= 0;unsigned char *ptr=NULL;struct ofp_header header ;unsigned int echo=0,echo_msg_len=0 ; struct ofp_switch_features feature ;int main_connection = 0 ;struct ofp_switch_config sw_config ;struct ofp_packet_in pakin;
switch(packet_type)
{
case OFPT_HELLO :
                                 msg = (struct ofp_hello *)malloc(offsetof(struct ofp_hello,elements)+(num_of_hello_elem * sizeof(struct ofp_hello_elem_header)));
                                 msg->header.version = OFP_VERSION ;
                                 msg->header.type = OFPT_HELLO ;   
                                 msg->header.xid = xid_gen() ;
                                 length = sizeof(struct ofp_header);
                                 for(i=ZERO;i<=num_element;i++)
                                 {  
                                    msg->elements[i].type = OFPHET_VERSIONBITMAP ;
                                    msg->elements[i].length = (sizeof(struct ofp_hello_elem_versionbitmap) + sizeof(struct ofp_hello_elem_header)) ;//TBD
                                 }
                                 ver_element.type = OFPHET_VERSIONBITMAP ;
                                 ver_element.length = sizeof(struct ofp_hello_elem_versionbitmap); 
                                 ver_element.bitmaps[0] = 0x0c ;
                                 ptr =(unsigned char*)&msg->elements[0].length + 1;
                                 memcpy(ptr ,&ver_element,sizeof(struct ofp_hello_elem_versionbitmap)) ;
                                 msg->header.length = length + (sizeof(struct ofp_hello_elem_header) + ver_element.length) ;
                                 break ;
case OFPT_ECHO_REQUEST:          echo_msg_len = 4 ;
                                 ptr = (unsigned char *)malloc(sizeof(struct ofp_header) + echo_msg_len);
                                 memset(ptr,0x0,(sizeof(struct ofp_header) + echo_msg_len));
                                 header.version = OFP_VERSION ;
                                 header.type = OFPT_HELLO ;
                                 header.xid = xid_gen() ;
                                 header.length = (sizeof(struct ofp_header) + sizeof(unsigned int));
                                 echo=0xdead;
                                 memcpy(ptr,&header,sizeof(struct ofp_header));
                                 memcpy((ptr + (sizeof(struct ofp_header) )),&echo,sizeof(unsigned int));
                                 break ;
case OFPT_ECHO_REPLY ://Send back what came.
                                 break;      
case OFPT_EXPERIMENTER :   
                                 break ;

    /* Switch configuration messages. */

case    OFPT_FEATURES_REQUEST :   /* This message is sent by controller for the enquireing about the switch capability. */ 
                                  header.version = OFP_VERSION;
                                  header.type = OFPT_FEATURES_REQUEST ;
                                  header.length = sizeof(struct ofp_header );
                                  header.xid = xid_gen() ;
                                  Send_Message(&header);
                                  break ;
case   OFPT_FEATURES_REPLY :             /* 6 */
                                  feature.header.version = OFP_VERSION;
                                  feature.header.type = OFPT_FEATURES_REPLY ;
                                  feature.header.xid = xid_gen() ;
                                  feature.header.length = (sizeof(struct ofp_header ) + sizeof(struct ofp_switch_features));
                                  feature.datapath_id = 0xabcdeabcde ;
                                  feature.n_buffers = MAX_PACKET_IN_BUFFER ;
                                  feature.n_tables = MAX_TABLE_SUPPORTED ;
                                  feature.auxiliary_id = main_connection ;
                                  feature.pad[0] = 0x0 ;
                                  feature.pad[1] = 0x0 ;
                                  feature.capabilities = OFPC_FLOW_STATS | OFPC_TABLE_STATS | OFPC_IP_REASM | OFPC_BUNDLES ; 
                                  feature.reserved = 0xdead ; 
case  OFPT_GET_CONFIG_REQUEST :
                                  header.version = OFP_VERSION;              
                                  header.type = OFPT_GET_CONFIG_REQUEST ;
                                  header.length = sizeof(struct ofp_header );
                                  header.xid = xid_gen() ; 
                                  Send_Message(&header);
                                  break ;

      /* Controller/switch message */
case  OFPT_GET_CONFIG_REPLY   :  
                                  sw_config.header.version = OFP_VERSION ;
                                  sw_config.header.type = OFPT_GET_CONFIG_REPLY ;
                                  sw_config.header.length = sizeof(struct ofp_switch_config ); 
                                  sw_config.header.xid = xid_gen() ;
                                  sw_config.flags = OFPC_FRAG_NORMAL ;
                                  sw_config.miss_send_len = OFPCML_NO_BUFFER ;
                                  Send_Message(&sw_config);

  /* Controller/switch message */
case OFPT_PACKET_IN           :   memset(&pakin,0,sizeof(pakin)); 
                                  pakin.header.version = OFP_VERSION ;
                                  pakin.header.type = OFPT_PACKET_IN ;
                                  pakin.header.length = sizeof(struct ofp_packet_in);
                                  pakin.header.xid = xid_gen() ;
                                  pakin.buffer_id = 0 ;/* If the packet is not buffered then the value should be OFP_NO_BUFFER ,else a unique buffer_id */
                                  pakin.total_len = sizeof(packet); /*TBD: This is normally the size of ethernet frame*/
                                  pakin.reason = OFPR_TABLE_MISS ; 
                                  pakin.table_id = TABLE_ID ;// Should come as input to the function.
                                  pakin.cookie = COOKIE ;// -1 if the cookie can't be associated with any flow,like it came from the group table
                                  pakin.match.type = OFPMT_OXM ;
                                  pakin.match.length = 0 ;//TBD : Length of this ofp_match excluding the padding 
 
                                  break ; 
                                  /* Controller/switch message */

    /* Asynchronous messages. */
  //  OFPT_PACKET_IN          = 10, /* Async message */
  //  OFPT_FLOW_REMOVED       = 11, /* Async message */
  //  OFPT_PORT_STATUS        = 12, /* Async message */

}
}

void Send_Message(void * msg)
{
}

