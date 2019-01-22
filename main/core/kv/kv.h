/*
 * Copyright (C) 2018  SuperGreenLab <towelie@supergreenlab.com>
 * Author: Constantin Clauzel <constantin.clauzel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KV_H_
#define KV_H_

#include <stdlib.h>
#include <stdbool.h>
#include "keys.h"

#define MAX_KVALUE_SIZE 517

void preinit_kv();
void postinit_kv();

int geti(const char * key);
void seti(const char * key, int value);
bool hasi(const char * key);
void defaulti(const char * key, int value);

void getstr(const char * key, char *value, const size_t length);
void setstr(const char * key, const char * value);
bool hasstr(const char * key);
void defaultstr(const char * key, const char * value);

/*
 * [GENERATED]
 */

int get_wifi_status();
void set_wifi_status(int value);
void get_wifi_ssid(char *dest, size_t len);
void set_wifi_ssid(const char *value);
void get_wifi_password(char *dest, size_t len);
void set_wifi_password(const char *value);
int get_time();
void set_time(int value);
int get_n_restarts();
void set_n_restarts(int value);
int get_ota_timestamp();
void set_ota_timestamp(int value);
void get_ota_server_ip(char *dest, size_t len);
void set_ota_server_ip(const char *value);
void get_ota_server_hostname(char *dest, size_t len);
void set_ota_server_hostname(const char *value);
void get_ota_server_port(char *dest, size_t len);
void set_ota_server_port(const char *value);
void get_ota_version_filename(char *dest, size_t len);
void set_ota_version_filename(const char *value);
void get_ota_filename(char *dest, size_t len);
void set_ota_filename(const char *value);
void get_broker_url(char *dest, size_t len);
void set_broker_url(const char *value);
int get_i2c_0_sda();
void set_i2c_0_sda(int value);
int get_i2c_0_scl();
void set_i2c_0_scl(int value);
int get_i2c_0_enabled();
void set_i2c_0_enabled(int value);
int get_i2c_1_sda();
void set_i2c_1_sda(int value);
int get_i2c_1_scl();
void set_i2c_1_scl(int value);
int get_i2c_1_enabled();
void set_i2c_1_enabled(int value);

/*
 * [/GENERATED]
 */

#endif
