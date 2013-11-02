/*
 * Copyright (c) 2013, Rudolf Tammekivi.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <cutils/properties.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <private/android_filesystem_config.h>

#include "oncrpc.h"
#include "nv.h"

#define MAC_SIZE 6
#define WIFI_MAC_PROPERTY "persist.wifi.mac"
#define DRIVER_ARG_PROP_NAME "wlan.driver.arg"
#define WIFI_SOFTMAC_FILE "/data/misc/wifi/softmac"
#define BT_MAC_PATH_PROPERTY "ro.bt.bdaddr_path"

static void start_rpc_client(bool enable);
static bool is_modem_available(void);
static void handle_wlan_mac(void);
static void handle_bt_mac(void);

static void get_wlan_mac(unsigned char *buffer);
static void get_bt_mac(unsigned char *buffer);

int main()
{
	start_rpc_client(true);

	if (!is_modem_available()) {
		printf("Modem is not available\n");
		goto exit;
	}

	handle_wlan_mac();
	handle_bt_mac();

exit:
	start_rpc_client(false);
	return 0;
}

static bool is_modem_available(void)
{
	return nv_null() ? true : false;
}

static void start_rpc_client(bool enable)
{
	if (enable) {
		oncrpc_init();
		oncrpc_task_start();
	} else {
		oncrpc_task_stop();
		oncrpc_deinit();
	}
}

#define MAC_STRING_SIZE 18

static void write_file(char *file, char *mac_address, int user, int group)
{
	int fd;

	/* Set chmod permissions. */
	fd = open(file, O_RDWR | O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (fd <= 0) {
		printf("Failed to open %s\n", file);
		return;
	}

	/* In case there is something written, return. */
	if (read(fd, mac_address, MAC_STRING_SIZE)) {
		printf("MAC address already set\n");
		return;
	}

	write(fd, mac_address, MAC_STRING_SIZE);

	/* Set chown permissions. */
	fchown(fd, user, group);

	close(fd);
}

static void handle_wlan_mac(void)
{
	unsigned char wlanmac[MAC_SIZE];
	char wlan_mac_address[PROPERTY_VALUE_MAX];
	char wlan_arg_property[PROPERTY_VALUE_MAX];

	/* Only write the property if it is not set. This allows the user
	 * to override it manually. */
	if (!property_get(WIFI_MAC_PROPERTY, wlan_mac_address, NULL)) {
		get_wlan_mac(wlanmac);

		sprintf(wlan_mac_address, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
			wlanmac[0], wlanmac[1], wlanmac[2],
			wlanmac[3], wlanmac[4], wlanmac[5]);

		property_set(WIFI_MAC_PROPERTY, wlan_mac_address);
	}

#ifdef ATH_WLAN
	write_file(WIFI_SOFTMAC_FILE, wlan_mac_address, AID_SYSTEM, AID_WIFI);
#else
	sprintf(wlan_arg_property, "mac_param=%s", wlan_mac_address);
	property_set(DRIVER_ARG_PROP_NAME, wlan_arg_property);
#endif
}

static void get_wlan_mac(unsigned char *buffer)
{
	nv_item_type nv_item;
	int i;

	/* Read the WLAN MAC NV Item */
	nv_cmd_remote(NV_READ_F, NV_WLAN_MAC_ADDRESS_I, &nv_item);

	/* Convert endianness (reverse the order). */
	for (i = 0; i < MAC_SIZE; i++)
		buffer[(MAC_SIZE - 1) - i] = nv_item.mac_address[i];
}

static void handle_bt_mac(void)
{
	unsigned char btmac[MAC_SIZE];
	char bt_path_property[PROPERTY_VALUE_MAX];
	char bt_mac_address[MAC_STRING_SIZE];

	/* Bluetooth MAC address is stored in a file. If the file path is set,
	 * write the MAC address. */
	if (property_get(BT_MAC_PATH_PROPERTY, bt_path_property, NULL)) {
		get_bt_mac(btmac);

		snprintf(bt_mac_address, MAC_STRING_SIZE,
			"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
			btmac[0], btmac[1], btmac[2],
			btmac[3], btmac[4], btmac[5]);

		write_file(bt_path_property, bt_mac_address, AID_BLUETOOTH, AID_NET_BT_STACK);
	}
}

static void get_bt_mac(unsigned char *buffer)
{
	nv_item_type nv_item;
	int i;

	/* Read the BT MAC NV Item */
	nv_cmd_remote(NV_READ_F, NV_BT_MAC_ADDRESS_I, &nv_item);

	/* Convert endianness (reverse the order). */
	for (i = 0; i < MAC_SIZE; i++)
		buffer[(MAC_SIZE - 1) - i] = nv_item.mac_address[i];
}
