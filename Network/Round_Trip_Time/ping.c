
/* myping.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
 * whole or in part in accordance to the General Public License (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <stdlib.h>
#include "network.h"
#include "cycle.h"

/* ping - Create message and send it.
 */
void ping(struct sockaddr_in *addr, int pid) {
  int my_socket;

  struct protoent *proto=NULL;
  proto = getprotobyname("ICMP");
  my_socket = socket(PF_INET, SOCK_RAW, proto->p_proto);
  if ( my_socket < 0 ){
    perror("socket");
    return;
  }

  const int val=255;
  if ( setsockopt(my_socket, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
    perror("Set TTL option");

  int  counter = 1;
  struct packet pckt;
  double sum = 0.0;
  int loop_times = 1000;
  for(counter = 1; counter < loop_times ; counter ++) {
    init_ICMP_ECHO_packet(&pckt, counter, pid);
    struct sockaddr_in r_addr;
    int len=sizeof(r_addr);
    int send_result, recv_result;

    ticks t1 = getticks();
    send_result = sendto(my_socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr));
    recv_result = recvfrom(my_socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &len);
    ticks t2 = getticks();
    double etime = elapsed(t2, t1);
    sum += etime;
    if(send_result == 0 || recv_result == 0){
      printf("error sending or recving!");
      exit(0);
    }
  }
  printf("%f\n", (sum / loop_times) * 0.417 / 1e6);
  close(my_socket);
}

int main(int argc, char *argv[]) {
  int pid = -1;
  pid = getpid();

  struct sockaddr_in addr;
  sockaddr_generator(&addr, argv[1], -1);

  ping(&addr, pid);

  return 0;
}

