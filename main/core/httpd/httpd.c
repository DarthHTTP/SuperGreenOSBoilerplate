/*
 * =====================================================================================
 *
 *       Filename:  httpd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/2018 12:09:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "httpd.h"
#include "httpd_kv_handlers.h"

#include <stdlib.h>

#include <esp_http_server.h>

#include "../kv/kv.h"
#include "../log/log.h"

#define IS_URI_SEP(c) (c == '?' || c == '&' || c == '=')

/* static size_t get_char_count(const char *uri) {
  size_t i = 0;
  for (; uri[i] && !IS_URI_SEP(uri[i]); ++i) {}
  return i;
} */

static const char *move_to_next_elem(const char *uri) {
  for (; *uri && !IS_URI_SEP(*uri); ++uri) {}
  if (*uri) ++uri;
  return uri;
}

static const char *move_to_key_value(const char *uri, const char *name) {
  while (*(uri = move_to_next_elem(uri))) {
    if (strncmp(uri, name, strlen(name)) == 0) {
      break;
    }
  }
  if (!*uri) return uri;
  return move_to_next_elem(uri);
}

/*static int find_int_param(const char *uri, const char *name) {
  int res = 0;
  const char *uri_offset = move_to_key_value(uri, name);
  if (!*uri_offset) {
    return 0;
  }
  return res;
}*/

static void find_str_param(const char *uri, const char *name, char *out, size_t *len) {
  uri = move_to_key_value(uri, name);
  ESP_LOGI(SGO_LOG_NOSEND, "found: %s", uri);
  if (!*uri) {
    return;
  }
  int i = 0;
  for (i = 0; uri[i] && i < *len && !IS_URI_SEP(uri[i]); ++i) {
    out[i] = uri[i];
  }
  *len = i;
}

static esp_err_t geti_handler(httpd_req_t *req) {
  size_t len = 50;
  char name[50] = {0};
  find_str_param(req->uri, "k", name, &len);

  const kvi_handler *h = get_kvi_handler(name);
  if (!h) {
    return httpd_resp_send_404(req);
  }

  int v = geti(h->nvs_key);
  char ret[12] = {0};
  sprintf(ret, "%d", v);

  httpd_resp_send(req, ret, strlen(ret));
  return ESP_OK;
}

static esp_err_t seti_handler(httpd_req_t *req) {
  size_t len = 50;
  char name[50] = {0};
  find_str_param(req->uri, "k", name, &len);

  if (!hasi(name)) {
    return httpd_resp_send_404(req);
  }

  len = 50;
  char value[50] = {0};
  find_str_param(req->uri, "v", value, &len);
  int res = atoi(value);

  const kvi_handler *h = get_kvi_handler(name);
  if (h) {
    h->handler(res);
  } else {
    seti(name, res);
  }
  httpd_resp_send(req, "OK", 2);
  return ESP_OK;
}

static esp_err_t getstr_handler(httpd_req_t *req) {
  char name[50] = {0};
  size_t len = 50;
  find_str_param(req->uri, "k", name, &len);

  const kvs_handler *h = get_kvs_handler(name);
  if (!h) {
    return httpd_resp_send_404(req);
  }

  char v[MAX_KVALUE_SIZE] = {0};
  getstr(h->nvs_key, v, MAX_KVALUE_SIZE);

  httpd_resp_send(req, v, strlen(v));
  return ESP_OK;
}

static esp_err_t setstr_handler(httpd_req_t *req) {
  char name[50] = {0};
  size_t len = 50;
  find_str_param(req->uri, "k", name, &len);

  if (!hasstr(name)) {
    return httpd_resp_send_404(req);
  }

  len = 50;
  char value[50] = {0};
  find_str_param(req->uri, "v", value, &len);

  const kvs_handler *h = get_kvs_handler(name);
  if (h) {
    h->handler(value);
  } else {
    setstr(name, value);
  }
  httpd_resp_send(req, "OK", 2);
  return ESP_OK;
}

httpd_uri_t uri_geti = {
  .uri      = "/i",
  .method   = HTTP_GET,
  .handler  = geti_handler,
  .user_ctx = NULL
};

httpd_uri_t uri_seti = {
  .uri      = "/i",
  .method   = HTTP_POST,
  .handler  = seti_handler,
  .user_ctx = NULL
};

httpd_uri_t uri_getstr = {
  .uri      = "/s",
  .method   = HTTP_GET,
  .handler  = getstr_handler,
  .user_ctx = NULL
};

httpd_uri_t uri_setstr = {
  .uri      = "/s",
  .method   = HTTP_POST,
  .handler  = setstr_handler,
  .user_ctx = NULL
};

static httpd_handle_t server = NULL;

static httpd_handle_t start_webserver(void) {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_register_uri_handler(server, &uri_geti);
    httpd_register_uri_handler(server, &uri_seti);
    httpd_register_uri_handler(server, &uri_getstr);
    httpd_register_uri_handler(server, &uri_setstr);
  }
  return server;
}

void init_httpd() {
  ESP_LOGI(SGO_LOG_EVENT, "@MQTT Intializing MQTT task");

  start_webserver();
}
