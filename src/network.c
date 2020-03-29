#include "network.h"

#include <stdio.h>
#include "dyad.h"

#ifdef CONTROL
static void onConnect(dyad_Event *e) {
  printf("connected: %s\n", e->msg);
}

static void onData(dyad_Event *e) {
  printf("%s", e->data);
}
#elif ROBOT
static void onData(dyad_Event *e) {
  dyad_write(e->stream, e->data, e->size);
}

static void onAccept(dyad_Event *e) {
  dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, NULL);
  dyad_writef(e->remote, "Echo server\r\n");
}
#endif

void *networkRunner() {
  dyad_init();

  dyad_Stream *s = dyad_newStream();

#ifdef CONTROL /* setup networking for the client */
  dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
  dyad_addListener(s, DYAD_EVENT_DATA,    onData,    NULL);
  dyad_connect(s, "10.0.0.147", 80); /* TODO: make IP not hard Coded*/
#elif ROBOT /* setup networking for the server */
  dyad_addListener(s, DYAD_EVENT_ACCEPT, onAccept, NULL);
  dyad_listen(s, 80);
#endif

  while (dyad_getStreamCount() > 0) {
    dyad_update();
  }

  dyad_shutdown();
  return NULL;
}
