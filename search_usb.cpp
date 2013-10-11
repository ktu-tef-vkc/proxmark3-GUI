//-----------------------------------------------------------------------------
// Copyright (C) 2013
//
// Zygimantas Butkus buzzy.ltu@gmail.com
// Tautvydas Zelvys gtautvis@yandex.ru
//
// Kaunas University of Technology
// Telecommunications and electronics department
// Virtual Center of excellence vkc@ktu.lt
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------

#include "search_usb.h"
#include <QDebug>
#include <poll.h>

using namespace std;

search_usb::search_usb(): QThread()
{
    udev = udev_new();
    if (!udev) {
        qDebug() << "Can't create udev\n";
        exit(1);
    }
}

search_usb::~search_usb()
{

}

void search_usb::run()
{
    monitor();
}

int search_usb::monitor()
{

    struct udev_device *dev;

    /* Create the udev object */


    /* Set up a monitor to monitor hidraw devices */\
    struct udev_monitor *mon;
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "tty", NULL);
    udev_monitor_enable_receiving(mon);

    /* Get the file descriptor (fd) for the monitor.
    This fd will get passed to select() */
    int fd = udev_monitor_get_fd(mon);

    /* This section will run continuously, calling usleep() at
    the end of each pass. This is to demonstrate how to use
    a udev_monitor in a non-blocking way. */


    struct pollfd pfd;
    int ret;
    pfd.fd = fd;
    pfd.events = POLLIN;

    while (1) {


        ret = poll(&pfd,1,-1);
        /* Check if our file descriptor has received data. */
        if (ret > 0) {
            dev = udev_monitor_receive_device(mon);

            if (dev) {
                if (udev_device_get_action(dev)[0] == 'a')
                    emit device_state_change("added", QString("%1").arg(udev_device_get_devnode(dev)));
                else if (udev_device_get_action(dev)[0] == 'r')
                    emit device_state_change("removed", QString("%1").arg(udev_device_get_devnode(dev)));

            }
            else {
                printf("No Device from receive_device(). An error occured.\n");
            }
        }
        //usleep(250*1000);
        fflush(stdout);
    }
    return 0;
}

QList<QString> search_usb::enumerate()
{
    QList<QString> lst;
    struct udev_enumerate *uen = udev_enumerate_new(udev);

    struct udev_list_entry *devices, *dev_list_entry;

    struct udev_device *dev;
    udev_enumerate_add_match_subsystem(uen,"tty");
    udev_enumerate_scan_devices(uen);


    devices = udev_enumerate_get_list_entry(uen);

    udev_list_entry_foreach(dev_list_entry, devices) {

        const char *path;
        stringstream ss;

        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        ss << path;

        if (ss.str().find("ACM") != string::npos)
            lst.append(QString(udev_device_get_devnode(dev)));
    }

    return lst;

}
