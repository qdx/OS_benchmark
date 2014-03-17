#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <resolv.h>
#include <netinet/ip_icmp.h>

#define PACKETSIZE	64

const char *PORT_SMALLER_THAN_ZERO = 
"Port number should not be smaller than 0!";
const char *ILLEGIAL_SOCKET = 
"The provided socket is illegal!";

struct packet {
  struct icmphdr hdr;
  char msg[PACKETSIZE-sizeof(struct icmphdr)];
};

void panic_error(const char* msg){
  perror(msg);
  exit(0);
}

/* A convenient function that create an TCP/IP socket. 
*/
int create_new_socket(){
  // create a new socket
  int new_socket;
  // SOCK_STREAM means we are using a stream protocol, TCP
  // AF_INET means IP protocol addrss families
  new_socket = socket(AF_INET, SOCK_STREAM, 0);
  return new_socket;
}

/* This is a convenient function that get client
 * socket from a listening local socket, here we 
 * simply discard the client sockaddr information.
 * DONT use this function if you need client
 * sockaddr.
 */
int accept_incoming_connection(int my_socket){
  if(my_socket < 0){
    panic_error(ILLEGIAL_SOCKET);
  }

  int client_socket;

  struct sockaddr cli_addr;
  socklen_t clilen;
  clilen = sizeof(cli_addr);

  // here accept blocks, it can be configured as noblocking
  client_socket = accept(my_socket, &cli_addr, &clilen);

  if( client_socket < 0)
    return -1;

  return client_socket;
}

/* This is a convenient function to create a
 * IP family sockaddr with specified port and IP.
 * If no IP is provided, it create a local socket.
 */
void sockaddr_generator(struct sockaddr_in *addr, char *addr_string, int port) {
  // initialize server addr struct
  bzero(addr, sizeof(struct sockaddr_in));
  // use IP address family
  addr->sin_family = AF_INET;
  // set to local IP if NULL is given
  if(addr_string == NULL)
    addr->sin_addr.s_addr = INADDR_ANY;
  else{
    struct hostent *host_name;
    host_name = gethostbyname(addr_string);
    addr->sin_addr.s_addr = *(long*)host_name->h_addr;
  }
  if(port > 0)
    addr->sin_port = htons(port);
}

/* Checksum - standard 1s complement checksum
 */
unsigned short checksum(void *b, int len) {	
  unsigned short *buf = b;
  unsigned int sum=0;
  unsigned short result;

  for ( sum = 0; len > 1; len -= 2 )
    sum += *buf++;
  if ( len == 1 )
    sum += *(unsigned char*)buf;
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

void init_ICMP_ECHO_packet(struct packet* pckt, int seq_number, int pid){
  bzero(pckt, sizeof(struct packet));
  pckt->hdr.type = ICMP_ECHO;
  pckt->hdr.un.echo.id = pid;
  int i = 0;
  for ( i = 0; i < sizeof(pckt->msg) - 1; i++ )
    pckt->msg[i] = i + '0';
  pckt->msg[i] = 0;
  pckt->hdr.un.echo.sequence = seq_number++;
  pckt->hdr.checksum = checksum(pckt, sizeof(struct packet));
}


