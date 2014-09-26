#include "WebRequest.h"

void WebRequest::clear(EthernetClient* client)
{
  this->client = client;
  this->state = SERV_REQ_METHOD;
  this->method = "";
  this->resource = "";
  this->params = "";
}

bool WebRequest::bodyAvailable()
{
  return this->client->available();
}

char WebRequest::bodyRead()
{
  return this->client->read();
}
