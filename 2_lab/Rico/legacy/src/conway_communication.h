#ifndef CONWAY_COMMUNICATION_H
#define CONWAY_COMMUNICATION_H


struct conway_msg_t;
typedef struct conway_msg_t conway_msg;

#define COMM_TOP 0
#define COMM_BOTTOM 1
#define COMM_LEFT 2
#define COMM_RIGHT 3
#define COMM_TOP_LEFT 4
#define COMM_TOP_RIGHT 5
#define COMM_BOTTOM_LEFT 6
#define COMM_BOTTOM_RIGHT 7

void conway_send(int dst, int t, conway_msg *msg);
void conway_recv(int *src, int t, conway_msg *msg);


#endif // CONWAY_COMMUNICATION_H
